//
// Created by ronald on 16/12/2024.
//
#ifndef MAXFLOW_H
#define MAXFLOW_H

#include <vector>
#include <tuple>

using namespace std;

bool bfs(const vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent);

int fordFulkerson(vector<vector<int>>& graph, int source, int sink, vector<vector<int>>& residualGraph);

vector<tuple<int, int, int>> findCriticalConnections(const vector<vector<int>>& originalGraph, const vector<vector<int>>& residualGraph, int vertices);

#endif //MAXFLOW_H
