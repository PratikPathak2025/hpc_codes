// ============================================================================
// Program: Sequential and Parallel Merge Sort using OpenMP
// Description: Compares standard sequential Merge Sort with a parallel version
//              implemented using the OpenMP tasking paradigm (#pragma omp task
//              and #pragma omp taskwait).
// ============================================================================

#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

/* Merge Function: Combines two sorted subarrays arr[l..m] and arr[m+1..r] */
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1; // Size of left subarray
    int n2 = r - m;     // Size of right subarray

    // Temporary vectors to hold the subarrays
    vector<int> L(n1), R(n2);

    // Copy data to temporary vectors L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary vectors back into arr[l..r]
    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    // Copy any remaining elements of L[]
    while (i < n1)
        arr[k++] = L[i++];

    // Copy any remaining elements of R[]
    while (j < n2)
        arr[k++] = R[j++];
}

/* Sequential Merge Sort: Standard recursive divide-and-conquer */
void sequential_merge_sort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        // Recursively sort first and second halves sequentially
        sequential_merge_sort(arr, l, m);
        sequential_merge_sort(arr, m + 1, r);
        // Merge sorted halves
        merge(arr, l, m, r);
    }
}

/* Parallel Merge Sort Utility: Uses OpenMP task parallelism */
void parallel_merge_sort_util(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;

        // Spawn a distinct asynchronous task to sort the left half
        #pragma omp task shared(arr)
        parallel_merge_sort_util(arr, l, m);

        // Spawn another distinct asynchronous task to sort the right half
        #pragma omp task shared(arr)
        parallel_merge_sort_util(arr, m + 1, r);

        // taskwait: Barrier ensuring both child tasks finish before merging
        #pragma omp taskwait
        merge(arr, l, m, r);
    }
}

/* Wrapper for Parallel Merge Sort */
void parallel_merge_sort(vector<int>& arr) {
    // Create a team of parallel threads
    #pragma omp parallel
    {
        // Only one thread creates the initial root task; others pick up spawned tasks
        #pragma omp single
        parallel_merge_sort_util(arr, 0, arr.size() - 1);
    }
}

/* Main Function */
int main() {
    // Initial unsorted array
    vector<int> arr = {5, 2, 9, 1, 7, 6, 8, 3, 4};
    vector<int> arr_seq = arr;
    vector<int> arr_par = arr;

    double start, end;

    // --- Benchmark Sequential Merge Sort ---
    start = omp_get_wtime();
    sequential_merge_sort(arr_seq, 0, arr_seq.size() - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << endl;

    // Print Sequential Sorted Array
    for (int x : arr_seq) cout << x << " ";
    cout << endl;

    // --- Benchmark Parallel Merge Sort ---
    start = omp_get_wtime();
    parallel_merge_sort(arr_par);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << endl;

    // Print Parallel Sorted Array
    for (int x : arr_par) cout << x << " ";
    cout << endl;

    return 0;
}
