#ifndef BETTER_BRUTE_H
#define BETTER_BRUTE_H

#include <vector>
#include <climits>
#include "common.h"

using namespace std;

// Algorithm 2: Better Brute Force
// Time Complexity: O(n^2)
// Space Complexity: O(1)
Result betterBruteForce(const vector<int>& arr) {
    Result r;
    r.maxSum = INT_MIN;
    r.startIdx = 0;
    r.endIdx = 0;

    if (arr.empty()) {
        r.maxSum = 0;
        r.startIdx = -1;
        r.endIdx = -1;
        return r;
    }

    for (int start = 0; start < (int)arr.size(); start++) {
        int currentSum = 0;

        for (int end = start; end < (int)arr.size(); end++) {
            currentSum += arr[end];

            if (currentSum > r.maxSum) {
                r.maxSum = currentSum;
                r.startIdx = start;
                r.endIdx = end;
            }
        }
    }

    return r;
}

#endif
