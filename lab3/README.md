# 1.  
Первая задача реализована в файле `src/1.cpp`. Нетрудно заметить, что метод бисекции не находит минимума, кроме случая невероятного везения - потому что он предназначен для выпуклых функция, каковой функция Растрыгина не является. Метод Ньютона работает хорошо, но не гарантирует находжение глобального экстремума.  
Сходится метод Ньютона, как видно, прилично быстрее, зачатую - за 1-2 шага.  
# 2.  
Реализация - в `src/2.cpp`. Картина та же, только случай многомерный. Поверзло, что матрица Гессе для функции Растрыгина диагональная, а то было бы больно.  
# 3 и 4.  
Реализация - в `src/3.cpp`. Вариант из №3 называется просто evalVolume/evalLevel, а для №4 - с добавкой ...Iq1000, потому что так интеллектуальнее.  
Что касается точности, то видно, что метод из №4 работает лучше. Взять хотя бы подсчёт объёма для h = H / 2 и для h = H. В случае №4 получившиеся два числа больше похожи на относящиеся как 1 к 2.  
Ну и вычисление уровня методом бисекции в №4 тоже даёт явно более точный результат.
