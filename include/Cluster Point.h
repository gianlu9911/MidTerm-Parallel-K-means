#include <cfloat>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>


class Point {
public:
    float x, y;
    int cluster_id;

    Point(float x_val = 0, float y_val = 0) : x(x_val), y(y_val), cluster_id(-1) {}

    float distanceTo(float cluster_x, float cluster_y) const {
        return std::pow(x - cluster_x, 2) + std::pow(y - cluster_y, 2);
    }
};


class Cluster {
public:
    float x, y;
    float sum_x, sum_y;
    int size;

    Cluster(float x_val = 0, float y_val = 0){
        
        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::uniform_real_distribution<> dis(-100.0, 100.0); 

        x = dis(gen);
        y = dis(gen);
        sum_x= 0;
        sum_y= 0;
        size = 0;
    }

    void reset() {
        sum_x = 0;
        sum_y = 0;
        size = 0;
    }

    void addPoint(const Point& point) {
        sum_x += point.x;
        sum_y += point.y;
        size++;
    }

    void updateCenter(float sum_x, float sum_y, int count) {
        if (count > 0) {
            x = sum_x / count;
            y = sum_y / count;
        }
    }

    void updateCenter() {
        if (size > 0) {
            x = sum_x / size;
            y = sum_y / size;
        }
    }
};


void readPointsFromCSV(const std::string& filename, std::vector<Point>& points, int numPoints) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    int count = 0;

    while (std::getline(file, line) && count < numPoints) {
        std::stringstream ss(line);
        std::string x_str, y_str;

        std::getline(ss, x_str, ',');
        std::getline(ss, y_str, ',');

        Point p(std::stof(x_str), std::stof(y_str));
        points.push_back(p);
        count++;
    }

    file.close();
}

void saveExecutionTimesToCSV(const std::string& filename, 
                             size_t numPoints, 
                             int numClusters, 
                             int maxIterations, 
                             double executionTime, 
                             int numThreads) {
    std::ofstream file(filename, std::ios::app); 
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    // Write header only if the file is empty
    static bool isHeaderWritten = false;
    if (!isHeaderWritten) {
        file << "numPoints,numClusters,maxIterations,executionTime,numThreads\n";
        isHeaderWritten = true;
    }

    file << numPoints << ',' 
         << numClusters << ',' 
         << maxIterations << ',' 
         << executionTime << ',' 
         << numThreads << '\n';

    file.close();
}

void saveLabelsToCSV(const std::vector<Point>& points, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::ios_base::failure("Error: Could not open file for writing.");
    }

    // Write each point's cluster assignment
    for (const auto& point : points) {
        file << point.cluster_id << '\n';
    }

    file.close();
    if (!file) {
        throw std::ios_base::failure("Error: Could not write to file successfully.");
    }
}