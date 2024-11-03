#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>


struct alignas(64) Points {
    std::vector<float> x;
    std::vector<float> y;
    std::vector<int> cluster_id;
    std::vector<float> minDistance;
};
struct alignas(64) Clusters
{
    std::vector<float> x;
    std::vector<float> y;
    std::vector<int> cluster_id;
    std::vector<int> size;
    std::vector<float> cum_sum_x;
    std::vector<float> cum_sum_y;
};

struct TimingResult {
    int num_points;
    int num_threads;
    double execution_time;
};

void savePointsToCSV(const std::vector<float>& x, const std::vector<float>& y, const std::vector<int>& cluster_id, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file!" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << "x,y,cluster_id\n";
    for (size_t i = 0; i < x.size(); ++i) {
        buffer << x[i] << "," << y[i] << "," << cluster_id[i] << "\n";
    }

    // Write the entire buffer to the file in one go
    file << buffer.str();

    file.close();
}

void savePointsToCSV(const std::vector<float>& x, const std::vector<float>& y, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file!" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << "x,y,cluster_id\n";
    for (size_t i = 0; i < x.size(); ++i) {
        buffer << x[i] << "," << y[i] << "," << i << "\n";
    }

    // Write the entire buffer to the file in one go
    file << buffer.str();

    file.close();
}

