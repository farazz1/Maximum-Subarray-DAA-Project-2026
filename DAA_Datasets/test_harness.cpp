#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>
#include <functional>
#include <cassert>
#include "common.h"
#include "brute_force.h"
#include "better_brute.h"
#include "divide_conquer.h"
#include "kadane.h"
#include "prefix_sum.h"

using namespace std;
using namespace chrono;

// ─────────────────────────────────────────────────────────────
// Algorithm registry
// ─────────────────────────────────────────────────────────────
struct Algorithm {
    string name;
    string complexity;
    function<Result(const vector<int>&)> run;
};

// ─────────────────────────────────────────────────────────────
// Dataset generation
// ─────────────────────────────────────────────────────────────
vector<int> genRandom(int n) {
    mt19937 rng(42);
    uniform_int_distribution<int> dist(-1000, 1000);
    vector<int> arr(n);
    for (int& x : arr) x = dist(rng);
    return arr;
}

vector<int> genAllNegative(int n) {
    mt19937 rng(99);
    uniform_int_distribution<int> dist(-1000, -1);
    vector<int> arr(n);
    for (int& x : arr) x = dist(rng);
    return arr;
}

vector<int> genAllPositive(int n) {
    mt19937 rng(7);
    uniform_int_distribution<int> dist(1, 1000);
    vector<int> arr(n);
    for (int& x : arr) x = dist(rng);
    return arr;
}

// ─────────────────────────────────────────────────────────────
// File I/O
// ─────────────────────────────────────────────────────────────
void saveDataset(const vector<int>& arr, const string& path) {
    ofstream f(path);
    f << arr.size() << "\n";
    for (int x : arr) f << x << " ";
    f << "\n";
    cout << "  Saved: " << path << "  (n=" << arr.size() << ")\n";
}

vector<int> loadDataset(const string& path) {
    ifstream f(path);
    if (!f.is_open()) {
        cerr << "ERROR: Cannot open " << path << "\n";
        cerr << "Run './harness --generate' first to create datasets.\n";
        exit(1);
    }
    int n; f >> n;
    vector<int> arr(n);
    for (int& x : arr) f >> x;
    return arr;
}

// ─────────────────────────────────────────────────────────────
// Dataset specs
// ─────────────────────────────────────────────────────────────
struct DatasetSpec {
    int    n;
    string type;
    string filename;
};

vector<int> TEST_SIZES = { 100, 500, 1000, 2000, 3000 };

vector<DatasetSpec> buildSpecs() {
    vector<DatasetSpec> specs;
    for (int n : TEST_SIZES) {
        specs.push_back({ n, "random",   "dataset_random_"   + to_string(n) + ".txt" });
        specs.push_back({ n, "negative", "dataset_negative_" + to_string(n) + ".txt" });
        specs.push_back({ n, "positive", "dataset_positive_" + to_string(n) + ".txt" });
    }
    return specs;
}

// ─────────────────────────────────────────────────────────────
// Generate datasets
// ─────────────────────────────────────────────────────────────
void generateDatasets() {
    cout << "\n=== Generating Shared Datasets ===\n";
    cout << "(Share these .txt files with ALL group members)\n\n";
    for (auto& spec : buildSpecs()) {
        vector<int> arr;
        if      (spec.type == "random")   arr = genRandom(spec.n);
        else if (spec.type == "negative") arr = genAllNegative(spec.n);
        else                              arr = genAllPositive(spec.n);
        saveDataset(arr, spec.filename);
    }
    cout << "\nDone. Copy ALL .txt files to every group member's machine.\n";
}

// ─────────────────────────────────────────────────────────────
// Benchmark
// ─────────────────────────────────────────────────────────────
struct Row {
    string    algoName;
    string    complexity;
    int       n;
    string    dataType;
    double    timeMs;
    long long maxSum;
    int       start, end;
};

Row runBenchmark(const Algorithm& algo, const vector<int>& arr,
                 int n, const string& dataType) {
    auto t0 = high_resolution_clock::now();
    Result r = algo.run(arr);
    auto t1 = high_resolution_clock::now();
    double ms = duration<double, milli>(t1 - t0).count();
    return { algo.name, algo.complexity, n, dataType, ms,
             r.maxSum, r.startIdx, r.endIdx };
}

// ─────────────────────────────────────────────────────────────
// Verification
// ─────────────────────────────────────────────────────────────
void verifyAgreement(const vector<Row>& rows, const string& outfile) {
    ofstream f(outfile);
    f << "Verification Report — all algorithms should agree on maxSum\n";
    f << string(60, '=') << "\n\n";
    bool allGood = true;
    auto specs = buildSpecs();

    for (auto& spec : specs) {
        vector<long long> sums;
        vector<string>    names;
        for (auto& r : rows) {
            if (r.n == spec.n && r.dataType == spec.type) {
                sums.push_back(r.maxSum);
                names.push_back(r.algoName);
            }
        }
        if (sums.empty()) continue;
        bool agree = true;
        for (size_t i = 1; i < sums.size(); i++)
            if (sums[i] != sums[0]) agree = false;

        f << "n=" << spec.n << "  type=" << spec.type << "\n";
        for (size_t i = 0; i < names.size(); i++)
            f << "  " << setw(30) << left << names[i] << " maxSum=" << sums[i] << "\n";
        f << "  Result: " << (agree ? "AGREE" : "MISMATCH — CHECK YOUR CODE") << "\n\n";
        if (!agree) allGood = false;
    }

    f << (allGood ? "ALL ALGORITHMS AGREE" : "MISMATCHES FOUND") << "\n";
    cout << "\nVerification: " << (allGood ? "All algorithms agree" : "MISMATCHES FOUND")
         << " — see " << outfile << "\n";
}

// ─────────────────────────────────────────────────────────────
// Print table
// ─────────────────────────────────────────────────────────────
void printTable(const vector<Row>& rows) {
    cout << "\n";
    cout << left
         << setw(28) << "Algorithm"
         << setw(14) << "Complexity"
         << setw(7)  << "n"
         << setw(12) << "Dataset"
         << setw(16) << "Time (ms)"
         << "Max Sum\n";
    cout << string(85, '-') << "\n";
    for (auto& r : rows) {
        cout << left
             << setw(28) << r.algoName
             << setw(14) << r.complexity
             << setw(7)  << r.n
             << setw(12) << r.dataType
             << setw(16) << fixed << setprecision(4) << r.timeMs
             << r.maxSum << "\n";
    }
}

// ─────────────────────────────────────────────────────────────
// Write CSV
// ─────────────────────────────────────────────────────────────
void writeCSV(const vector<Row>& rows, const string& path) {
    ofstream f(path);
    f << "algorithm,complexity,n,dataset_type,time_ms,max_sum,start_idx,end_idx\n";
    for (auto& r : rows) {
        f << r.algoName   << ","
          << r.complexity << ","
          << r.n          << ","
          << r.dataType   << ","
          << fixed << setprecision(6) << r.timeMs << ","
          << r.maxSum     << ","
          << r.start      << ","
          << r.end        << "\n";
    }
    cout << "Results saved to: " << path << "\n";
}

// ─────────────────────────────────────────────────────────────
// Correctness check
// ─────────────────────────────────────────────────────────────
void correctnessCheck(const vector<Algorithm>& algos) {
    vector<int> sample = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    long long expected = 6;
    cout << "\n=== Correctness Check ===\n";
    cout << "Input: [-2, 1, -3, 4, -1, 2, 1, -5, 4]  — expected max = 6\n\n";
    bool allPass = true;
    for (auto& algo : algos) {
        Result r = algo.run(sample);
        bool pass = (r.maxSum == expected);
        cout << "  " << setw(30) << left << algo.name
             << " -> " << r.maxSum
             << (pass ? "  PASS" : "  FAIL — WRONG ANSWER") << "\n";
        if (!pass) allPass = false;
    }
    if (!allPass) {
        cout << "\nFix failing algorithms before benchmarking.\n";
        exit(1);
    }
    cout << "\nAll passed!\n";
}

// ─────────────────────────────────────────────────────────────
// MAIN
// ─────────────────────────────────────────────────────────────
int main(int argc, char* argv[]) {

    vector<Algorithm> algorithms = {
        { "Brute Force (All Subarrays)", "O(n^3)",   bruteForce       },
        { "Better Brute Force",          "O(n^2)",   betterBruteForce },
        { "Prefix Sum",                  "O(n^2)",   prefixSum        },
        { "Divide & Conquer",            "O(nlogn)", [](const vector<int>& arr) {
            return divideAndConquer(arr);
        }},
        { "Kadane's Algorithm",          "O(n)",     kadane           },
    };

    if (argc > 1 && string(argv[1]) == "--generate") {
        generateDatasets();
        return 0;
    }

    correctnessCheck(algorithms);

    cout << "\n=== Benchmarking ===\n";
    auto specs = buildSpecs();
    vector<Row> results;

    for (auto& spec : specs) {
        vector<int> arr = loadDataset(spec.filename);
        for (auto& algo : algorithms) {
            cout << "  Running " << algo.name << "  n=" << spec.n
                 << "  [" << spec.type << "] ... " << flush;
            Row row = runBenchmark(algo, arr, spec.n, spec.type);
            cout << fixed << setprecision(2) << row.timeMs << " ms\n";
            results.push_back(row);
        }
    }

    printTable(results);
    writeCSV(results, "results.csv");
    verifyAgreement(results, "verification.txt");
    return 0;
}