#include<bits/stdc++.h>
using namespace std;

struct Point {
    int64_t x, y;

    Point(int64_t x_, int64_t y_): x(x_), y(y_) {}

    Point operator - (const Point & p) const {
        return Point(x - p.x, y - p.y);
    }

    int64_t cross (const Point & p) const {
        return x * p.y - y * p.x;
    }

    int64_t cross (const Point & p, const Point & q) const {
        return (p - *this).cross(q - *this);
    }

    int half () const {
        return int(y < 0 || (y == 0 && x < 0));
    }
};

void ldfs(size_t vi, size_t ui, vector<bool> &visited, vector<Point> vertices, vector<vector<size_t>> &adj, vector<size_t> &ans, vector<size_t> &parent){
    cout<< ui << " ";
    ans.push_back(ui);
    visited[ui]=true;
    parent[ui]=vi;
    if(adj[ui].size()==0){
        return;
    }
    size_t e1 = lower_bound(adj[ui].begin(), adj[ui].end(), vi, [&](size_t vi, size_t r) {
                    Point pl = vertices[vi] - vertices[ui];
                    Point pr = vertices[r] - vertices[ui];
                    if (pl.half() != pr.half())
                        return pl.half() < pr.half();
                    return pl.cross(pr) > 0;
                }) - adj[ui].begin();
                if (e1 >= adj[ui].size()) {
                    e1 = 0;
                }
    // cout<<"e1:"<<e1<<endl;
    // cout<<"next "<<adj[ui][e1]<<endl;

    for(size_t i=e1; i<adj[ui].size(); i++){
        // cout<<"i:"<<i<<endl;
        if (!visited[adj[ui][i]])
            ldfs(ui, adj[ui][i], visited, vertices, adj, ans, parent);
    }
    
    for(size_t i=0; i<e1; i++){
        // cout<<"i:"<<i<<endl;
        if (!visited[adj[ui][i]])
            ldfs(ui, adj[ui][i], visited, vertices, adj, ans, parent);
    }
    // cout<<"done"<<endl;
}

void print_2d(vector<vector<size_t>> m){
    for(size_t i=0; i<m.size(); i++){
        for(size_t j=0; j<m[i].size(); j++){
            cout<<m[i][j]<<" ";
        }
        cout<<endl;
    }
}

vector<size_t> ldfs_call(vector<Point> vertices, int vi, int ui, vector<vector<size_t>> &adj){
    size_t n = vertices.size();
    for (size_t i = 0; i < n; i++) {
        auto compare = [&](size_t l, size_t r) {
            Point pl = vertices[l] - vertices[i];
            Point pr = vertices[r] - vertices[i];
            if (pl.half() != pr.half())
                return pl.half() < pr.half();
            return pl.cross(pr) > 0;
        };
        sort(adj[i].begin(), adj[i].end(), compare);
    }
    cout<<"Adjacency list updated:"<<endl;
    print_2d(adj);

    vector<bool> visited(vertices.size());
    cout<<"Starting edge:" <<vi<<"->"<<ui<<endl;
    // cout<<"vi:"<<vi<<" "<<endl;
    // cout<<"ui:"<<ui<<" "<<endl;
    cout<<vi<<" ";
    vector<size_t> ans={};
    ans.push_back(vi);
    visited[vi]=true;
    vector<size_t> parent(n,-1);
    cout<<"done"<<endl;
    ldfs(vi, ui, visited, vertices, adj, ans, parent); 
    for(int i=0; i<parent.size(); i++){
        cout<<"parent of" <<i<<":" <<parent[i]<<endl;
    }
    return ans;  
}



int main(){
     vector<Point> vertices=  {{0, 1}, {1, 2}, {2, 2}, {1, 0}, {2, 0}, {3, 1}};
    vector<vector<size_t>> adj =
{
    {1, 3},   // Vertex 0 is adjacent to vertices 1 and 3
    {2, 3},// Vertex 1 is adjacent to vertices 2 and 3
    {5},   // Vertex 2 is adjacent to vertices 5
    {2, 4}, //Vertex 3 is adjacent to vertex 2 and 4
    {5},    // Vertex 4 is adjacent to vertex 5
    {}   // Vertex 5 is adjacent to vertex no one
};
//      vector<Point> vertices=  {{0, 0}, {1, -3}, {2, 0}, {1, -1}, {2, -2}, {3, -1}};

//     vector<vector<size_t>> adj =
// {
//     {3},   // Vertex 0 is adjacent to vertices 1 and 3
//     {},// Vertex 1 is adjacent to vertices 2 and 3
//     {5},   // Vertex 2 is adjacent to vertices 5
//     {1, 2, 4}, //Vertex 3 is adjacent to vertex 2 and 4
//     {5},    // Vertex 4 is adjacent to vertex 5
//     {}   // Vertex 5 is adjacent to vertex no one
// };
    cout<<"original adjacency list:"<<endl;
    print_2d(adj);
    ldfs_call(vertices, 0, 3, adj);
    return 0;
}