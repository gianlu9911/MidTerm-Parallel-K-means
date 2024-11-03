#include "SoA_Parallel.h"

int main() {

    int numPoints=100;
    int numClusters=5;
    int maxIteration=10;
    float epsilon=0.01;
    float max_value=100;
    int num_Thjreads=omp_get_max_threads();
    
    float execution_time =  execute_SoA_parallel(numPoints,numClusters,maxIteration,epsilon,max_value,num_Thjreads);
    return 0;
}