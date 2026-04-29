#ifndef PREFIX_SUM_H
#define PREFIX_SUM_H

#include "common.h"
#include <climits>
using namespace std;

Result prefixSum(const vector<int>& arr) {
    int n = arr.size();
    vector<int> pre(n + 1, 0);

    for (int i = 0; i < n; i++)
        pre[i + 1] = pre[i] + arr[i];

    Result r;
    r.maxSum   = INT_MIN;
    r.startIdx = 0;
    r.endIdx   = 0;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j <= n; j++) {
            int sum = pre[j] - pre[i];
            if (sum > r.maxSum) {
                r.maxSum   = sum;
                r.startIdx = i;
                r.endIdx   = j - 1;
            }
        }
    }
    return r;
}

#endif