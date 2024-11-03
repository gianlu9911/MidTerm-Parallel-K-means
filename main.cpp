#include "SoA.h"

int main() {
    std::vector<TimingResult> results;

    int numPoints=10000;
    int numClusters=5;
    int maxIteration=10;
    float epsilon=0.01;
    float max_value=100;
    int num_Thjreads=4;
    
    for (int num_Thjreads = 1; num_Thjreads <= 4; num_Thjreads++) {
        float execution_time =  execute_SoA(numPoints,numClusters,maxIteration,epsilon,max_value,omp_get_max_threads());
    results.push_back({numPoints, num_Thjreads, execution_time});
    std::ofstream file("../data/execution_times.csv");
    file << "num_points,num_threads,execution_time\n";
    for (const auto& result : results) {
        file << result.num_points << "," << result.num_threads << "," << result.execution_time << "\n";
    }
    file.close();
    }
    
    return 0;
}