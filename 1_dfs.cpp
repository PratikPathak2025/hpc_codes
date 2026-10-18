// ============================================================================
// Program: Parallel Depth-First Search (DFS) using OpenMP
// Description: Traverses an undirected graph using recursive DFS where outgoing
//              branches of a node are explored concurrently using OpenMP threads.
// ============================================================================

#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// Maximum number of vertices supported
const int MAXN = 100000;

// Graph representation: Adjacency list and visited status array
vector<int> adj[MAXN + 5];
bool visited[MAXN + 5];

// Recursive Depth-First Search with parallel branch exploration
void dfs(int node) {
    visited[node] = true;

    // Critical section to prevent concurrent threads from corrupting console output
    #pragma omp critical
    cout << node << " ";

    // Parallelize the traversal of outgoing edges / adjacent neighbors
    #pragma omp parallel for
    for (int i = 0; i < adj[node].size(); i++) {
        int next = adj[node][i];

        bool do_visit = false;

        // Critical section: atomic check-and-set on the visited array
        // Prevents multiple threads from simultaneously visiting the same neighbor
        #pragma omp critical
        {
            if (!visited[next]) {
                visited[next] = true;
                do_visit = true;
            }
        }

        // Recursively visit the neighbor if successfully claimed by this thread
        if (do_visit) {
            dfs(next);
        }
    }
}

int main() {
    // n = number of vertices, m = number of edges
    int n, m;
    cin >> n >> m;

    // Read edges and build undirected adjacency list
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Source vertex to initiate DFS traversal
    int start;
    cin >> start;

    // Execute parallel DFS starting from the source node
    dfs(start);

    return 0;
}

// Sample Input Format:
// 5 4         -> 5 nodes, 4 edges
// 1 2         -> Edge between 1 and 2
// 1 3         -> Edge between 1 and 3
// 2 4         -> Edge between 2 and 4
// 3 5         -> Edge between 3 and 5
// 1           -> Starting vertex for DFS
