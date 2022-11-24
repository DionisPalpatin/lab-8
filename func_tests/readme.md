Входные данные - это 3 или 4 параметра:
1. o in_file out_file
2. a/m in_file_1 in_file_2 out_file

Выходные данные: или матрица, или число (зависит от операции) выводится в файл

Негативные тесты:
1. Дано имя несуществующего файла с исходными данными
2. Некорректное количество аргументов (<3 или >4)
3. Аргументов 4, первый аргумент не a и не m
4. Аргументов 3, первый аргумент не o
5. Входной файл пустой
6. Во входном файле пустые строки
7. Во входном файле есть некорректно записанные числа
8. Попытка записать элемент по индексу большему, чем размер матрицы
9. Попытка записать элемент по индексу < 0
10. Размеры входных матриц неодинаковы (для сложения матриц)
11. Количество столбцов первой входной матрицы не равно количеству строку второй входной матрицы (для умножения)
12. Матрица не квадратная (для определителя матрицы)


Позитивные тесты:
1. Складываются две квадратные матрицы, состоящие более чем из одного элемента
2. Складываются две прямоугольные матрицы, состоящие более чем из одного элемента
3. Складываются две матрицы, состоящие из одного элемента
4. Умножаются две квадратные матрицы, состоящие более чем из одного элемента
5. Умножаются две прямоугольные матрицы, состоящие более чем из одного элемента
6. Умножаются две матрицы, состоящие из одного элемента
7. Вычисляется детерминант матрицы, состоящей более чем из 3 элементов
8. Вычисляется детерминант матрицы, состоящей из 3 элементов
9. Вычисляется детерминант матрицы, состоящей из 2 элементов
10. Вычисляется детерминант матрицы, состоящей из 1 элемента