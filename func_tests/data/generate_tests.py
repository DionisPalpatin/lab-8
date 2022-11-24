import _io
from random import randint
from typing import Optional


class File:
    def __init__(self, filename: str, mode: str):
        self.file: Optional[_io.TextIOWrapper] = None

        self._open(filename, mode)

    def __del__(self):
        self.file.close()

    def _open(self, filename: str, mode: str):
        self.file = open(filename, mode)


# Класс матрицы. Создается матрица автоматически при объявлении переменной
class SrcMatrix:
    def __init__(self, rows: int, cols: int):
        self.data: Optional[list[[int]]] = None
        self.rows: Optional[int] = rows
        self.cols: Optional[int] = cols
        self.total_non_zeros: Optional[int] = 0

        self._gen_mtr()
        self._fill_mtr()
        self._count_non_zero_els()

    def _gen_mtr(self):
        self.data = [[randint(-100, 100) for _ in range(self.cols)] for _ in range(self.rows)]

    def _fill_mtr(self):
        new_zero_quan = randint(0, self.rows * self.cols)
        for i in range(new_zero_quan):
            i_zero, j_zero = randint(0, self.rows - 1), randint(0, self.cols - 1)
            if self.data[i_zero][j_zero]:
                self.data[i_zero][j_zero] = 0

    def _count_non_zero_els(self):
        for i in range(self.rows):
            for j in range(self.cols):
                if self.data[i][j] != 0:
                    self.total_non_zeros += 1

    def write_mtr_to_file(self, file: File):
        string = f"{self.rows} {self.cols} {self.total_non_zeros}\n"
        file.file.write(string)

        for i in range(self.rows):
            for j in range(self.cols):
                if self.data[i][j] != 0:
                    string = f"{i + 1} {j + 1} {self.data[i][j]}\n"
                    file.file.write(string)


class Submatrix:
    def __init__(self, main_matrix: SrcMatrix, excluded_col: int):
        self.data: Optional[list[[int]]] = list()
        self.rows: Optional[int] = main_matrix.rows - 1
        self.cols: Optional[int] = main_matrix.cols - 1

        self._creat_submtr(main_matrix, excluded_col)

    def _creat_submtr(self, main_matrix: SrcMatrix, excluded_col: int):
        for i in range(1, main_matrix.rows):
            self.data.append(main_matrix.data[i][:excluded_col] + main_matrix.data[i][excluded_col + 1:])


class ResMatrix:
    def __init__(self):
        self.data: Optional[list[[int]]] = list()
        self.rows: Optional[int] = 0
        self.cols: Optional[int] = 0
        self.total_non_zeros: Optional[int] = 0
        self.determinant: Optional[int] = None

    def add(self, mtr1: SrcMatrix, mtr2: SrcMatrix):
        self.data = [[mtr1.data[i][j] + mtr2.data[i][j] for j in range(mtr1.cols)] for i in range(mtr1.rows)]

    def mult(self, mtr1: SrcMatrix, mtr2: SrcMatrix):
        for i in range(mtr1.rows):
            new_res_mtr_string = []
            for j in range(mtr2.cols):
                new_el = 0
                for k in range(mtr1.cols):
                    new_el += mtr1.data[i][k] * mtr2.data[k][j]
                new_res_mtr_string.append(new_el)
            self.data.append(new_res_mtr_string)

    def det(self, mtr: SrcMatrix):
        ans = 0
        if mtr.rows == 1:
            ans = mtr.data[0][0]
        elif mtr.rows == 2:
            ans = mtr.data[0][0] * mtr.data[1][1] - mtr.data[0][1] * mtr.data[1][0]
        elif mtr.rows == 3:
            ans = mtr.data[0][0] * mtr.data[1][1] * mtr.data[2][2] + \
                  mtr.data[0][1] * mtr.data[1][2] * mtr.data[2][0] + \
                  mtr.data[1][0] * mtr.data[2][1] * mtr.data[0][2] - \
                  mtr.data[0][2] * mtr.data[1][1] * mtr.data[2][0] - \
                  mtr.data[1][0] * mtr.data[0][1] * mtr.data[2][2] - \
                  mtr.data[0][0] * mtr.data[1][2] * mtr.data[2][1]
        else:
            k = 1
            for j in range(0, mtr.rows):
                submatrix = Submatrix(mtr, j)
                for i in range(1, mtr.rows):
                    submatrix.data.append(mtr.data[i][:j] + mtr.data[i][j + 1:])
                ans += k * mtr.data[0][j] * self.det(submatrix.data)
                k *= -1

        self.determinant = ans


tests_info = [
    [(5, 5), (5, 5), "add"],
    [(4, 5), (4, 5), "add"],
    [(1, 1), (1, 1), "add"],
    [(5, 5), (5, 5), "mul"],
    [(4, 5), (5, 4), "mul"],
    [(1, 1), (1, 1), "mul"],
    [(5, 5), "det"],
    [(3, 3), "det"],
    [(2, 2), "det"],
    [(1, 1), "det"]
]

for i in range(len(tests_info)):
    file_num = f"0{i + 1}" if i < 9 else f"{i + 1}"
    test_type = tests_info[i][-1]

    if test_type == "add" or test_type == "mul":
        with open(f"pos_{file_num}_args.txt", "w") as af:
            af.write(f"{test_type[0]} ./func_tests/data/pos_{file_num}_in1.txt" + \
                     f" ./func_tests/data/pos_{file_num}_in2.txt ./out/out.txt")

        mtr1 = gen_mtr(tests_info[i][0])
        total_zeros = fill_mtr(mtr1)
        with open(f"pos_{file_num}_in1.txt", "w") as f1:
            write_mtr_to_file_another_format(mtr1, total_zeros, f1)

        mtr2 = gen_mtr(tests_info[i][1])
        total_zeros = fill_mtr(mtr2)
        with open(f"pos_{file_num}_in2.txt", "w") as f2:
            write_mtr_to_file_another_format(mtr2, total_zeros, f2)

        with open(f"pos_{file_num}_out.txt", "w") as rf:
            res = list()
            if test_type == "add":
                res = add_matrix(mtr1, mtr2)
            else:
                res = mult_matrix(mtr1, mtr2)

            write_mtr_to_file(res, rf)
    else:
        with open(f"pos_{file_num}_args.txt", "w") as af:
            af.write(f"o ./func_tests/data/pos_{file_num}_in.txt ./out/out.txt")

        mtr1 = gen_mtr(tests_info[i][0])
        total_zeros = fill_mtr(mtr1)
        with open(f"pos_{file_num}_in.txt", "w") as f1:
            write_mtr_to_file_another_format(mtr1, total_zeros, f1)

        with open(f"pos_{file_num}_out.txt", "w") as rf:
            res = det(mtr1)
            rf.write("{}\n".format(res))
