/*
 * ============================================================
 * CSE 317: Algorithms – Design and Analysis
 * Algorithm 1: Brute Force (All Subarrays) — O(n³)
 * ============================================================
 * Design Technique : Exhaustive Search
 * Time Complexity  : O(n³)  — three nested loops
 * Space Complexity : O(1)   — no auxiliary memory
 * ============================================================
 *
 * HOW IT WORKS:
 *   Every contiguous subarray is defined by a start index i
 *   and end index j. We enumerate all (i,j) pairs and use a
 *   third inner loop k to sum elements i..j. We track the
 *   best sum seen across all subarrays.
 * ============================================================
 */

#ifndef BRUTE_FORCE_H
#define BRUTE_FORCE_H

#include <vector>
#include <climits>
#include "common.h"   // shared Result struct

using namespace std;

Result bruteForce(const vector<int>& arr) {
    int n = arr.size();
    Result best = { LLONG_MIN, 0, 0 };

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {

            long long currentSum = 0;
            for (int k = i; k <= j; k++) {   // ← third loop → O(n³)
                currentSum += arr[k];
            }

            if (currentSum > best.maxSum) {
                best.maxSum    = currentSum;
                best.startIdx  = i;
                best.endIdx    = j;
            }
        }
    }

    return best;
}

#endif
