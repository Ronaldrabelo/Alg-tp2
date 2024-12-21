#include <iostream>
#include <vector>
#include <tuple>
#include <climits>
#include "services/MaxFlow.h"
#include "utils/GraphUtils.h"

using namespace std;

int main() {
    int vertices, edges;
    cin >> vertices >> edges;

    vector<int> demand(vertices + 1, 0);
    vector<vector<int>> graph(vertices + 2, vector<int>(vertices + 2, 0));
    vector<vector<int>> originalGraph(vertices + 2, vector<int>(vertices + 2, 0));
    vector<vector<int>> residualGraph(vertices + 2, vector<int>(vertices + 2, 0));

    int source = 0, sink = vertices + 1, totalDemand = 0;

    for (int i = 0; i < vertices; i++) {
        int id, demandValue;
        cin >> id >> demandValue;
        demand[id] = demandValue;
        if (demandValue == 0) graph[source][id] = INT_MAX;
        else {
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

    int energySupplied = 0, energyLost = 0;

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

    sortConnections(criticalConnections);

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
