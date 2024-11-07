#include "SoA.h"

void SoA_varing_num_threads(int max_num_threads){
    std::vector<TimingResult> results;

    int numPoints=10000;
    int numClusters=5;
    int maxIteration=10;
    float epsilon=0.01;
    
    for (int num_Threads = 1; num_Threads <= max_num_threads; num_Threads++) {
        float execution_time =  execute_SoA(numPoints,numClusters,maxIteration,epsilon,omp_get_max_threads());
        results.push_back({numPoints, num_Threads, execution_time});
        std::ofstream file("../data/execution_times.csv");
        file << "num_points,num_threads,execution_time\n";
        for (const auto& result : results) {
            file << result.num_points << "," << result.num_threads << "," << result.execution_time << "\n";
        }
        file.close();
    }
}




int main() {
    std::cout << omp_get_max_threads() << std::endl;
    //SoA_varing_num_threads(omp_get_max_threads());
    execute_SoA(10000,5,10,0.01,omp_get_max_threads());

    return 0;
}