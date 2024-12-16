#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <tuple>

using namespace std;

void sort(vector<tuple<int, int, int>>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (get<2>(arr[j]) < get<2>(arr[j + 1])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

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

int main() {
    int vertices, edges;
    cin >> vertices >> edges;

    vector<int> demand(vertices + 1, 0);
    vector<vector<int>> graph(vertices + 2, vector<int>(vertices + 2, 0));
    vector<vector<int>> originalGraph(vertices + 2, vector<int>(vertices + 2, 0));
    vector<vector<int>> residualGraph(vertices + 2, vector<int>(vertices + 2, 0));

    int source = 0;
    int sink = vertices + 1;

    int totalDemand = 0;

    for (int i = 0; i < vertices; i++) {
        int id, demandValue;
        cin >> id >> demandValue;
        demand[id] = demandValue;
        if (demandValue == 0) {
            graph[source][id] = INT_MAX;
        } else {
            graph[id][sink] = demandValue;
            totalDemand += demandValue;
        }
    }

    for (int i = 0; i < edges; i++) {
        int u, v, capacity;
        cin >> u >> v >> capacity;
        graph[u][v] = capacity;
        originalGraph[u][v] = capacity;
    }

    int maxFlow = fordFulkerson(graph, source, sink, residualGraph);

    int unmetDemand = max(0, totalDemand - maxFlow);

    int energySupplied = 0;
    int energyLost = 0;

    for (int i = 1; i <= vertices; i++) {
        if (demand[i] == 0) {
            for (int j = 0; j < vertices + 2; j++) {
                if (graph[i][j] > 0) {
                    energySupplied += graph[i][j];
                    energyLost += graph[i][j];
                }
            }
        }
    }

    energyLost -= maxFlow;

    vector<tuple<int, int, int>> criticalConnections = findCriticalConnections(originalGraph, residualGraph, vertices);

    sort(criticalConnections);

    cout << maxFlow << endl
    << unmetDemand << endl
    << energyLost << endl
    << criticalConnections.size() << endl;

    for (const auto& connection : criticalConnections) {
        int u, v, cap;
        tie(u, v, cap) = connection;
        cout << u << " " << v << " " << cap << endl;
    }

    return 0;
}
