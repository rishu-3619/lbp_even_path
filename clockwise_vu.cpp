#include<bits/stdc++.h>
using namespace std;

struct Point
{
    double x;
    double y;
};

bool clockwise(Point p1, Point p2, Point p3)
{
    double val = (p2.y - p1.y) * (p3.x - p2.x)
              - (p2.x - p1.x) * (p3.y - p2.y);
    int ans;
    if (val == 0)
        ans= 0; // collinear
 
    ans=(val > 0) ? 1 : 2; // clock or counterclock wise

    if(ans==1){
        return true;
    }
    return false;
}
 
int main(){
    cout<<"Clockwise:"<<clockwise({1.3333, 1.3333}, {1,2}, {2,2})<<endl;
    return 0;
}