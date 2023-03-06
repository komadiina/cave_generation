from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans

# Load the test image
bmp_image = Image.open('cave.bmp')
matrix = np.array(bmp_image)

if matrix.ndim == 3:
    matrix = matrix[:, :, 0]

matrix = matrix.astype(int)

for i in range(matrix.shape[0]):
    for j in range(matrix.shape[1]):
        matrix[i, j] = not matrix[i, j]
        if matrix[i, j] > 0:
            matrix[i, j] = 1

# Perfom clustering using a kernel matrix' entropy
kdim = 4
kernel = np.zeros(shape=(kdim, kdim))

for y in range(0, matrix.shape[0], kdim):
    for x in range(0, matrix.shape[1], kdim):
        kernel = matrix[y:y + kdim, x:x + kdim]
        kernel_entropy = np.sum(kernel)
        # print(f'(x, y) = ({x}, {y}), entropy = {kernel_entropy}')

        if kernel_entropy < (kdim**2) // 2:
            kernel.fill(0)
            matrix[y:y + kdim, x: x + kdim] = kernel[:, :]


# Plot our cave system
plt.imshow(matrix, cmap='binary', interpolation='nearest')
plt.show()
