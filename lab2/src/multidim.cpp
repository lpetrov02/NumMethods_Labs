#include <ostream>
#include <vector>

template <int N>
class Expression {};

template <class T, int N>
class Array;

template <class E, int N>
Array<typename E::value_type, N> evaluate(const E& expr) {
    using value_type = typename E::value_type;
    const auto n = expr.size();
    Array<value_type, 1> result(n);
    for (int i = 0; i < N; ++i) {
        result(i) = expr.evaluate(i);
    }
    return result;
}

template <class T, int N>
class Array: public Expression {
public:
    using value_type = T;
private:
    std::vector<T> _data;
    std::vector<int> _dims;
    int _s;
public:
    Array(std::initializer_list<T> rhs): _data(rhs) {}
    explicit Array(std::vector<int> n) {
        int s = 1;
        for (int i = 0; i < N; i++) {
            s *= n[i];
        }
        _s = s;
        _data = std::vector<int>(s);
        _dims = std::vector<int>();
        for (int i = 0; i < N; i++) {
            _dims.push_back(s / n[i]);
            s /= n[i];
        }
    }

    template <class E>
    Array(const E& expr,
           typename std::enable_if<std::is_base_of<Expression,E>::value,E>::type* dummy=nullptr):
    Array(::evaluate(expr)) {}

    Array() = default;
    ~Array() = default;
    Array(Array&& rhs) = default;
    Array(const Array& rhs) = default;
    Array& operator=(Array&& rhs) = default;
    Array& operator=(const Array& rhs) = default;

    T& operator()(std::vector<int> idx) { 
        int s = 1;
        for (int i = 0; i < N; i++) {
            s = s + idx[i] * _dims[i];
        }
        return _data[s];
    }
    
    const T& operator()(int i) const { return this->_data[i]; }
    T evaluate(int i) { return this->_data[i]; }
    T evaluate(int i) const { return this->_data[i]; }
    int size() const { return this->_data.size(); }
    void display(std::ostream& out) const {
        out << "Array(";
        const auto n = size();
        if (n != 0) { out << this->_data.front(); }
        for (int i=1; i<n; ++i) { out << ',' << this->_data[i]; }
        out << ')';
    }

};

template <class E>
typename std::enable_if<std::is_base_of<Expression,E>::value,std::ostream&>::type
operator<<(std::ostream& out, const E& expr) {
    expr.display(out); return out;
}

template <class E1, class E2>
class Plus: public Expression {

public:
    using value_type =
        typename std::common_type<typename E1::value_type,typename E2::value_type>::type;

private:
    const E1& _a;
    const E2& _b;

public:
    explicit Plus(const E1& a, const E2& b): _a(a), _b(b) {}
    value_type evaluate(int i) { return this->_a.evaluate(i) + this->_b.evaluate(i); }
    value_type evaluate(int i) const { return this->_a.evaluate(i) + this->_b.evaluate(i); }
    int size() const { 
        return this->_a.size(); 
    }
    void display(std::ostream& out) const {
        out << "Plus(" << this->_a << ", " << this->_b << ')';
    }

};

template <class E1, class E2>
typename std::enable_if<std::is_base_of<Expression,E1>::value &&
                        std::is_base_of<Expression,E2>::value,Plus<E1,E2>>::type
operator+(const E1& a, const E2& b) {
    return Plus<E1,E2>(a,b);
}