//
// Created by ronald on 16/12/2024.
//

#include "graphutils.h"

void sortConnections(vector<tuple<int, int, int>>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (get<2>(arr[j]) < get<2>(arr[j + 1])) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}
