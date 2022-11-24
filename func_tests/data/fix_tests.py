for i in range(1, 7):
    matrix = []
    with open(f"pos_0{i}_out.txt", "r") as f:
        for line in f:
            matrix.append([int(el) for el in line.split()])

    non_zeros = 0
    for row in matrix:
        for el in row:
            if el != 0:
                non_zeros += 1

    # for row in matrix:
    #     print(*row)

    with open(f"pos_0{i}_out.txt", "w") as f:
        string = f"{len(matrix)} {len(matrix[0])} {non_zeros}\n"
        f.write(string)

        for i in range(len(matrix)):
            for j in range(len(matrix[0])):
                if matrix[i][j] != 0:
                    string = f"{i + 1} {j + 1} {matrix[i][j]}\n"
                    f.write(string)