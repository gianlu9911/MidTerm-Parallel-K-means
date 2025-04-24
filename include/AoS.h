#include "Cluster Point.h"
#include <limits>
#include <omp.h>

void kMeansAoS_seq(std::vector<Point>& points, int num_clusters, int max_iterations, bool saving = false) {
    std::vector<Cluster> clusters(num_clusters);
    for (int i = 0; i < num_clusters; ++i) {
        clusters[i] = Cluster();
    }

    for (int iter = 0; iter < max_iterations; ++iter) {
        // Assignment step
        for (auto& point : points) {
            float min_distance = std::numeric_limits<float>::max();
            int best_cluster = -1;

            for (size_t i = 0; i < clusters.size(); ++i) {
                float distance = point.distanceTo(clusters[i].getX(), clusters[i].getY());
                if (distance < min_distance) {
                    min_distance = distance;
                    best_cluster = i;
                }
            }
            point.setClusterId(best_cluster);
        }

        // Update step
        for (auto& cluster : clusters) {
            cluster.reset();
        }

        for (const auto& point : points) {
            clusters[point.getClusterId()].addPoint(point);
        }

        for (auto& cluster : clusters) {
            cluster.updateCenter();
        }
    }

    if (saving) {
        saveLabelsToCSV(points, "../data/labels.csv");
    }
}

void kMeansAoS_parallel(std::vector<Point>& points,
                        int num_clusters,
                        int max_iterations,
                        int num_threads,
                        bool saving = false) {
    omp_set_num_threads(num_threads);

    // 1) Initialize clusters
    std::vector<Cluster> clusters(num_clusters);
    #pragma omp parallel for
    for (int i = 0; i < num_clusters; ++i) {
        clusters[i] = Cluster();
    }

    // 2) Allocate C‑arrays for reduction
    float* cluster_sum_x = new float[num_clusters];
    float* cluster_sum_y = new float[num_clusters];
    int*   cluster_count = new int[num_clusters];

    // 3) Main parallel region
    #pragma omp parallel shared(points, clusters, cluster_sum_x, cluster_sum_y, cluster_count)
    {
        for (int iter = 0; iter < max_iterations; ++iter) {
            // --- Assignment step ---
            #pragma omp for
            for (size_t i = 0; i < points.size(); ++i) {
                float min_distance = std::numeric_limits<float>::max();
                int best_cluster = -1;

                for (int j = 0; j < num_clusters; ++j) {
                    float d = points[i].distanceTo(clusters[j].getX(), clusters[j].getY());
                    if (d < min_distance) {
                        min_distance = d;
                        best_cluster = j;
                    }
                }

                points[i].setClusterId(best_cluster);
            }

            // --- Zero out accumulators once per iteration ---
            #pragma omp for
            for (int c = 0; c < num_clusters; ++c) {
                cluster_sum_x[c] = 0.0f;
                cluster_sum_y[c] = 0.0f;
                cluster_count[c] = 0;
            }

            // --- Accumulation with array reduction (no atomics) ---
            #pragma omp for reduction(+: cluster_sum_x[:num_clusters], \
                                           cluster_sum_y[:num_clusters], \
                                           cluster_count[:num_clusters])
            for (size_t i = 0; i < points.size(); ++i) {
                int cid = points[i].getClusterId();
                cluster_sum_x[cid] += points[i].getX();
                cluster_sum_y[cid] += points[i].getY();
                cluster_count[cid] += 1;
            }

            // --- Update cluster centers ---
            #pragma omp for
            for (int c = 0; c < num_clusters; ++c) {
                clusters[c].updateCenter(cluster_sum_x[c],
                                         cluster_sum_y[c],
                                         cluster_count[c]);
            }
        }
    }

    // 4) Cleanup
    delete[] cluster_sum_x;
    delete[] cluster_sum_y;
    delete[] cluster_count;

    // 5) Optional save
    if (saving) {
        saveLabelsToCSV(points, "../data/labels.csv");
    }
}
