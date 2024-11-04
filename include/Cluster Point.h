#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <random>
#include <omp.h>


class Point{
    private:
    float x;
    float y;
    int assign;
    public:
    Point(float x, float y, int assign){
        this->x = x;
        this->y = y;
        this->assign = assign;
    }

    Point(){
        this->x = 0;
        this->y = 0;
        this->assign = -1;
    }
    float getX(){
        return x;
    }
    float getY(){
        return y;
    }
    int getAssign(){
        return assign;
    }
    void setAssign(int assign){
        this->assign = assign;
    }
    
    void setX(float x){
        this->x = x;
    }
    void setY(float y){
        this->y = y;
    }

    float getDistance(float centroid_x, float centroid_y){
        float dx = x - centroid_x;
        float dy = y - centroid_y;
        return sqrt(dx * dx + dy * dy);
    }
    
};
class Cluster{
    private:
    float x;
    float y;
    float cum_sum_x;
    float cum_sum_y;
    int size;
    int assign;
    public:
    Cluster(float x, float y, int size){
        this->x = x;
        this->y = y;
        this->size = size;
    }
    Cluster(){
        this->x = 0;
        this->y = 0;
        this->size = 0;
    }
    float getX(){
        return x;
    }
    float getY(){
        return y;
    }
    int getSize(){
        return size;
    }

    int getAssign(){
        return assign;
    }

    void setAssign(int assign){ 
        this->assign = assign;
    }

    void setSize(int size){
        this->size = size;
    }
    void setX(float x){
        this->x = x;
    }
    void setY(float y){
        this->y = y;
    }

    bool updateCentroid(float epsilon){
        if (size == 0) return true;

        bool flag = false;
        float newx = cum_sum_x / size;
        float newy = cum_sum_y / size;
        size = 0;
        cum_sum_x = 0;
        cum_sum_y = 0;

        if (abs(x - newx) < epsilon && abs(y - newy) < epsilon){
            flag = true;
        }

        x = newx;
        y = newy;

        return flag;
    }

    void addPoint(Point point){
        cum_sum_x += point.getX();
        cum_sum_y += point.getY();
        size++;
    }

};

void savePointsToCSV(std::vector<Point>& points, const std::string& filename) { 
    std::ofstream file(filename); 
    if (!file.is_open()) {
         std::cerr << "Failed to open the file!" << std::endl; 
         return; } 
    file << "x,y,cluster_id\n"; 
    for (auto& point : points) {
         file << point.getX() << "," << point.getY() << ","
          << point.getAssign() << "\n"; } file.close();

}

void saveCentroidsToCSV(std::vector<Cluster>& clusters, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file!" << std::endl;
        return;
    }

    file << "x,y,cluster_id\n";
    for (auto& cluster : clusters) {
        file << cluster.getX() << "," << cluster.getY() << "," << cluster.getAssign() <<   "\n";
    }

    file.close();
}




std::vector<Point> generatePoints(int num_points, int num_clusters, int num_threads) {
    // Resize the vector to hold the required number of points
    std::vector<Point> points;
    points.resize(num_points);

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Random number generation setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-100.0f, 100.0f);
    std::uniform_int_distribution<int> cluster_dis(0, num_clusters - 1);

    #pragma omp parallel
    {
        // Each thread gets its own random number generator to avoid data races
        std::mt19937 thread_gen(rd() + omp_get_thread_num());
        std::uniform_real_distribution<float> thread_dis(-100.0f, 100.0f);
        std::uniform_int_distribution<int> thread_cluster_dis(0, num_clusters - 1);

        #pragma omp for
        for (int i = 0; i < num_points; ++i) {
            points[i].setX(thread_dis(thread_gen));
            points[i].setY(thread_dis(thread_gen));
            points[i].setAssign(thread_cluster_dis(thread_gen)); // thread_cluster_dis(thread_gen);
        }
    }
    return points;
}


std::vector<Cluster> generateClusters(int num_clusters, int num_threads) {
    // Resize the vector to hold the required number of clusters
    std::vector<Cluster> clusters;
    clusters.resize(num_clusters);

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Random number generation setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-100.0f, 100.0f);

    #pragma omp parallel
    {
        // Each thread gets its own random number generator to avoid data races
        std::mt19937 thread_gen(rd() + omp_get_thread_num());
        std::uniform_real_distribution<float> thread_dis(-100.0f, 100.0f);

        #pragma omp for
        for (int i = 0; i < num_clusters; ++i) {
            clusters[i].setX(thread_dis(thread_gen));
            clusters[i].setY(thread_dis(thread_gen));
            clusters[i].setAssign(i);
        }
    }
    return clusters;
}


