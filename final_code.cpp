#include<bits/stdc++.h>
using namespace std;

struct Point {
    double x, y;

    Point(double x_, double y_): x(x_), y(y_) {}

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

Point find_Centroid(vector<size_t> face, vector<Point> vertices)
{
    Point ans = { 0, 0 };
     
    int n = face.size();
    double signedArea = 0;
     
    // For all vertices
    for (int i = 0; i < face.size(); i++) {
         
        double x0 = vertices[face[i]].x, y0 = vertices[face[i]].y;
        double x1 = vertices[face[(i + 1) % n]].x, y1 = 
                            vertices[face[(i + 1) % n]].y;
                             
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

bool face_with_clockwise_vu(vector<size_t> face, int v, int u, vector<Point> vertices){
    Point a= find_Centroid(face, vertices);
    return clockwise(a, vertices[v], vertices[u]);
}

void print_2d(vector<vector<size_t>> m){
    for(size_t i=0; i<m.size(); i++){
        for(size_t j=0; j<m[i].size(); j++){
            cout<<m[i][j]<<" ";
        }
        cout<<endl;
    }
}

void print_1d(vector<size_t> m){
    for(size_t i=0; i<m.size(); i++){
        cout<<m[i]<<" ";
    }
    cout<<endl;
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
    // print_2d(adj);
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

void ldfs(size_t vi, size_t ui, vector<bool> &visited, vector<Point> vertices, vector<vector<size_t>> &adj, vector<size_t> &ans, vector<size_t> &parent){
    // cout<< ui << " ";
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


vector<size_t> ldfs_call(vector<Point> vertices, int vi, int ui, vector<vector<size_t>> &adj, vector<size_t> &parent){
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
    // cout<<"Adjacency list updated:"<<endl;
    // print_2d(adj);

    vector<bool> visited(vertices.size());
    // cout<<"Starting edge:" <<vi<<"->"<<ui<<endl;
    // cout<<"vi:"<<vi<<" "<<endl;
    // cout<<"ui:"<<ui<<" "<<endl;
    // cout<<vi<<" ";
    vector<size_t> ans={};
    ans.push_back(vi);
    visited[vi]=true;
    ldfs(vi, ui, visited, vertices, adj, ans, parent); 
    // for(int i=0; i<parent.size(); i++){
    //     cout<<"parent of" <<i<<":" <<parent[i]<<endl;
    // }
    return ans;  
}

void rdfs(size_t vi, size_t ui, vector<bool> &visited, vector<Point> vertices, vector<vector<size_t>> &adj, vector<size_t> &ans, vector<size_t> &parent){
    // cout<< ui << " ";
    ans.push_back(ui);
    visited[ui]=true;
    parent[ui]=vi;
    if(adj[ui].size()==0){
        return;
    }
    size_t e1 = upper_bound(adj[ui].begin(), adj[ui].end(), vi, [&](size_t vi, size_t r) {
                    Point pl = vertices[vi] - vertices[ui];
                    Point pr = vertices[r] - vertices[ui];
                    if (pl.half() != pr.half())
                        return pl.half() > pr.half();
                    return pl.cross(pr) < 0;
                }) - adj[ui].begin()+1;
                if (e1 >= adj[ui].size()) {
                    e1 = 0;
                }
    // cout<<"e1:"<<e1<<endl;
    // cout<<"next "<<adj[ui][e1]<<endl;

    for(int i=e1; i>=0; i--){
        // cout<<"i:"<<i<<endl;
        if (!visited[adj[ui][i]])
            rdfs(ui, adj[ui][i], visited, vertices, adj, ans, parent);
    }
    
    for(size_t i=adj[ui].size()-1; i>e1; i--){
        // cout<<"i:"<<i<<endl;
        if (!visited[adj[ui][i]])
            rdfs(ui, adj[ui][i], visited, vertices, adj, ans, parent);
    }
    // cout<<"done"<<endl;
}



vector<size_t> rdfs_call(vector<Point> vertices, int vi, int ui, vector<vector<size_t>> &adj, vector<size_t> &parent){
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
    // cout<<"Adjacency list updated:"<<endl;
    // print_2d(adj);

    vector<bool> visited(vertices.size());
    // cout<<"Starting edge:" <<vi<<"->"<<ui<<endl;
    // cout<<"vi:"<<vi<<" "<<endl;
    // cout<<"ui:"<<ui<<" "<<endl;
    // cout<<vi<<" ";
    vector<size_t> ans={};
    ans.push_back(vi);
    visited[vi]=true;
    rdfs(vi, ui, visited, vertices, adj, ans, parent);  
    // for(int i=0; i<parent.size(); i++){
    //     cout<<"parent of" <<i<<":" <<parent[i]<<endl;
    // }
    return ans; 
}

vector<vector<size_t>> convertToUndirected(const vector<vector<size_t>> adj) {
    int V = adj.size(); // Number of vertices

    // Initialize the undirected adjacency list with V empty vectors
    vector<vector<size_t>> undirectedAdj(V);

    // Traverse the directed adjacency list
    for (int u = 0; u < V; ++u) {
        // Traverse the neighbors of vertex u in the directed adjacency list
        for (int v : adj[u]) {
            // Add edge (u, v) to the undirected adjacency list
            undirectedAdj[u].push_back(v);
            // Add edge (v, u) to the undirected adjacency list
            undirectedAdj[v].push_back(u);
        }
    }

    return undirectedAdj;
}
bool pointComparator(size_t p1, size_t p2) {
    return p1==p2;
}


vector<size_t> findCommonPoints(vector<size_t> ldfs_tree, vector<size_t> rdfs_tree, size_t i) {
    vector<size_t> commonPoints;

    // Iterate over points in the first vector
    for (const auto& p1 : ldfs_tree) {
        // Check if the point exists in the second vector
        auto it = std::find_if(rdfs_tree.begin(), rdfs_tree.end(), [&](size_t p2) {
            return pointComparator(p1, p2);
        });
        
        if (it != rdfs_tree.end()) {
            if(p1!=i){
                commonPoints.push_back(p1);
            }
            // If found, add it to the commonPoints vector
            
        }
    }

    return commonPoints;
}


vector<size_t> findPath(size_t start, size_t dest, vector<size_t> parent){
    vector<size_t> path;
    while(dest!=start){
        path.push_back(dest);
        dest=parent[dest];
    }
    path.push_back(dest);
    return path;
}

int main(){
    vector<Point> vertices=  {{0, 1}, {1, 2}, {2, 2}, {1, 0}, {2, 0}, {3, 1}};

    vector<vector<size_t>> adj =
{
    {1, 3},   // Vertex 0 is outgoing to vertices 1 and 3
    {2, 3},// Vertex 1 is outgoing to vertices 2, and 3
    {5},   // Vertex 2 is outgoing to vertices 5
    {2, 4}, //Vertex 3 is outgoing to vertex 2 and 4
    {5},    // Vertex 4 is outgoing to vertex 5
    {}   // Vertex 5 is outgoing to none
};  

    vector<vector<size_t>> undirected_adj=  convertToUndirected(adj);
    // cout<<"Undirected adj: "<<endl;
    // print_2d(undirected_adj);
    vector<vector<size_t>> f= find_faces(vertices, undirected_adj);
    for(int i=0; i<vertices.size(); i++){
        for(int j=0; j<adj[i].size(); j++){
            for(int k=0; k<f.size()-1; k++){
                if(count(f[k].begin(), f[k].end(), i) && count(f[k].begin(), f[k].end(), adj[i][j])){
                    if(face_with_clockwise_vu(f[k], i, adj[i][j], vertices)==1){
                    // cout<<"FACE:"<<endl;
                    // print_1d(f[k]);
                    // cout<<i<<"->"<<adj[i][j]<<"= clockwise"<<endl;
                    vector<size_t> parent1(vertices.size(),-1);
                    vector<size_t> ldfs_tree=ldfs_call(vertices, i, adj[i][j], adj, parent1);
                    // cout<<"LDFSTREE:"<<endl;
                    // print_1d(ldfs_tree);
                    vector<size_t> parent2(vertices.size(),-1);
                    vector<size_t> rdfs_tree=rdfs_call(vertices, i, adj[i][j], adj, parent2);
                    // cout<<"RDFSTREE:"<<endl;
                    // print_1d(rdfs_tree);
                    vector<size_t> common_points=findCommonPoints(ldfs_tree, rdfs_tree, i);
                    // cout<<"COMMONPOINTS:"<<endl;
                    // print_1d(common_points);
                    
                    for(int ab=0; ab<common_points.size(); ab++){
                        vector<size_t> path_ldfs= findPath(i, common_points[ab], parent1);
                        path_ldfs.pop_back();
                        reverse(path_ldfs.begin(), path_ldfs.end());
                        
                        vector<size_t> path_rdfs= findPath(i, common_points[ab], parent2);
                        path_rdfs.pop_back();
                        reverse(path_rdfs.begin(), path_rdfs.end());
                        int check=0;
                        for(int i=0; i<path_ldfs.size(); i++){
                            if(path_ldfs[i]!=path_rdfs[i]){
                                check=1;
                            }
                        }
                        // if(findCommonPoints(path_ldfs,path_rdfs,i).size()==1 && findCommonPoints(path_ldfs,path_rdfs,i)[0]==common_points[ab] && (path_ldfs.size()>2 || path_rdfs.size()>2)){
                        if(check==1){
                            cout<<"Simple List Superface:"<<endl;
                            // cout<<"P1:"<<i<<"->"<<common_points[ab]<<endl;
                            print_1d(path_ldfs);
                            // cout<<"P2:"<<i<<"->"<<common_points[ab]<<endl;
                            print_1d(path_rdfs);
                        }
                           
                        // }
                    }
                    

                }
                else{
                    // print_1d(f[k]);
                    // cout<<i<<"->"<<adj[i][j]<<"= anticlockwise"<<endl;
                }
                }
            }
        }
    }
    // cout<<" 1->3:" << face_with_clockwise_vu(face, 1, 3, vertices)<<endl;
    // cout<<" 3->1:" << face_with_clockwise_vu(face, 3, 1, vertices)<<endl;
    return 0;
}