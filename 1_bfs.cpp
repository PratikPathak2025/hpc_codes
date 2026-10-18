// ============================================================================
// Program: Parallel Breadth-First Search (BFS) using OpenMP
// Description: Implements a level-synchronous parallel BFS traversal on an
//              undirected graph. Neighbors of nodes at the current level are
//              explored in parallel across multiple threads.
// ============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

int main() {
    // n = number of vertices, m = number of edges, source = starting node
    int n, m, source;
    cin >> n >> m >> source;

    // Adjacency list representation of the graph (1-based indexing)
    vector<vector<int>> adj(n + 1);

    // Read undirected edges and construct the adjacency list
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Visited array to keep track of visited vertices to prevent cycles
    vector<bool> visited(n + 1, false);
    // Queue for standard BFS traversal
    queue<int> q;

    // Initialize BFS with the source vertex
    q.push(source);
    visited[source] = true;

    // Level-by-level BFS traversal
    while (!q.empty()) {
        int size = q.size();

        // Stores all nodes belonging to the current BFS level
        vector<int> current_level;

        // Extract and display all nodes at the current level
        for (int i = 0; i < size; i++) {
            int node = q.front();
            q.pop();
            cout << node << " ";
            current_level.push_back(node);
        }

        // Stores neighbors discovered at the current level to form the next level
        vector<int> next_level;

        // Parallelize the exploration of neighbors across all nodes in the current level
        #pragma omp parallel for
        for (int i = 0; i < current_level.size(); i++) {
            int node = current_level[i];

            // Traverse all adjacent neighbors of the current node
            for (int j = 0; j < adj[node].size(); j++) {
                int neigh = adj[node][j];

                bool add = false;

                // Critical section: ensure mutually exclusive access to the visited array
                // to avoid race conditions when multiple threads check/update the same neighbor
                #pragma omp critical
                {
                    if (!visited[neigh]) {
                        visited[neigh] = true;
                        add = true;
                    }
                }

                // If this thread was the first to mark the neighbor visited, add it to next level
                if (add) {
                    // Critical section: vector push_back is not thread-safe
                    #pragma omp critical
                    next_level.push_back(neigh);
                }
            }
        }

        // Push all newly discovered nodes of the next level into the BFS queue
        for (int x : next_level) {
            q.push(x);
        }
    }

    return 0;
}

// Sample Input Format:
// 5 4 1       -> 5 nodes, 4 edges, start BFS from node 1
// 1 2         -> Edge between 1 and 2
// 1 3         -> Edge between 1 and 3
// 2 4         -> Edge between 2 and 4
// 3 5         -> Edge between 3 and 5
1
2
3
4
5
6
7
8
9