m = matrix.create(3, 3)
n = matrix.create(2, 2, 1)

matrix.set(m, 1, 1, 44)
print(matrix.get(m, 1, 1))
matrix.setElements(n, 2, 3, 4, 66, 4)

print(matrix.toString(m))
print(matrix.toString(n))

matrix.clear()

io.read()