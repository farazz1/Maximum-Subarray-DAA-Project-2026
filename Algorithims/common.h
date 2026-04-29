#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <climits>

using namespace std;

// Every algorithm returns this struct
struct Result {
    long long maxSum;
    int startIdx;
    int endIdx;
};

#endif
