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


submatrices = []
kdim = 6

for y in range(matrix.shape[0] - kdim + 1):
    for x in range(matrix.shape[1] - kdim + 1):
        submatrix = matrix[y: y + kdim, x: x + kdim]
        submatrices.append(submatrix.flatten())

X = np.array(submatrices) / 1.0

kmeans = KMeans(n_clusters=2, random_state=0).fit(X)

# Identify the submatrices that belong to each cluster
cluster_labels = kmeans.labels_
cluster_0_indices = np.where(cluster_labels == 0)[0]
cluster_1_indices = np.where(cluster_labels == 1)[0]

# Calculate the centroids of the submatrices in each cluster
centroid_0 = np.mean(X[cluster_0_indices], axis=0)
centroid_1 = np.mean(X[cluster_1_indices], axis=0)

# Convert the centroids back to the original matrix format
centroid_0_matrix = centroid_0.reshape((kdim, kdim))
centroid_1_matrix = centroid_1.reshape((kdim, kdim))


# ...
