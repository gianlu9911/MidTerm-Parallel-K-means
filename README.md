# MidTerm-Parallel-K-means
MidTerm examp for parallel computing: Implementing a sequential and parallel version of Kmenas algorithm in C++, parallel version is done in OpenMP.


# K-Means Algorithm

This task aim to partition n points into k clusters in which each point belongs to the cluster with the closer mean. It is based on the following steps:

1. Randomly generate N points
2. Randomly generate K clusters
3. For every point, find the distance from the nearest cluster and assign the point the cluster.
4. Update the centroid's coordinates.

# Implementations
### 1. Sequential Version
The main steps include initialization, assignment of data points to the nearest centroid, updating centroids, and iterative clustering until convergence. Here we have 2 different versions: Aos and SoA.

# Using Array of Structures (AoS)
The points are organized in an array of structures. In the parallelized version multiple threads work on different points simultaneously to achieve parallel processing.

# Using Structure of Arrays (SoA)
Points are organized as structure of vectors. This can provide better performance in certain scenarios, especially for large datasets and optimized memory access. A parallelized version with OpenMP is implemented too.

# SoA.h and AoS.h

In these files there is the real implementation of the algorithms in both version: As array of Structure and Structure of Array.

# Utility.h
Header with Point and CLuster struct definition and usefull function used in SoA implementation like:
1. Function to save the points in a csv file to plot them
2. Function to save the clusters' centroids in a csv file, to plot them 

Points coordinated are defined as vectors of float. The cluster assignment of the points are described as vector of int.

# ClusterPoint.h
Here I define Point and Cluster classes. Point has x and y coordinated as float. The assignment to the cluster is done as an integer.

Cluster class has float coordinates of centroids, cumulative sums as floats and size as integer.

In this file there is even function to save points and cluster informations in a csv in order to plot them later na dto generate random points and centroids.

# Plot_Points.py

Python file to plot points and centroids described in data/points.csv and data/centroids.csv .

# Plot_execution_time.py

Python file to plot execution time of experments done in main.cpp and main2.cpp .
