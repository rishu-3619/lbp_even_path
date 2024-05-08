#include<bits/stdc++.h>
using namespace std;


void dfs(int u, int dest, const vector<vector<int>>& adj, const unordered_set<int>& set_of_vertices, vector<bool>& visited, vector<int>& path, vector<int>& result) {
    visited[u] = true;
    path.push_back(u);

    if (u == dest) {
        result = path;
        return;
    }

    for (int v : adj[u]) {
        if (!visited[v] && set_of_vertices.count(v)) {
            dfs(v, dest, adj, set_of_vertices, visited, path, result);
        }
    }

    path.pop_back();
}

vector<int> findPath(int start, int dest, const vector<vector<int>>& adj, const unordered_set<int>& set_of_vertices) {
    vector<bool> visited(adj.size(), false);
    vector<int> path, result;
    
    dfs(start, dest, adj, set_of_vertices, visited, path, result);

    return result;
}

signed main(){
    
    return 0;
}