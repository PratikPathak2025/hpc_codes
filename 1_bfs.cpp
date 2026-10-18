// 1
// 2
// 3
// 4
// 5
// 6
// 7
// 8
// 9

#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

int main() {
    int n, m, source;
    cin >> n >> m >> source;

    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<bool> visited(n + 1, false);
    queue<int> q;

    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int size = q.size();

        vector<int> current_level;

        for (int i = 0; i < size; i++) {
            int node = q.front();
            q.pop();
            cout << node << " ";
            current_level.push_back(node);
        }

        vector<int> next_level;

        #pragma omp parallel for
        for (int i = 0; i < current_level.size(); i++) {
            int node = current_level[i];

            for (int j = 0; j < adj[node].size(); j++) {
                int neigh = adj[node][j];

                bool add = false;

                #pragma omp critical
                {
                    if (!visited[neigh]) {
                        visited[neigh] = true;
                        add = true;
                    }
                }

                if (add) {
                    #pragma omp critical
                    next_level.push_back(neigh);
                }
            }
        }

        for (int x : next_level) {
            q.push(x);
        }
    }

    return 0;
}

// INPUT
// 5 4 1
// 1 2
// 1 3
// 2 4
// 3 5