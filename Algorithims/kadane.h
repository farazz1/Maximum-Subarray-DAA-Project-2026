#ifndef KADANE_H
#define KADANE_H

#include <vector>
#include <climits>
#include <algorithm>
#include "common.h"

using namespace std;

// Algorithm 4: Kadane's Algorithm
// Time Complexity: O(n)
// Space Complexity: O(1)
Result kadane(const vector<int>& arr) {
    Result r;

    if (arr.empty()) {
        r.maxSum = 0;
        r.startIdx = -1;
        r.endIdx = -1;
        return r;
    }

    int currentSum = arr[0];
    int bestSum = arr[0];
    int currentStart = 0;
    int bestStart = 0;
    int bestEnd = 0;

    for (int i = 1; i < (int)arr.size(); i++) {
        if (arr[i] > currentSum + arr[i]) {
            currentSum = arr[i];
            currentStart = i;
        } else {
            currentSum += arr[i];
        }

        if (currentSum > bestSum) {
            bestSum = currentSum;
            bestStart = currentStart;
            bestEnd = i;
        }
    }

    r.maxSum = bestSum;
    r.startIdx = bestStart;
    r.endIdx = bestEnd;
    return r;
}

#endif
