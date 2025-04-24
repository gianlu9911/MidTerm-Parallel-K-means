#include "Utility.h"
#include <omp.h>

void kmeansSoA_seq(const PointsSoA& points,
                   size_t numPoints,
                   int k,
                   int maxIterations,
                   bool saving = false)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-100.0f, 100.0f);

    std::vector<float> centroidsX(k), centroidsY(k);
    for (int i = 0; i < k; i++) {
        centroidsX[i] = dist(gen);
        centroidsY[i] = dist(gen);
    }

    std::vector<int> labels(numPoints, -1);
    for (int iter = 0; iter < maxIterations; iter++) {
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

        // 2) Update
        std::vector<float> newCentroidsX(k, 0.0f), newCentroidsY(k, 0.0f);
        std::vector<int> counts(k, 0);
        for (size_t i = 0; i < numPoints; i++) {
            int c = labels[i];
            newCentroidsX[c] += points.getX(i);
            newCentroidsY[c] += points.getY(i);
            counts[c]++;
        }

        for (int j = 0; j < k; j++) {
            if (counts[j] > 0) {
                centroidsX[j] = newCentroidsX[j] / counts[j];
                centroidsY[j] = newCentroidsY[j] / counts[j];
            }
        }
    }

    if (saving) {
        saveLabelsToCSV(labels, "../data/labels_seq.csv");
    }
}


void kmeansSoA_parallel(const PointsSoA& points,
                        size_t numPoints,
                        int k,
                        int maxIterations,
                        int numThreads,
                        bool saving = false)
{
    omp_set_num_threads(numThreads);

    std::vector<float> centroidsX(k), centroidsY(k);

    #pragma omp parallel
    {
        std::random_device rd;
        std::mt19937 gen(rd() ^ omp_get_thread_num());
        std::uniform_real_distribution<float> dis(-100.0f, 100.0f);

        #pragma omp for
        for (int i = 0; i < k; i++) {
            centroidsX[i] = dis(gen);
            centroidsY[i] = dis(gen);
        }
    }

    std::vector<int> labels(numPoints, -1);

    float newCentroidsX[k] = {0.0f}, newCentroidsY[k] = {0.0f};
    int counts[k] = {0};

    #pragma omp parallel shared(newCentroidsX, newCentroidsY, counts)
    {
        for (int iter = 0; iter < maxIterations; iter++) {
            #pragma omp for
            for (int j = 0; j < k; ++j) {
                newCentroidsX[j] = 0.0f;
                newCentroidsY[j] = 0.0f;
                counts[j]        = 0;
            }

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

            #pragma omp for reduction(+:newCentroidsX, newCentroidsY, counts)
            for (size_t i = 0; i < numPoints; i++) {
                int cluster = labels[i];
                newCentroidsX[cluster] += points.getX(i);
                newCentroidsY[cluster] += points.getY(i);
                counts[cluster]++;
            }

            #pragma omp for
            for (int j = 0; j < k; j++) {
                if (counts[j] > 0) {
                    centroidsX[j] = newCentroidsX[j] / counts[j];
                    centroidsY[j] = newCentroidsY[j] / counts[j];
                }
            }
        }
    }

    if (saving) {
        saveLabelsToCSV(labels, "../data/labels.csv");
    }
}
