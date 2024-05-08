#include <bits/stdc++.h>
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

void print_2d(vector<vector<size_t>> m){
    for(size_t i=0; i<m.size(); i++){
        for(size_t j=0; j<m[i].size(); j++){
            cout<<m[i][j]<<" ";
        }
        cout<<endl;
    }
}


vector<vector<size_t>> find_faces(vector<Point> vertices, vector<vector<size_t>> &adj) {
    size_t n = vertices.size();
    vector<vector<char>> used(n);
    for (size_t i = 0; i < n; i++) {
        used[i].resize(adj[i].size());
        used[i].assign(adj[i].size(), 0);
        auto compare = [&](size_t l, size_t r) {
            Point pl = vertices[l] - vertices[i];
            Point pr = vertices[r] - vertices[i];
            if (pl.half() != pr.half())
                return pl.half() < pr.half();
            return pl.cross(pr) > 0;
        };
        sort(adj[i].begin(), adj[i].end(), compare);
    }
    print_2d(adj);
    vector<vector<size_t>> faces;
    for (size_t i = 0; i < n; i++) {
        for (size_t edge_id = 0; edge_id < adj[i].size(); edge_id++) {
            if (used[i][edge_id]) {
                continue;
            }
            vector<size_t> face;
            size_t v = i;
            size_t e = edge_id;
            while (!used[v][e]) {
                used[v][e] = true;
                face.push_back(v);
                size_t u = adj[v][e];
                size_t e1 = lower_bound(adj[u].begin(), adj[u].end(), v, [&](size_t l, size_t r) {
                    Point pl = vertices[l] - vertices[u];
                    Point pr = vertices[r] - vertices[u];
                    if (pl.half() != pr.half())
                        return pl.half() < pr.half();
                    return pl.cross(pr) > 0;
                }) - adj[u].begin() + 1;
                if (e1 == adj[u].size()) {
                    e1 = 0;
                }
                v = u;
                e = e1;
            }
            // reverse(face.begin(), face.end());
            int sign = 0;
            for (size_t j = 0; j < face.size(); j++) {
                size_t j1 = (j + 1) % face.size();
                size_t j2 = (j + 2) % face.size();
                int64_t val = vertices[face[j]].cross(vertices[face[j1]], vertices[face[j2]]);
                if (val > 0) {
                    sign = 1;
                    break;
                } else if (val < 0) {
                    sign = -1;
                    break;
                }
            }
            if (sign <= 0) {
                faces.insert(faces.begin(), face);
            } else {
                faces.emplace_back(face);
            }
        }
    }
    return faces;
}


int main(){
    vector<Point> vertices=  {{0, 1}, {1, 2}, {2, 2}, {1, 0}, {2, 0}, {3, 1}};
    vector<vector<size_t>> adj =
{
    {1, 3},   // Vertex 0 is adjacent to vertices 1 and 3
    {0, 2, 3},// Vertex 1 is adjacent to vertices 0, 2, and 3
    {1, 3, 5},   // Vertex 2 is adjacent to vertices 1, 3 and 5
    {0, 1, 2, 4}, //Vertex 3 is adjacent to vertex 0, 1, 2 and 4
    {3, 5},    // Vertex 4 is adjacent to vertex 3 and 5
    {2, 4}   // Vertex 5 is adjacent to vertex 3 and 5
};
    cout<<"Original Adjacency list:"<<endl;
    print_2d(adj);
    cout<<"Reordered Adjacecy list:"<<endl;
    vector<vector<size_t>> k= find_faces(vertices, adj);
    cout<<"Faces:"<<endl;
    print_2d(k);
}