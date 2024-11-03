#include "SoA.h"

int main() {

    int numPoints=10000;
    int numClusters=5;
    int maxIteration=10;
    float epsilon=0.01;
    float max_value=100;
    int num_Thjreads=4;
    
    float execution_time =  execute_SoA(numPoints,numClusters,maxIteration,epsilon,max_value,omp_get_max_threads());
    float sequential_time = execute_SoA(numPoints,numClusters,maxIteration,epsilon,max_value,1,true);
    return 0;
}