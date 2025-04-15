import numpy as np
from sklearn.datasets import load_wine
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.metrics import adjusted_rand_score
import matplotlib.pyplot as plt

# Load and preprocess the wine dataset
data = load_wine()
X_raw = data.data
y = data.target

# Standardize the data
scaler = StandardScaler()
X = scaler.fit_transform(X_raw)

# Parameters for Fuzzy C-Means
k = 5
m = 1.7
max_iter = 100
tol = 1e-5

# Fuzzy C-Means clustering function
def fuzzy_c_means(X, k, m, initial_centers, max_iter=100, tol=1e-5):
    n, d = X.shape
    centers = initial_centers.copy()
    U = np.zeros((n, k))

    # Initialize membership matrix U based on initial centers
    for i in range(n):
        distances = np.linalg.norm(X[i] - centers, axis=1)
        distances = np.clip(distances, 1e-10, None)  # Prevent division by zero
        U[i] = (distances**(-2 / (m - 1))) / np.sum(distances**(-2 / (m - 1)))

    # Iterate until convergence or max_iter
    for _ in range(max_iter):
        # Update centers
        new_centers = np.array([np.dot(U[:, j]**m, X) / np.sum(U[:, j]**m) for j in range(k)])

        # Update membership matrix U
        new_U = np.zeros((n, k))
        for i in range(n):
            distances = np.linalg.norm(X[i] - new_centers, axis=1)
            distances = np.clip(distances, 1e-10, None)  # Prevent division by zero
            new_U[i] = (distances**(-2 / (m - 1))) / np.sum(distances**(-2 / (m - 1)))

        # Check for convergence
        if np.max(np.abs(new_U - U)) < tol:
            break
        U, centers = new_U.copy(), new_centers.copy()

    return U, centers

# PCA for 2D visualization
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X)

# Initialize cluster centers using different methods
np.random.seed(42)
initializations = [
    ('Origin', np.zeros((k, X.shape[1]))),
    ('Gaussian', np.random.multivariate_normal(np.zeros(X.shape[1]), np.eye(X.shape[1]), k)),
    ('Data Points', X[np.random.choice(X.shape[0], k, replace=False)])
]

# Run FCM for each initialization and compute ARI score
results = []
for name, centers in initializations:
    U, final_centers = fuzzy_c_means(X, k, m, centers, max_iter, tol)
    clusters = np.argmax(U, axis=1)
    ari = adjusted_rand_score(y, clusters)
    results.append((name, clusters, final_centers, ari))

# Plot clusters and centers
for name, clusters, centers, ari in results:
    # Check for NaN values in centers
    if np.any(np.isnan(centers)):
        print(f"Warning: NaN values found in centers for initialization: {name}")
        continue
    
    # Perform PCA transformation on centers
    centers_pca = pca.transform(centers)
    plt.figure(figsize=(8, 6))
    plt.scatter(X_pca[:, 0], X_pca[:, 1], c=clusters, cmap='viridis', alpha=0.6)
    plt.scatter(centers_pca[:, 0], centers_pca[:, 1], c='red', marker='X', s=200, label='Centers')
    plt.title(f'Initialization: {name}\nAdjusted Rand Index: {ari:.2f}')
    plt.xlabel('PC1')
    plt.ylabel('PC2')
    plt.legend()
    plt.colorbar(label='Cluster')
    plt.show()

# Print ARI scores for each initialization
print("Adjusted Rand Index Scores:")
for name, _, _, ari in results:
    print(f"{name}: {ari:.3f}")