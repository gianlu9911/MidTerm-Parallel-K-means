#include "AoS.h"
#include <chrono>
#include <vector>
#include <iostream>

int main() {
    const std::string dataset = "../data/dataset.csv";

    std::vector<size_t> numPointsList = {1000};  // , 100000000
    std::vector<int> clusters = {5};  
    std::vector<int> maxIterationsList = {50};  
    std::vector<int> numThreadsList = {12};  

    for (size_t num_points : numPointsList) {
        for (int num_clusters : clusters) {
            for (int max_iterations : maxIterationsList) {
                std::vector<Point> points;
                readPointsFromCSV(dataset, points, num_points);

                auto start = std::chrono::high_resolution_clock::now();
                kMeansAoS_seq(points, num_clusters, max_iterations);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = end - start;

                saveExecutionTimesToCSV("../data/execution_times_AoS.csv", 
                                         num_points, num_clusters, max_iterations, elapsed.count(), 0);

                std::cout << "Sequential -> Points: " << num_points
                          << ", Clusters: " << num_clusters
                          << ", Max Iterations: " << max_iterations
                          << " -> Execution Time: " << elapsed.count() << " seconds\n";

                for (int num_threads : numThreadsList) {
                    start = std::chrono::high_resolution_clock::now();
                    kMeansAoS_parallel(points, num_clusters, max_iterations, num_threads,true);
                    end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed_parallel = end - start;

                    saveExecutionTimesToCSV("../data/execution_times_AoS.csv", 
                                             num_points, num_clusters, max_iterations, elapsed_parallel.count(), num_threads);

                    std::cout << "Parallel (Threads: " << num_threads << ") -> Points: " << num_points
                              << ", Clusters: " << num_clusters
                              << ", Max Iterations: " << max_iterations
                              << " -> Execution Time: " << elapsed_parallel.count() << " seconds\n";
                }
            }
        }
    }

    return 0;
}
