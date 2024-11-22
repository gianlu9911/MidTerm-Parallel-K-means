#include "SoA.h"
#include <chrono>

int main() {
    try {
        std::vector<size_t> numPointsList = {10000, 10000000, 100000000};
        std::vector<int> clusters = {50,100}; 
        std::vector<int> maxIterationsList = {50,100}; 
        std::vector<int> numThreadsList = {1,4,8, 10, 12, 14, 16, 18, 20}; 

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

                    std::cout << "Sequential execution time for " << numPoints << " points, "<< k << " clusters, and " << maxIterations << " max iterations: "<< durationSeq.count() << " seconds" << std::endl;
                    saveExecutionTimeToCSV("../data/execution_times_SoA.csv", numPoints, k, maxIterations, durationSeq.count(), 0);

                    for (auto numThreads : numThreadsList) {

                        auto startPar = std::chrono::high_resolution_clock::now();

                        kmeansSoA_parallel(points, numPoints, k, maxIterations, numThreads);

                        auto endPar = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double> durationPar = endPar - startPar;

                        std::cout << "Parallel execution time with " << numThreads << " threads for "<< numPoints << " points, " << k << " clusters, and "<< maxIterations << " max iterations: " << durationPar.count() << " seconds" << std::endl;
                        saveExecutionTimeToCSV("../data/execution_times_SoA.csv", numPoints, k, maxIterations, durationPar.count(), numThreads);
                    }
                }
            }
        }

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
