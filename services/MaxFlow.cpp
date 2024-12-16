//
// Created by ronald on 16/12/2024.
//

#include "MaxFlow.h"
#include <queue>
#include <climits>

bool bfs(const vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent) {
    int n = residualGraph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                if (v == sink) {
                    parent[v] = u;
                    return true;
                }
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }
    return false;
}

int fordFulkerson(vector<vector<int>>& graph, int source, int sink, vector<vector<int>>& residualGraph) {
    int u, v;
    int n = graph.size();
    residualGraph = graph;
    vector<int> parent(n);
    int maxFlow = 0;

    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        for (v = sink; v != source; v = parent[v]) {
            u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }
    return maxFlow;
}

vector<tuple<int, int, int>> findCriticalConnections(const vector<vector<int>>& originalGraph, const vector<vector<int>>& residualGraph, int vertices) {
    vector<tuple<int, int, int>> criticalConnections;

    for (int u = 1; u <= vertices; u++) {
        for (int v = 1; v <= vertices; v++) {
            if (originalGraph[u][v] > 0 && residualGraph[u][v] == 0) {
                criticalConnections.push_back(make_tuple(u, v, originalGraph[u][v]));
            }
        }
    }
    return criticalConnections;
}