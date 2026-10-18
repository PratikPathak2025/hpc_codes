// ============================================================================
// Program: Parallel Reduction Operations (Min, Max, Sum, Average) using OpenMP
// Description: Demonstrates parallel statistical aggregations on an array
//              using OpenMP's `reduction` clause to avoid race conditions
//              and lock contention across threads.
// ============================================================================

#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>
using namespace std;

int main() {
    // Input vector of elements
    vector<int> arr = {5, 2, 9, 1, 7, 6, 8, 3, 4};

    // Initialize tracking variables with identity/boundary values
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    int sum = 0;

    // Parallel For Loop with Reduction Clauses:
    // - Each thread gets a private copy of min_val (init INT_MAX), max_val (init INT_MIN), and sum (init 0).
    // - Threads independently update their private copies without locking overhead.
    // - At loop completion, OpenMP automatically combines thread-private results into the global variables.
    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val) reduction(+:sum)
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < min_val) min_val = arr[i];
        if (arr[i] > max_val) max_val = arr[i];
        sum += arr[i];
    }

    // Compute average using the reduced sum
    double avg = (double)sum / arr.size();

    // Output aggregated statistical results
    cout << "Min: " << min_val << endl;
    cout << "Max: " << max_val << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;

    return 0;
}