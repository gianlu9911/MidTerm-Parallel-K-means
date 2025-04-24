#include "SoA.h"
#include "AoS.h"
#include <chrono>

int main() {
    try {
        std::vector<size_t> numPointsList = { 100000}; // 100000000
        std::vector<int> clusters = {50,100}; 
        std::vector<int> maxIterationsList = {50, 100}; 
        std::vector<int> numThreadsList = {1,2,4,8,12,16,20,22,24,30}; 

        for (auto numPoints : numPointsList) {
            for (auto k : clusters) {
                for (auto maxIterations : maxIterationsList) {
                    std::string filename = "../data/dataset.csv"; 

                    // READ CSV!
                    PointsSoA points = readCSVToSoA(filename, numPoints);

                    auto startSeq = std::chrono::high_resolution_clock::now();

                    kmeansSoA_seq(points, numPoints, k, maxIterations);

                    auto endSeq = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> durationSeq = endSeq - startSeq;

                    std::cout << "SoA Sequential execution time for " << numPoints << " points, "<< k << " clusters, and " << maxIterations << " max iterations: "<< durationSeq.count() << " seconds" << std::endl;
                    saveExecutionTimeToCSV("../data/execution_times_SoA.csv", numPoints, k, maxIterations, durationSeq.count(), 0);

                    for (auto numThreads : numThreadsList) {

                        auto startPar = std::chrono::high_resolution_clock::now();

                        kmeansSoA_parallel(points, numPoints, k, maxIterations, numThreads, true);

                        auto endPar = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double> durationPar = endPar - startPar;

                        std::cout << " SoA Parallel execution time with " << numThreads << " threads for "<< numPoints << " points, " << k << " clusters, and "<< maxIterations << " max iterations: " << durationPar.count() << " seconds" << std::endl;
                        saveExecutionTimeToCSV("../data/execution_times_SoA.csv", numPoints, k, maxIterations, durationPar.count(), numThreads);
                    }
                }
            }
        }





        // AoS
        const std::string dataset = "../data/dataset.csv";
        for (size_t num_points : numPointsList) {
            for (int num_clusters : clusters) {
                for (int max_iterations : maxIterationsList) {
    
                    // Read CSV data into a vector of Points (AoS)
                    std::vector<Point> points;
                    readPointsFromCSV(dataset, points, num_points);
    
                    // SEQUENTIAL EXECUTION
                    auto start = std::chrono::high_resolution_clock::now();
                    kMeansAoS_seq(points, num_clusters, max_iterations);
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end - start;
                    
                    // Save and print sequential execution time
                    saveExecutionTimesToCSV("../data/execution_times_AoS.csv", 
                                              num_points, num_clusters, max_iterations, 
                                              elapsed.count(), 0);
                    std::cout << "Sequential -> Points: " << num_points
                              << ", Clusters: " << num_clusters
                              << ", Max Iterations: " << max_iterations
                              << " -> Execution Time: " << elapsed.count() << " seconds\n";
    
                    // PARALLEL EXECUTION for different thread counts
                    for (int num_threads : numThreadsList) {
                        // (Optional) Re-read the CSV to restore the original state,
                        // if kMeansAoS_parallel modifies the data in-place.
                        // Uncomment the next two lines if necessary:
                        // points.clear();
                        // readPointsFromCSV(dataset, points, num_points);
    
                        start = std::chrono::high_resolution_clock::now();
                        kMeansAoS_parallel(points, num_clusters, max_iterations, num_threads, true);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double> elapsed_parallel = end - start;
                        
                        // Save and print parallel execution time
                        saveExecutionTimesToCSV("../data/execution_times_AoS.csv", 
                                                  num_points, num_clusters, max_iterations, 
                                                  elapsed_parallel.count(), num_threads);
                        std::cout << "Parallel (Threads: " << num_threads << ") -> Points: " << num_points
                                  << ", Clusters: " << num_clusters
                                  << ", Max Iterations: " << max_iterations
                                  << " -> Execution Time: " << elapsed_parallel.count() << " seconds\n";
                    }
                }
            }
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
