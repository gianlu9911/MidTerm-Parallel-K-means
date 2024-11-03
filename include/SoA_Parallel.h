#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <random>
#include <omp.h>
#include <chrono>
#include "Utility.h"

float execute_SoA_parallel (int num_points, int num_clusters, int maxIteration, float epsilon, float max_value, int num_threads) {

    std::string output_points = "../data/output_parallel.csv";
    std::string output_centroids = "../data/centroids_parallel.csv";
    
    auto start_time = std::chrono::high_resolution_clock::now();

    // Set the number of threads
    omp_set_num_threads(num_threads);

    int numPoints = num_points;
    int numClusters = num_clusters;
    float min = -100.0f, max = 100.0f;

    Points points;
    Clusters clusters;

    // Resize Points and Clusters data
    points.x.resize(numPoints);
    points.y.resize(numPoints);
    points.cluster_id.resize(numPoints, -1);
    points.minDistance.resize(numPoints, std::numeric_limits<float>::max());

    clusters.x.resize(numClusters);
    clusters.y.resize(numClusters);
    clusters.cum_sum_x.resize(numClusters, 0.0f);
    clusters.cum_sum_y.resize(numClusters, 0.0f);
    clusters.size.resize(numClusters, 0);

    #pragma omp parallel
    {
        std::random_device rd;
        std::mt19937 gen(rd() + omp_get_thread_num());
        std::uniform_real_distribution<float> dis(min, max);

        // Random point initialization
        #pragma omp for
        for (size_t i = 0; i < numPoints; ++i) {
            points.x[i] = dis(gen);
            points.y[i] = dis(gen);
            points.cluster_id[i] = -1;
        }

        // Random centroid initialization
        #pragma omp for
        for (int i = 0; i < numClusters; ++i) {
            clusters.x[i] = dis(gen);
            clusters.y[i] = dis(gen);
            clusters.size[i] = 0;

            }
    }

    // Compute points-centroid distance -> Assign points to nearest cluster
    #pragma omp parallel for schedule(dynamic, 100)
    for (size_t point = 0; point < numPoints; ++point) {
        float px = points.x[point];
        float py = points.y[point];
        float minDist = points.minDistance[point];
        int nearestCluster = points.cluster_id[point];

        for (size_t cluster = 0; cluster < numClusters; ++cluster) {
            float dx = px - clusters.x[cluster];
            float dy = py - clusters.y[cluster];
            float squared_distance = dx * dx + dy * dy; // No sqrt(), compare squared distances

            if (squared_distance < minDist) {
                minDist = squared_distance;
                nearestCluster = cluster;
            }
        }
        points.minDistance[point] = minDist;
        points.cluster_id[point] = nearestCluster;
    }

    for (size_t cluster = 0; cluster < numClusters; ++cluster) {
        clusters.size[cluster] = 0;
    }

    // Update clusters -> COMPUTE CUMULATIVE SUMS
    #pragma omp parallel
    {
        std::vector<float> local_cum_sum_x(numClusters, 0.0f);
        std::vector<float> local_cum_sum_y(numClusters, 0.0f);
        std::vector<int> local_size(numClusters, 0);

        #pragma omp for
        for (size_t point = 0; point < numPoints; ++point) {
            int cluster = points.cluster_id[point];
            local_cum_sum_x[cluster] += points.x[point];
            local_cum_sum_y[cluster] += points.y[point];
            local_size[cluster]++;
        }

        // Use atomic operations
        for (size_t cluster = 0; cluster < numClusters; ++cluster) {
            #pragma omp atomic
            clusters.cum_sum_x[cluster] += local_cum_sum_x[cluster];

            #pragma omp atomic
            clusters.cum_sum_y[cluster] += local_cum_sum_y[cluster];

            #pragma omp atomic
            clusters.size[cluster] += local_size[cluster];
        }
    }

    // Compute new centroids
    #pragma omp parallel for
    for (size_t cluster = 0; cluster < numClusters; ++cluster) {
        if (clusters.size[cluster] > 0) {
            clusters.x[cluster] = clusters.cum_sum_x[cluster] / clusters.size[cluster];
            clusters.y[cluster] = clusters.cum_sum_y[cluster] / clusters.size[cluster];
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> execution_time = end_time - start_time;
    std::cout << "Execution time: " << execution_time.count() << " seconds." << std::endl;

    savePointsToCSV(points.x, points.y, points.cluster_id, output_points);
    savePointsToCSV(clusters.x, clusters.y, output_centroids);

    return (float)execution_time.count();
}
