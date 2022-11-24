from random import randint

matrix = [[randint(-100, 100) for _ in range(4)] for _ in range(5)]


total_zeros = randint(10, 18)
for i in range(total_zeros):
    i_zero, j_zero = randint(0, 4), randint(0, 3)
    if matrix[i_zero][j_zero]:
        matrix[i_zero][j_zero] = 0


print(f"5 4 {20 - total_zeros}")
for i in range(5):
    for j in range(4):
        if matrix[i][j]:
            print(f"{i + 1} {j + 1} {matrix[i][j]}")


print("\n\n\n")


for row in matrix:
    print(*row, sep=' ', end='\n')
