#include "Cluster Point.h"
#include <limits>
#include <omp.h>

void kMeansAoS_seq(std::vector<Point>& points, int num_clusters, int max_iterations, bool saving = false) {
    std::vector<Cluster> clusters(num_clusters);

    std::mt19937 gen(std::random_device{}() ^ omp_get_thread_num());
    std::uniform_real_distribution<float> dis(-100.0f, 100.0f);

    for (int i = 0; i < num_clusters; ++i) {
        clusters[i] = Cluster(dis(gen), dis(gen));
    }

    
    for (int iter = 0; iter < max_iterations; ++iter) {
        // Assignment step
        for (auto& point : points) {
            float min_distance = std::numeric_limits<float>::max();
            int best_cluster = -1;

            for (size_t i = 0; i < clusters.size(); ++i) {
                float distance = point.distanceTo(clusters[i].x, clusters[i].y);
                if (distance < min_distance) {
                    min_distance = distance;
                    best_cluster = i;
                }
            }
            point.cluster_id = best_cluster;
        }

        // Update step
        for (auto& cluster : clusters) {
            cluster.reset();
        }

        for (const auto& point : points) {
            clusters[point.cluster_id].addPoint(point);
        }

        for (auto& cluster : clusters) {
            cluster.updateCenter();
        }
    }
    if (saving) {
        saveLabelsToCSV(points, "../data/labels.csv");
    }
}

void kMeansAoS_parallel(std::vector<Point>& points, int num_clusters, int max_iterations, int num_threads, bool saving = false) {
    omp_set_num_threads(num_threads);  

    std::vector<Cluster> clusters(num_clusters);
    #pragma omp parallel
    {
        std::mt19937 gen(std::random_device{}() ^ omp_get_thread_num());
        std::uniform_real_distribution<float> dis(-100.0f, 100.0f);

        #pragma omp for
        for (int i = 0; i < num_clusters; ++i) {
            clusters[i] = Cluster(dis(gen), dis(gen));
        }
    }

    for (int iter = 0; iter < max_iterations; ++iter) {
        // Assignment Step
        #pragma omp parallel for schedule(dynamic, 1000)
        for (size_t i = 0; i < points.size(); ++i) {
            float min_distance = std::numeric_limits<float>::max();
            int best_cluster = -1;

            for (size_t j = 0; j < clusters.size(); ++j) {
                float distance = points[i].distanceTo(clusters[j].x, clusters[j].y);
                if (distance < min_distance) {
                    min_distance = distance;
                    best_cluster = j;
                }
            }
            points[i].cluster_id = best_cluster;
        }

        std::vector<float> cluster_sum_x(clusters.size(), 0.0f);
        std::vector<float> cluster_sum_y(clusters.size(), 0.0f);
        std::vector<int> cluster_count(clusters.size(), 0);

        // Accumulate
        #pragma omp parallel for
        for (size_t i = 0; i < points.size(); ++i) {
            int cluster_id = points[i].cluster_id;

            #pragma omp atomic
            cluster_sum_x[cluster_id] += points[i].x;
            #pragma omp atomic
            cluster_sum_y[cluster_id] += points[i].y;
            #pragma omp atomic
            cluster_count[cluster_id]++;
        }

        // Update the cluster
        #pragma omp parallel for
        for (size_t i = 0; i < clusters.size(); ++i) {
            clusters[i].updateCenter(cluster_sum_x[i], cluster_sum_y[i], cluster_count[i]);
        }
    }

    if (saving) {
        saveLabelsToCSV(points, "../data/labels.csv");
    }
}



/*

void kMeansAoS_parallel(std::vector<Point>& points, int num_clusters, int max_iterations, int num_threads, bool saving = false) {
    omp_set_num_threads(num_threads);  

    std::vector<Cluster> clusters(num_clusters);
    for (int i = 0; i < num_clusters; ++i) {
        clusters[i] = Cluster(points[i].x, points[i].y); 
    }

    #pragma omp parallel for
    for (int iter = 0; iter < max_iterations; ++iter) {
        // Assignment Step
        #pragma omp parallel for schedule(dynamic, 1000)
        for (size_t i = 0; i < points.size(); ++i) {
            float min_distance = std::numeric_limits<float>::max();
            int best_cluster = -1;

            for (size_t j = 0; j < clusters.size(); ++j) {
                float distance = points[i].distanceTo(clusters[j].x, clusters[j].y);
                if (distance < min_distance) {
                    min_distance = distance;
                    best_cluster = j;
                }
            }
            points[i].cluster_id = best_cluster;
        }

        float* cluster_sum_x = new float[clusters.size()]();  
        float* cluster_sum_y = new float[clusters.size()]();  
        int* cluster_count = new int[clusters.size()]();      

        

        // Reset the clusters before accumulation
        #pragma omp parallel for
        for (size_t i = 0; i < clusters.size(); ++i) {
            clusters[i].reset();
        }

        // Accumulate 
        #pragma omp parallel for reduction(+:cluster_sum_x[:clusters.size()], cluster_sum_y[:clusters.size()], cluster_count[:clusters.size()])
        for (size_t i = 0; i < points.size(); ++i) {
            int cluster_id = points[i].cluster_id;

            cluster_sum_x[cluster_id] += points[i].x;
            cluster_sum_y[cluster_id] += points[i].y;
            cluster_count[cluster_id]++;  
        }

        // Update the cluster
        #pragma omp parallel for
        for (size_t i = 0; i < clusters.size(); ++i) {
            clusters[i].updateCenter(cluster_sum_x[i], cluster_sum_y[i], cluster_count[i]);
        }

        delete[] cluster_sum_x;
        delete[] cluster_sum_y;
        delete[] cluster_count;
    }
    if (saving) {
        saveLabelsToCSV(points, "../data/labels.csv");
    }
}


*/
