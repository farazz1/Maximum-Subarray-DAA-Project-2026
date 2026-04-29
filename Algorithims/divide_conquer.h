// divide_conquer.h
#ifndef DIVIDE_CONQUER_H
#define DIVIDE_CONQUER_H

#include "common.h"

Result maxCrossing(const vector<int>& arr, int left, int mid, int right) {
    long long leftSum = LLONG_MIN, rightSum = LLONG_MIN;
    long long sum = 0;
    int leftIdx = mid, rightIdx = mid + 1;

    for (int i = mid; i >= left; i--) {
        sum += arr[i];
        if (sum > leftSum) {
            leftSum = sum;
            leftIdx = i;
        }
    }

    sum = 0;
    for (int i = mid + 1; i <= right; i++) {
        sum += arr[i];
        if (sum > rightSum) {
            rightSum = sum;
            rightIdx = i;
        }
    }

    return { leftSum + rightSum, leftIdx, rightIdx };
}

Result divideAndConquer(const vector<int>& arr, int left, int right) {
    if (left == right) return { arr[left], left, right };

    int mid = (left + right) / 2;

    Result leftResult  = divideAndConquer(arr, left, mid);
    Result rightResult = divideAndConquer(arr, mid + 1, right);
    Result crossResult = maxCrossing(arr, left, mid, right);

    if (leftResult.maxSum >= rightResult.maxSum && leftResult.maxSum >= crossResult.maxSum)
        return leftResult;
    else if (rightResult.maxSum >= leftResult.maxSum && rightResult.maxSum >= crossResult.maxSum)
        return rightResult;
    else
        return crossResult;
}

// This is the wrapper the harness calls — must match this exact signature
Result divideAndConquer(const vector<int>& arr) {
    return divideAndConquer(arr, 0, arr.size() - 1);
}

#endif