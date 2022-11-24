#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <errors.h>
#include <matrix.h>
#include <file.h>


/**
 * Обработка переданных в программу параметров
 * @param argc количество переданных параметров
 * @param argv массив указателей на параметры
 * @return код ошибки
 */
int check_arguments(int argc, char **argv)
{
	assert(argv);

	if (argc < 4 || argc > 5)
		return INCORRECT_ARGS_QUAN;
	else if ((argc == 4 && strcmp(argv[1], "o") != 0) || \
		(argc == 5 && strcmp(argv[1], "a") != 0 && strcmp(argv[1], "m") != 0))
		return INCORRECT_ARGUMENT;

	return OK;
}


/**
 * Функция, считывающая из файла одну строку. В строке обязательно должно быть три целых числа.
 * @param file - указатель на файл.
 * @param row_index - переменная, в которую помещается индекс строки, в которую надо записать элемент.
 * @param max_row_index - максимальное значение индекса строки, которое может быть.
 * @param col_index - переменная, в которую помещается индекс столбца, в который надо записать элемент.
 * @param max_col_index - максимальное значение индекса столбца, которое может быть.
 * @param value - значение записываемого элемента.
 * @return 0, если ошибок не было, иначе ненулевое значение.
 */
int read_string(FILE *file, size_t *row_index, size_t max_row_index, size_t *col_index, size_t max_col_index, int *value)
{
	assert(file && row_index && col_index && value);

	if (fscanf(file, "%zu %zu %d", row_index, col_index, value) != 3)
		return STR_READ_ERROR;
	else if (*row_index <= 0 || *row_index > max_row_index || *col_index <= 0 || *col_index > max_col_index)
		return INDEX_VALUE_ERROR;

	return OK;
}


/**
 * Функция сохранения числа в файл.
 * @param filename - имя файла, в который надо сохранить число.
 * @param number - число, которое надо сохранить.
 * @return 0, если ошибок не было, иначе не нулевое значение.
 */
int save_num_fo_file(const char *filename, double number)
{
	assert(filename);

	FILE *out = fopen(filename, "w");
	if (!out)
		return FILE_ERROR;

	// Так как матрицы целочисленные, результат их сложения, умножения будет состоять из целых чисел.
	// Детерминант тоже будет целым числом.
	// Во избежание переполнения в коде используются числа типа double, но результат все равно выводится как int.
	fprintf(out, "%.0f\n", number);
	fclose(out);

	return OK;
}
