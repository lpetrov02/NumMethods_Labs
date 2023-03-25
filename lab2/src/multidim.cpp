#include <iostream>
#include <ostream>
#include <vector>

class Expression {};

template <class T>
class Array;

template <class E>
Array<typename E::value_type> evaluate(const E& expr) {
    using value_type = typename E::value_type;
    const auto n = expr.size();
    int s = 1;
    for (int i = 0; i < n.size(); i++) {
        s *= n[i];
    }
    Array<value_type> result(n);
    for (int i = 0; i < s; ++i) {
        result(i) = expr.evaluate(i);
    }
    return result;
}

template <class T>
class Array: public Expression {
public:
    using value_type = T;
private:
    std::vector<T> _data;
    std::vector<int> _dims;
    std::vector<int> _idxs;
public:
    Array(std::initializer_list<T> rhs): _data(rhs) {}
    explicit Array(std::vector<int> n_vect) {
        int s = 1;
        for (auto& x: n_vect) {
            s *= x;
        }
        _data = std::vector<int>(s);
        _dims = std::vector<int>(n_vect);
        _idxs = std::vector<int>();
        for (int i = 0; i < n_vect.size(); i++) {
            _idxs.push_back(i);
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

    int countIndex(std::vector<int> idx) {
        int s = idx[0];
        for (int i = 1; i < idx.size(); i++) {
            s = s * _dims[i] + idx[i];
        }
        return s;
    }

    T& operator()(std::vector<int> idx) { return _data[countIndex(idx)]; }
    const T& operator()(int idx) const { return _data[countIndex(idx)]; }
    T evaluate(int idx) { return _data[countIndex(idx)]; }
    T evaluate(int idx) const { return _data[countIndex(idx)]; }

    std::vector<int> size() const { return this->_dims; }
    void display(std::ostream& out) const {
        out << "Array(";
        const auto n = _data.size();
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
    value_type evaluate(std::vector<int> idx) { 
        int index = _a.countIndex(idx);
        return this->_a.evaluate(index) + this->_b.evaluate(index); 
    }
    value_type evaluate(std::vector<int> idx) const {
        int index = _a.countIndex(idx);
        return this->_a.evaluate(index) + this->_b.evaluate(index);
    }

    std::vector<int> size() const { 
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


int main(int argc, char* argv[]) {
    using T = float;
    Array<T> a{ {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
    std::vector<int> idx1 = { 1, 2 };
    std::cout << a(idx1) << std::endl;
    return 0;
}