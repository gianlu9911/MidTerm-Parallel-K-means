#include "Utility.h"
#include <omp.h>

void kmeansSoA_seq(const PointsSoA &points, size_t numPoints, int k, int maxIterations, bool saving = false) {
    std::vector<float> centroidsX(k), centroidsY(k);
    for (int i = 0; i < k; i++) {
        size_t randomIndex = std::rand() % numPoints;
        centroidsX[i] = points.getX(randomIndex);
        centroidsY[i] = points.getY(randomIndex);
    }

    std::vector<int> labels(numPoints, -1);
    for (int iter = 0; iter < maxIterations; iter++) {

        // Assign points to the nearest centroid
        for (size_t i = 0; i < numPoints; i++) {
            int bestCluster = -1;
            float bestDistance = std::numeric_limits<float>::max();
            for (int j = 0; j < k; j++) {
                float d = distance(points, i, centroidsX[j], centroidsY[j]);
                if (d < bestDistance) {
                    bestDistance = d;
                    bestCluster = j;
                }
            }
            if (labels[i] != bestCluster) {
                labels[i] = bestCluster;
            }
        }

        // Update centroids
        std::vector<float> newCentroidsX(k, 0.0f), newCentroidsY(k, 0.0f);
        std::vector<int> counts(k, 0);
        for (size_t i = 0; i < numPoints; i++) {
            int cluster = labels[i];
            newCentroidsX[cluster] += points.getX(i);
            newCentroidsY[cluster] += points.getY(i);
            counts[cluster]++;
        }

        for (int j = 0; j < k; j++) {
            if (counts[j] > 0) {
                centroidsX[j] = newCentroidsX[j] / counts[j];
                centroidsY[j] = newCentroidsY[j] / counts[j];
            }
        }
    }

    if(saving) {
        saveLabelsToCSV(labels, "../data/labels_seq.csv");
    }
    
}

void kmeansSoA_parallel(const PointsSoA &points, size_t numPoints, int k, int maxIterations, int numThreads, bool saving = false) {
    omp_set_num_threads(numThreads);
    
    // Explicitly initialize centroids
    std::vector<float> centroidsX(k), centroidsY(k);
    #pragma omp parallel for
    for (int i = 0; i < k; i++) {
        // Generate random float within the range
        centroidsX[i] = (-100) + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (200)));
        centroidsY[i] = (-100) + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (200)));
    }

    std::vector<int> labels(numPoints, -1);

    // Allocate arrays for centroid updates (no need to delete)
    float newCentroidsX[k] = {0.0f}, newCentroidsY[k] = {0.0f};
    int counts[k] = {0};

    // Iterate through the algorithm for maxIterations
    #pragma omp parallel shared(newCentroidsX, newCentroidsY, counts)
    {
        for (int iter = 0; iter < maxIterations; iter++) {

            // Parallelize point-to-centroid assignment
            #pragma omp for
            for (size_t i = 0; i < numPoints; i++) {
                int bestCluster = -1;
                float bestDistance = std::numeric_limits<float>::max();
                for (int j = 0; j < k; j++) {
                    float d = distance(points, i, centroidsX[j], centroidsY[j]);
                    if (d < bestDistance) {
                        bestDistance = d;
                        bestCluster = j;
                    }
                }
                labels[i] = bestCluster;
            }

            // Parallelize centroid update with reduction
            #pragma omp for reduction(+:newCentroidsX, newCentroidsY, counts)
            for (size_t i = 0; i < numPoints; i++) {
                int cluster = labels[i];
                newCentroidsX[cluster] += points.getX(i);
                newCentroidsY[cluster] += points.getY(i);
                counts[cluster]++;
            }

            // Update centroids after parallel computation
            #pragma omp for
            for (int j = 0; j < k; j++) {
                if (counts[j] > 0) {
                    centroidsX[j] = newCentroidsX[j] / counts[j];
                    centroidsY[j] = newCentroidsY[j] / counts[j];
                }
            }
        }
    }

    // Save the labels if requested
    if (saving) {
        saveLabelsToCSV(labels, "../data/labels.csv");
    }
}
