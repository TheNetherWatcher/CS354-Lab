# The following code implements the fuzzy c-means clustering algorithm.
# Fuzzy C-Means is a soft clustering algorithm where each data point can belong to multiple clusters with varying degrees (membership values).
# The function fuzzy_c_means performs the clustering by iteratively updating the membership values and cluster centers until convergence.

def fuzzy_c_means(X, Y, C1, C2, m=2, max_iter=100, tol=1e-5):
    """
    Fuzzy C-Means clustering algorithm.
    
    This algorithm clusters the data points (X, Y) into two clusters, with each point having a membership value for both clusters (C1 and C2).
    The clustering process is based on minimizing the distance between each point and the cluster centers, with a fuzziness parameter (m)
    controlling the degree of membership. The algorithm converges when the membership values stop changing significantly.

    Parameters:
    X : list - List of x-coordinates of the data points.
    Y : list - List of y-coordinates of the data points.
    C1 : list - List of initial membership values for cluster 1.
    C2 : list - List of initial membership values for cluster 2.
    m : int - Fuzziness parameter (default is 2).
    max_iter : int - Maximum number of iterations (default is 100).
    tol : float - Tolerance for convergence (default is 1e-5).
    
    Returns:
    C1 : list - Final membership values for cluster 1 after convergence.
    C2 : list - Final membership values for cluster 2 after convergence.
    """
    
    # Initialize cluster centers based on weighted averages of the data points, weighted by the initial membership values.
    C1_center = [
        sum(x * c**m for x, c in zip(X, C1)) / sum(c**m for c in C1),
        sum(y * c**m for y, c in zip(Y, C1)) / sum(c**m for c in C1)
    ]
    C2_center = [
        sum(x * c**m for x, c in zip(X, C2)) / sum(c**m for c in C2),
        sum(y * c**m for y, c in zip(Y, C2)) / sum(c**m for c in C2)
    ]
    
    # Iterate through the maximum number of iterations or until convergence.
    for _ in range(max_iter):
        # Update membership values for each data point based on its distance to the current cluster centers.
        new_C1 = []
        new_C2 = []
        
        for i in range(len(X)):
            d1 = ((X[i] - C1_center[0])**2 + (Y[i] - C1_center[1])**2)**0.5  # Distance to cluster 1 center
            d2 = ((X[i] - C2_center[0])**2 + (Y[i] - C2_center[1])**2)**0.5  # Distance to cluster 2 center
            
            # Handle division by zero in case of a perfect match with the center.
            if d1 == 0:
                new_C1.append(1)
                new_C2.append(0)
            elif d2 == 0:
                new_C1.append(0)
                new_C2.append(1)
            else:
                # Calculate the membership values based on the distances (fuzziness parameter m controls the degree of membership).
                c1 = 1 / (1 + (d1/d2)**(2/(m-1)))
                new_C1.append(c1)
                new_C2.append(1 - c1)
        
        # Check for convergence: if the membership values don't change significantly, stop the algorithm.
        if all(abs(nc1 - c1) < tol and abs(nc2 - c2) < tol 
              for nc1, c1, nc2, c2 in zip(new_C1, C1, new_C2, C2)):
            break
        
        C1 = new_C1
        C2 = new_C2
        
        # Update the cluster centers based on the new membership values.
        C1_center = [
            sum(x * c**m for x, c in zip(X, C1)) / sum(c**m for c in C1),
            sum(y * c**m for y, c in zip(Y, C1)) / sum(c**m for c in C1)
        ]
        C2_center = [
            sum(x * c**m for x, c in zip(X, C2)) / sum(c**m for c in C2),
            sum(y * c**m for y, c in zip(Y, C2)) / sum(c**m for c in C2)
        ]
    
    # Return the final membership values for both clusters.
    return C1, C2

# Test data points and initial membership values
X = [1, 2, 3, 4, 5, 6]
Y = [6, 5, 8, 4, 7, 9]
C1 = [0.8, 0.9, 0.7, 0.3, 0.5, 0.2]
C2 = [0.2, 0.1, 0.3, 0.7, 0.5, 0.8]

# Run the fuzzy c-means algorithm
C1, C2 = fuzzy_c_means(X, Y, C1, C2)

# Print the final membership values for each cluster
print("Final membership values for cluster 1:", C1)
print("Final membership values for cluster 2:", C2)
