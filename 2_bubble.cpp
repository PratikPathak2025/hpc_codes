// ============================================================================
// Program: Parallel Bubble Sort (Odd-Even Transposition Sort) using OpenMP
// Description: Standard bubble sort has sequential dependencies between adjacent
//              swaps. Odd-Even Transposition Sort resolves this by alternating
//              between even and odd phases where disjoint pairs are compared
//              and swapped concurrently across threads.
// ============================================================================

#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

// Odd-Even Transposition Sort implementation
void parallel_bubble_sort(vector<int>& arr) {
    int n = arr.size();

    // The algorithm requires n phases to ensure full sorting in the worst case
    for (int phase = 0; phase < n; phase++) {

        // Even Phase: Compare and swap disjoint pairs starting at even indices: (0,1), (2,3), (4,5)...
        if (phase % 2 == 0) {
            #pragma omp parallel for
            for (int i = 0; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
            // An implicit barrier exists at the end of the OpenMP parallel for region
        } 
        // Odd Phase: Compare and swap disjoint pairs starting at odd indices: (1,2), (3,4), (5,6)...
        else {
            #pragma omp parallel for
            for (int i = 1; i < n - 1; i += 2) {
                if (arr[i] > arr[i + 1]) {
                    swap(arr[i], arr[i + 1]);
                }
            }
            // An implicit barrier exists at the end of the OpenMP parallel for region
        }
    }
}

int main() {
    // Sample input array
    vector<int> arr = {5, 2, 9, 1, 7, 6, 8, 3, 4};

    double start, end;

    // Measure execution time using omp_get_wtime()
    start = omp_get_wtime();
    parallel_bubble_sort(arr);
    end = omp_get_wtime();

    // Display performance metrics
    cout << "Parallel Bubble Sort Time: " << end - start << endl;

    // Display the sorted output
    for (int x : arr) cout << x << " ";
    cout << endl;

    return 0;
}
