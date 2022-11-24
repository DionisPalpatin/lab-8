#include <stdlib.h>
#include <string.h>

#include <file.h>
#include <matrix.h>
#include <errors.h>


/**
 * Программа считывает из текстового файла числа, записывает их в массив, при необходимости создает из него новый,
 * сортирует массив (старый или новый) и записывает его в другой файл.
 * @param argc количество аргументов, переданных программе
 * @param argv аргументы, переданные программе
 * @return код ошибки
 */
int main(int argc, char **argv)
{
	int exit_code = OK;

	exit_code = check_arguments(argc, argv);
	if (exit_code)
		return exit_code;

	if (argc == 4)
	{
		matrix_t src_matrix = { .matrix = NULL};
		double det;

		exit_code = creat_matrix(&src_matrix, argv[2]);
		if (!exit_code)
			exit_code = determinant(&src_matrix, &det);
		if (!exit_code)
			exit_code = save_num_fo_file(argv[3], det);

		free_matrix(src_matrix.matrix, src_matrix.rows);
	}
	else
	{
		matrix_t src_matrix_1 = { .matrix = NULL};
		matrix_t src_matrix_2 = { .matrix = NULL};
		matrix_t res_matrix = { .matrix = NULL};

		exit_code = creat_matrix(&src_matrix_1, argv[2]);

		if (!exit_code)
			exit_code = creat_matrix(&src_matrix_2, argv[3]);

		if (!exit_code)
		{
			if (strcmp(argv[1], "a") == 0)
				exit_code = add_matrix(&src_matrix_1, &src_matrix_2, &res_matrix);
			else
				exit_code = multiply_matrix(&src_matrix_1, &src_matrix_2, &res_matrix);
		}

		if (!exit_code)
			exit_code = save_matrix(argv[4], &res_matrix);

		free_matrix(src_matrix_1.matrix, src_matrix_1.rows);
		free_matrix(src_matrix_2.matrix, src_matrix_2.rows);
		free_matrix(res_matrix.matrix, res_matrix.rows);
	}

	return exit_code;
}


