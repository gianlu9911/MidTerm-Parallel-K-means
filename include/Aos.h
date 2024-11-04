#pragma once

#include "Cluster Point.h"
#include <vector>
#include <omp.h>
#include <chrono>
#include <iostream>

float execute_AOS(int num_points, int num_clusters, int maxIteration, float epsilon, int num_threads, bool saving = false) {
    // Generate points and clusters
    std::vector<Point> points = generatePoints(num_points, num_clusters, num_threads);
    std::vector<Cluster> clusters = generateClusters(num_clusters, num_threads);

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Start timing
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int iteration = 0; iteration < maxIteration; iteration++) {
        bool converged = true;

        // Assign points to the nearest cluster
        #pragma omp parallel for
        for (int i = 0; i < num_points; i++) {
            float min_distance = std::numeric_limits<float>::max();
            int best_cluster = -1;

            for (int j = 0; j < num_clusters; j++) {
                float distance = points[i].getDistance(clusters[j].getX(), clusters[j].getY());
                if (distance < min_distance) {
                    min_distance = distance;
                    best_cluster = j;
                }
            }

            if (points[i].getAssign() != best_cluster) {
                points[i].setAssign(best_cluster);
                converged = false;
            }
        }

        if (converged) {
            break;
        }

        // Add points to their assigned clusters
        #pragma omp parallel for
        for (int i = 0; i < num_points; i++) {
            int assign = points[i].getAssign();
            clusters[assign].addPoint(points[i]);
        }

        // Update centroids
        #pragma omp parallel for
        for (int j = 0; j < num_clusters; j++) {
            clusters[j].updateCentroid(epsilon);
        }
    }

    // End timing
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end_time - start_time;
    float execution_time = duration.count();
    std::cout << "Execution time: " << execution_time << " seconds." << std::endl;

    if (saving) {
        savePointsToCSV(points, "../data/points.csv");
        saveCentroidsToCSV(clusters, "../data/centroids.csv");
    }

    return execution_time;
}
