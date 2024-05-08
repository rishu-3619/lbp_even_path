#include<bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
};

bool pointComparator(const Point& p1, const Point& p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

// Function to find common points between two vectors
std::vector<Point> findCommonPoints(const std::vector<Point>& ldfs_tree, const std::vector<Point>& rdfs_tree) {
    std::vector<Point> commonPoints;

    // Iterate over points in the first vector
    for (const auto& p1 : ldfs_tree) {
        // Check if the point exists in the second vector
        auto it = std::find_if(rdfs_tree.begin(), rdfs_tree.end(), [&](const Point& p2) {
            return pointComparator(p1, p2);
        });
        
        if (it != rdfs_tree.end()) {
            // If found, add it to the commonPoints vector
            commonPoints.push_back(p1);
        }
    }

    return commonPoints;
}

int main(){
    std::vector<Point> ldfs_tree = {{1, 1}, {2, 2}, {3, 3}};
    std::vector<Point> rdfs_tree = {{2, 2}, {3, 3}, {4, 4}};

    // Find common points
    vector<Point> commonPoints = findCommonPoints(ldfs_tree, rdfs_tree);

    // Print common points
    std::cout << "Common points:" << std::endl;
    for (const auto& p : commonPoints) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }

    return 0;
}