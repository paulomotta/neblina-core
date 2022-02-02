import numpy as np

a = np.array([[1+1j, 2+2j],
              [3+3j, 4+4j]])

b = np.array([[5+5j, 6+6j],
              [7+7j, 8+8j]])

A = np.array([[17.+1.j, -3.+0.j],
              [-7.+1.j,  1.+0.j]])

B = np.array([[ 60.+0.j,  -4.+1.j],
              [-12.+0.j,   0.+1.j]])

print(np.matmul(A, B))
