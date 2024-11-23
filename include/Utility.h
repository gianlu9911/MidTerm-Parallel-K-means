#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <limits>
#include <math.h>
#include <random>

struct alignas(64) PointsSoA {
    std::vector<float> x;
    std::vector<float> y;

    PointsSoA(size_t numPoints) : x(numPoints), y(numPoints) {}

    float getX(size_t index) const { return x[index]; }
    float getY(size_t index) const { return y[index]; }

    void set(size_t index, float xVal, float yVal) {
        x[index] = xVal;
        y[index] = yVal;
    }
};


PointsSoA readCSVToSoA(const std::string &filename, size_t numPoints) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    size_t count = 0;
    
    // Create the PointsSoA structure with a size of 'numPoints'
    PointsSoA points(numPoints);

    // Read file line by line until we read 'numPoints' or reach the end
    while (std::getline(file, line) && count < numPoints) {
        std::stringstream ss(line);
        std::string xStr, yStr;
        if (std::getline(ss, xStr, ',') && std::getline(ss, yStr, ',')) {
            points.set(count, std::stof(xStr), std::stof(yStr));
            count++;
        }
    }

    file.close();

    if (count < numPoints) {
        throw std::runtime_error("Not enough data in the file");
    }

    return points;
}

float distance(const PointsSoA &points, size_t pointIndex, float centroidX, float centroidY) {
    float dx = points.getX(pointIndex) - centroidX;
    float dy = points.getY(pointIndex) - centroidY;
    return (dx * dx + dy * dy);
}

void saveExecutionTimeToCSV(const std::string& filename, size_t numPoints, int numClusters, int maxIterations, double executionTime, int numThreads) {
    std::ofstream file(filename, std::ios::app); 
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file to save execution times");
    }

    file.seekp(0, std::ios::end);  
    if (file.tellp() == 0) {
        file << "numPoints,numClusters,maxIterations,executionTime,numThreads\n";
    }

    file << numPoints << "," << numClusters << "," << maxIterations << "," << executionTime << "," << numThreads << "\n";
    file.close();
}


void initializeCentroidsKMeansPlusPlus(const PointsSoA &points, size_t numPoints, int k, std::vector<float> &centroidsX, std::vector<float> &centroidsY) {
    std::mt19937 gen(std::random_device{}());  

    std::uniform_int_distribution<size_t> dis(0, numPoints - 1);
    size_t firstCentroid = dis(gen);
    centroidsX[0] = points.getX(firstCentroid);
    centroidsY[0] = points.getY(firstCentroid);

    std::vector<float> distances(numPoints, std::numeric_limits<float>::max());
    for (int i = 1; i < k; i++) {
        // Compute distances of all points to the nearest centroid
        for (size_t j = 0; j < numPoints; j++) {
            float d = std::sqrt(std::pow(points.getX(j) - centroidsX[i-1], 2) + std::pow(points.getY(j) - centroidsY[i-1], 2));
            if (d < distances[j]) {
                distances[j] = d;
            }
        }

        // Select the next centroid with probability proportional to the distance squared
        float totalDistance = std::accumulate(distances.begin(), distances.end(), 0.0f);
        std::uniform_real_distribution<float> dist(0.0f, totalDistance);
        float randomValue = dist(gen);
        float sum = 0.0f;
        for (size_t j = 0; j < numPoints; j++) {
            sum += distances[j];
            if (sum >= randomValue) {
                centroidsX[i] = points.getX(j);
                centroidsY[i] = points.getY(j);
                break;
            }
        }
    }
}

void saveLabelsToCSV(const std::vector<int> &labels, const std::string &filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (size_t i = 0; i < labels.size(); i++) {
            file << labels[i] << "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for saving labels." << std::endl;
    }
}