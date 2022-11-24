/**
 * Файл, в котором находятся все функции для работы с матрицами
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <errors.h>
#include <constants.h>
#include <file.h>
#include <matrix.h>


/**
 * Функция освобождения памяти, выделенной для матрицы. Освобождаются сначала строки, затем массив указателей на них.
   Если указатель matrix, переданный в функцию, нулевой, то функция просто прекращает работу.
 * @param matrix - указатель на массив указателей на строки матрицы.
 * @param total_rows_alloc - количество строк матрицы, под которые была выделена память.
 */
void free_matrix(int **matrix, size_t strings_allocated)
{
	if (!matrix)
		return;

	/* Обработка ради избежания ошибки в случае, если порядок указателей на строки был изменен. Так как строки находятся
	   в памяти последовательно, находим наименьший указатель и освобождаем уже его. */
	int *ptr_to_strings = matrix[0];
	for (size_t i = 1; i < strings_allocated; ++i)
	{
		if (ptr_to_strings > matrix[i])
			ptr_to_strings = matrix[i];
	}

	free(ptr_to_strings);
	free(matrix);
}


/**
 * Функция, выделяющая память под матрицу по схеме с массивом указателей на строки матрицы и непосредственно строки
 * матрицы, расположенные в памяти друг за другом. Размеры матрицы должны быть известны на момент вызова функции и
 * объявлены вне ее.
 * @param matrix - указатель на структуру, содержащая указатель на массив указателей на строки и размеры матрицы.
 * @return 0 в случае отсутствия ошибок, ненулевое число.
 */
int matrix_alloc(matrix_t *matrix)
{
	assert(matrix);

	int exit_code = OK;
	int **tmp_matrix = malloc(matrix->rows * sizeof(int *));

	if (!tmp_matrix)
		exit_code = MEMORY_ALLOC_ERROR;
	else
	{
		int *matrix_rows = calloc(matrix->rows * matrix->cols, sizeof(int));

		if (matrix_rows)
		{
			matrix->matrix = tmp_matrix;
			for (size_t i = 0; i < matrix->rows; ++i)
				matrix->matrix[i] = matrix_rows + i * matrix->cols;
		}
		else
		{
			exit_code = MEMORY_ALLOC_ERROR;
			free(tmp_matrix);
		}
	}

	return exit_code;
}


/**
 * Функция, создающая матрицу с нуля: выделяющая память под нее и считывающая матрицу из файла (эти действия происходят
 * с помощью отдельных функций).
 * @param matrix - указатель на структуру, содержащая указатель на массив указателей на строки и размеры матрицы.
 * @param filename - имя файла, из которого происходит считывание матрицы.
 * @return 0 в случае отсутствия ошибок, ненулевое число.
 */
int creat_matrix(matrix_t *matrix, const char *filename)
{
	assert(matrix && filename);

	FILE *src_file = NULL;
	size_t total_rows;
	int exit_code = OK;

	if (!(src_file = fopen(filename, "r")))
		exit_code = FILE_ERROR;
	else if (fscanf(src_file, "%zu %zu %zu\n", &matrix->rows, &matrix->cols, &total_rows) != 3)
		exit_code = SIZE_READ_ERROR;
	else if (matrix->rows < MIN_SIZE_VALUE || matrix->cols < MIN_SIZE_VALUE)
		exit_code = SIZE_VALUE_ERROR;
	else if (matrix_alloc(matrix))
		exit_code = MEMORY_ALLOC_ERROR;
	else if (read_matrix_from_file(src_file, matrix, total_rows))
		exit_code = EL_READ_ERROR;

	if (src_file)
		fclose(src_file);

	return exit_code;
}


/**
 * Функция, считывающая элементы матрицы из файла.
 * @param file - указатель на файл с данными.
 * @param matrix - указатель на матричную структуру.
 * @param total_rows - количество строк в файле, которое надо считать.
 * @return 0, если ошибок не было, иначе ненулевое значение.
 */
int read_matrix_from_file(FILE *file, matrix_t *matrix, size_t total_rows)
{
	assert(matrix && file);

	int exit_code = OK;
	size_t row_index, col_index;
	int value;

	for (size_t i = 0; i < total_rows && !exit_code; ++i)
	{
		exit_code = read_string(file, &row_index, matrix->rows, &col_index, matrix->cols, &value);
		if (!exit_code)
			*(matrix->matrix[row_index - 1] + (col_index - 1)) = value;
	}

	return exit_code;
}


/**
 * Функция вычисления определителя матрицы. Формулы для матриц размером 1х1, 2х2 и 3х3 зашиты в код. Определитель для
   матриц большего размера рекурсивно вычисляется с помощью разложения матрицы по первой строке.
 * @param matrix - указатель на матричную структуру.
 * @param det - указатель на переменную, хранящую значение детерминанта.
 * @return 0, если ошибок не было, иначе ненулевое значение.
 */
int determinant(matrix_t *matrix, double *det)
{
	assert(matrix && det);
	if (matrix->rows != matrix->cols)
		return DET_CANT_BE_CALC;

	int exit_code = OK;
	double det_tmp = 0;

	if (matrix->rows == 1)
		det_tmp = *(matrix->matrix[0]);
	else if (matrix->rows == 2)
	{
		// Данный массив элементов матрицы создается для удобства восприятия кода и его легкой читаемости.
		int elems[][2] = { { *(matrix->matrix[0]), *(matrix->matrix[0] + 1) },
			{ *(matrix->matrix[1]), *(matrix->matrix[1] + 1) } };
		det_tmp = elems[0][0] * elems[1][1] - elems[1][0] * elems[0][1];
	}
	else if (matrix->rows == 3)
	{
		// Данный массив элементов матрицы создается для удобства восприятия кода и его легкой читаемости.
		int elems[][3] = { { *(matrix->matrix[0]), *(matrix->matrix[0] + 1), *(matrix->matrix[0] + 2) },
			{ *(matrix->matrix[1]), *(matrix->matrix[1] + 1), *(matrix->matrix[1] + 2) },
			{ *(matrix->matrix[2]), *(matrix->matrix[2] + 1), *(matrix->matrix[2] + 2) } };
		det_tmp = elems[0][0] * elems[1][1] * elems[2][2] + \
			elems[0][1] * elems[1][2] * elems[2][0] + \
			elems[1][0] * elems[2][1] * elems[0][2] - \
			elems[0][2] * elems[1][1] * elems[2][0] - \
			elems[1][0] * elems[0][1] * elems[2][2] - \
			elems[0][0] * elems[1][2] * elems[2][1];
	}
	else
	{
		/* Алгоритм следующий:
		   1) Создаю переменную матричного типа и инициализирую поля количества строк и столбцов. В эту переменную и
			  будут записываться все подматрицы текущей матрицы.
		   2) Создаю переменную для хранения значения детерминанта подматрицы и переменную, которая отвечает за знак
			  определителя подматрицы.
		   3) Выделяю память для хранения подматриц и проверяю ее выделение.

		   Если все хорошо, далее в цикле:
		   4) Выделяю очередную подматрицу из матрицы.
		   4) Вычисляю детерминант этой подматрицы и проверяю успешность его вычисления.
		   5) Если все хорошо, прибавляю значение определителя текущей подматрицы к значению определителя матрицы (не
			  забывая поменять значение коэффициента, на который умножается очередной вычисленный определитель).

		   После цикла освобождаю память, выделенную под подматрицы. Значение детерминанта матрицы присваивается внешней
		   переменной.


		   k = (-1)^(i + j). i = 1 => k = (-1)^(1 + j) => т.к. j0 = 1, k0 = (-1)^2 = 1. Далее знак k чередуется. */

		matrix_t submatrix = { .rows = matrix->rows - 1, .cols = matrix->cols - 1, .matrix = NULL};
		double new_det_term;
		short int k = 1;

		exit_code = matrix_alloc(&submatrix);

		for (size_t j = 0; j < matrix->cols && !exit_code; ++j)
		{
			creat_submatrix(matrix, &submatrix, j);
			exit_code = determinant(&submatrix, &new_det_term);
			if (!exit_code)
			{
				det_tmp += k * (*(matrix->matrix[0] + j)) * new_det_term;
				k *= -1;
			}
		}

		free_matrix(submatrix.matrix, submatrix.rows);
	}

	if (!exit_code)
		*det = det_tmp;

	return exit_code;
}


/**
 * Функция выделения подматрицы из матрицы. Размеры подматрицы присваиваются ей внутри функции.
 * @param matrix - указатель на структурную переменную, содержащую исходную матрицу.
 * @param submatrix - указатель на структурную переменную для подматрицы.
 * @param excluded_col - номер столбца, по которому идет разложение.
 */
void creat_submatrix(matrix_t *matrix, matrix_t *submatrix, size_t excluded_col)
{
	assert(matrix && submatrix && excluded_col < matrix->cols);

	submatrix->rows = matrix->rows - 1;
	submatrix->cols = matrix->cols - 1;

	for (size_t i = 1; i < matrix->rows; ++i)
	{
		for (size_t j = 0; j < excluded_col; ++j)
			*(submatrix->matrix[i - 1] + j) = *(matrix->matrix[i] + j);
		for (size_t j = excluded_col + 1; j < matrix->cols; ++j)
			*(submatrix->matrix[i - 1] + (j - 1)) = *(matrix->matrix[i] + j);
	}
}


/**
 * Функция, вычисляющая произведение двух матриц. Размеры итоговой матрицы присваиваются ей внутри функции.
 * @param first - указатель на матричную переменную первой матрицы
 * @param sec - указатель на матричную переменную второй матрицы
 * @param res - указатель на матричную переменную матрицы, получившейся в результате умножение первых двух матриц
 * @return 0, если все хорошо, иначе ненулевое число
 */
int multiply_matrix(matrix_t *first, matrix_t *sec, matrix_t *res)
{
	assert(first && sec && res);

	if (first->cols != sec->rows)
		return CANT_BE_MULTIPLY;

	res->rows = first->rows;
	res->cols = sec->cols;

	if (matrix_alloc(res))
		return MEMORY_ALLOC_ERROR;

	for (size_t i = 0; i < first->rows; ++i)
	{
		for (size_t j = 0; j < sec->cols; ++j)
		{
			int new_el = 0;
			for (size_t k = 0; k < first->cols; ++k)
				new_el += *(first->matrix[i] + k) * *(sec->matrix[k] + j);
			*(res->matrix[i] + j) = new_el;
		}
	}

	return OK;
}


/**
 * Функция, вычисляющая сумму двух матриц. Размеры итоговой матрицы присваиваются ей внутри функции.
 * @param first - указатель на матричную переменную первой матрицы
 * @param sec - указатель на матричную переменную второй матрицы
 * @param res - указатель на матричную переменную матрицы, получившейся в результате сложения первых двух матриц
 * @return 0, если все хорошо, иначе ненулевое число
 */
int add_matrix(matrix_t *first, matrix_t *sec, matrix_t *res)
{
	assert(first && sec && res);

	if (first->rows != sec->rows || first->cols != sec->cols)
		return CANT_BE_ADD;

	res->rows = first->rows;
	res->cols = first->cols;

	if (matrix_alloc(res))
		return MEMORY_ALLOC_ERROR;

	for (size_t i = 0; i < first->rows; ++i)
	{
		for (size_t j = 0; j < first->cols; ++j)
			*(res->matrix[i] + j) = *(first->matrix[i] + j) + *(sec->matrix[i] + j);
	}

	return OK;
}


/**
 * Функция сохранения матрицы в файл.
 * @param filename - строка-имя файла.
 * @param matrix - указатель на матричную переменную той матрицы, которую нужно сохранить.
 * @return 0, если ошибок не было, иначе ненулевое значение.
 */
int save_matrix(const char *filename, matrix_t *matrix)
{
	assert(filename && matrix);

	FILE *file = NULL;
	if (!(file = fopen(filename, "w")))
		return FILE_ERROR;

	// Подсчитываем количество ненулевых элементов
	size_t total_non_zero_els = 0;
	for (size_t i = 0; i < matrix->rows; ++i)
	{
		for (size_t j = 0; j < matrix->cols; ++j)
			if (*(matrix->matrix[i] + j) != 0)
				++total_non_zero_els;
	}

	// Записываем матрицу в файл в координатном методе
	fprintf(file, "%zu %zu %zu\n", matrix->rows, matrix->cols, total_non_zero_els);
	for (size_t i = 0; i < matrix->rows; ++i)
		for (size_t j = 0; j < matrix->cols; ++j)
			if (*(matrix->matrix[i] + j) != 0)
				fprintf(file, "%zu %zu %d\n", i + 1, j + 1, *(matrix->matrix[i] + j));

	fclose(file);

	return OK;
}
