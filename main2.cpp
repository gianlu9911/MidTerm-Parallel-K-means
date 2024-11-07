#include "AoS.h"
#include <iostream>
#include <fstream>
#include <vector>

// Assuming execute_AoS is defined elsewhere

void runExperimentsAndSaveToCSV(const std::string& filename) {
    std::ofstream csv_file(filename);
    
    // Write the CSV header
    csv_file << "num_points,num_clusters,maxIteration,epsilon,num_threads,execution_time\n";

    // Experiment parameters
    std::vector<int> points_values = {1000, 10000, 100000};
    std::vector<int> threads_values = {1, 2, 4, 8};
    std::vector<int> max_iter_values = {10, 50, 100};
    int num_clusters = 5;  // Example fixed cluster count
    float epsilon = 0.001; // Example fixed convergence threshold

    for (int points : points_values) {
        for (int threads : threads_values) {
            for (int max_iter : max_iter_values) {
                // Run the K-Means function and measure execution time
                float exec_time = execute_AoS(points, num_clusters, max_iter, epsilon, threads, false);

                // Save the result to the CSV file
                csv_file << points << "," << num_clusters << "," << max_iter << "," 
                         << epsilon << "," << threads << "," << exec_time << "\n";

                std::cout << "Saved result - Points: " << points << ", Threads: " << threads
                          << ", MaxIter: " << max_iter << ", Execution Time: " << exec_time << "s\n";
            }
        }
    }

    // Close the CSV file
    csv_file.close();
    std::cout << "All results have been saved to " << filename << std::endl;
}

int main() {
    runExperimentsAndSaveToCSV("../data/execution_times.csv");
    return 0;
}
