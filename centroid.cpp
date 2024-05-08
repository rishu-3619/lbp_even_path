#include<bits/stdc++.h>
using namespace std;

struct Point
{
    double x;
    double y;
};

Point find_Centroid(vector<size_t> face, vector<Point> vertices)
{
    Point ans = { 0, 0 };
     
    int n = face.size();
    double signedArea = 0;
     
    // For all vertices
    for (int i = 0; i < face.size(); i++) {
         
        double x0 = vertices[face[i]].x, y0 = vertices[face[i]].y;
        double x1 = vertices[(face[i] + 1) % n].x, y1 = 
                            vertices[(face[i] + 1) % n].y;
                             
        // Calculate value of A
        // using shoelace formula
        double A = (x0 * y1) - (x1 * y0);
        signedArea += A;
         
        // Calculating coordinates of
        // centroid of polygon
        ans.x += (x0 + x1) * A;
        ans.y += (y0 + y1) * A;
    }
 
    signedArea *= 0.5;
    ans.x = (ans.x) / (6 * signedArea);
    ans.y = (ans.y) / (6 * signedArea);
 
    return ans;
}

int main()
{   vector<Point> vertices=  {{0, 1}, {1, 2}, {2, 2}, {1, 0}, {2, 0}, {3, 1}};
    vector<size_t> face=  {1, 2, 3};
    Point centroid = find_Centroid(face, vertices);
    std::cout << "Centroid is (" << centroid.x << ", " << centroid.y << ")\n";
}