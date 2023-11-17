#ifndef SOrightT_HPP_
#define SOrightT_HPP_

#include "sequence.hpp"

// todo Need to make it work for static sequences as well
// todo Make member function version of it

namespace efp
{
    template <typename A>
    void swap(A &a, A &b)
    {
        A temp = efp::move(a);
        a = efp::move(b);
        b = efp::move(temp);
    }

    // Merge function for merge sort
    template <typename A, typename F>
    void merge(Vector<A> &arr, int left, int middle, int right, const F &comp)
    {
        int n1 = middle - left + 1;
        int n2 = right - middle;

        Vector<A> lefts, rights;

        for (int i = 0; i < n1; i++)
            lefts.push_back(arr[left + i]);

        for (int j = 0; j < n2; j++)
            rights.push_back(arr[middle + 1 + j]);

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2)
        {
            if (comp(lefts[i], rights[j]))
            {
                arr[k] = lefts[i++];
            }
            else
            {
                arr[k] = rights[j++];
            }
            k++;
        }

        while (i < n1)
            arr[k++] = lefts[i++];

        while (j < n2)
            arr[k++] = rights[j++];
    }

    // Merge sort using a comparison function
    template <typename A, typename F>
    void merge_sort_by(Vector<A> &arr, int left, int right, const F &comp)
    {
        if (left < right)
        {
            int middle = left + (right - left) / 2;
            merge_sort_by(arr, left, middle, comp);
            merge_sort_by(arr, middle + 1, right, comp);
            merge(arr, left, middle, right, comp);
        }
    }

    // Function to create a max heap using a comparison function

    template <typename A, typename F = bool (*)(const A &, const A &)>
    void max_heapify_by(Vector<A> &arr, int n, int i, const F &comp)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && comp(arr[largest], arr[left]))
            largest = left;

        if (right < n && comp(arr[largest], arr[right]))
            largest = right;

        if (largest != i)
        {
            swap(arr[i], arr[largest]);
            max_heapify_by(arr, n, largest, comp);
        }
    }

    // Heapsort using a comparison function

    template <typename A, typename F = bool (*)(const A &, const A &)>
    void heapsort_by(Vector<A> &arr, const F &comp)
    {
        int n = arr.size();
        for (int i = n / 2 - 1; i >= 0; i--)
        {
            max_heapify_by(arr, n, i, comp);
        }
        for (int i = n - 1; i >= 0; i--)
        {
            swap(arr[0], arr[i]);
            max_heapify_by(arr, i, 0, comp);
        }
    }

    // More specific insertion sort that sorts part of the vector from `left` to `right`
    template <typename A, typename F>
    void insertion_sort_by_(Vector<A> &arr, int left, int right, const F &comp)
    {
        for (int i = left + 1; i <= right; i++)
        {
            A key = efp::move(arr[i]);
            int j = i - 1;

            while (j >= left && comp(key, arr[j]))
            {
                arr[j + 1] = efp::move(arr[j]);
                j--;
            }
            arr[j + 1] = efp::move(key);
        }
    }

    // General-purpose insertion sort that sorts the entire vector
    template <typename A, typename F = bool (*)(const A &, const A &)>
    void insertion_sort_by(Vector<A> &arr, const F &comp)
    {
        // Call the more specific insertion sort for the entire range of the vector
        insertion_sort_by_(arr, 0, arr.size() - 1, comp);
    }

    // Quicksort using a comparison function

    template <typename A, typename F = bool (*)(const A &, const A &)>
    int partition_by(Vector<A> &arr, int low, int high, const F &comp)
    {
        A pivot = arr[high];
        int i = (low - 1);

        for (int j = low; j < high; j++)
        {
            if (comp(arr[j], pivot))
            {
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return (i + 1);
    }

    template <typename A, typename F = bool (*)(const A &, const A &)>

    void quicksort_by(Vector<A> &arr, int low, int high, const F &comp)
    {
        if (low < high)
        {
            int pi = partition_by(arr, low, high, comp);
            quicksort_by(arr, low, pi - 1, comp);
            quicksort_by(arr, pi + 1, high, comp);
        }
    }

    // Introsort using a comparison function

    template <typename A, typename F = bool (*)(const A &, const A &)>
    void introsort_by(Vector<A> &arr, const F &comp)
    {
        if (arr.size() <= 1)
            return;
        quicksort_by(arr, 0, arr.size() - 1, comp);
        insertion_sort_by(arr, comp);
    }

    // Function to merge two sorted sub-vectors
    template <typename A, typename F>
    void timsort_merge(Vector<A> &arr, int start, int mid, int end, const F &comp)
    {
        int start2 = mid + 1;

        // If the direct merge is already sorted
        if (!comp(arr[mid], arr[start2]))
            return;

        // Two pointers to maintain start of both arrays to merge
        while (start <= mid && start2 <= end)
        {
            // If element 1 is in right place
            if (!comp(arr[start2], arr[start]))
            {
                start++;
            }
            else
            {
                A value = arr[start2];
                int index = start2;

                // Shift all the elements between element 1 and element 2, right by 1.
                while (index != start)
                {
                    arr[index] = arr[index - 1];
                    index--;
                }
                arr[start] = value;

                // Update all the pointers
                start++;
                mid++;
                start2++;
            }
        }
    }

    // Iterative Timsort function to sort the array[0...n-1] (similar to Python's and Java's)
    template <typename A, typename F = bool (*)(const A &, const A &)>
    void timsort_by(Vector<A> &arr, const F &comp)
    {
        int n = arr.size();
        const int RUN = 32; // This is a chosen run size.

        // Sort individual sub-arrays of size RUN
        for (int i = 0; i < n; i += RUN)
            insertion_sort_by_(arr, i, min((i + 31), (n - 1)), comp);

        // Start merging from size RUN. It will merge to form size 64, then 128, 256 and so on...
        for (int size = RUN; size < n; size = 2 * size)
        {
            // Pick starting point of left sub array. We are going to merge arr[left..left+size-1]
            // and arr[left+size, left+2*size-1]. After every merge, we increase left by 2*size
            for (int left = 0; left < n; left += 2 * size)
            {
                // Find ending point of left sub array. mid+1 is starting point of right sub array
                int mid = left + size - 1;
                int right = min((left + 2 * size - 1), (n - 1));

                // Merge sub array arr[left.....mid] & arr[mid+1....right]
                if (mid < right)
                {
                    timsort_merge(arr, left, mid, right, comp);
                }
            }
        }
    }

    // Default sort functions that call the sort_by functions with the default less-than comparison

    template <typename A>
    void quicksort(Vector<A> &arr)
    {
        quicksort_by(arr, 0, arr.size() - 1, efp::lt_v<A>);
    }

    template <typename A>
    void heapsort(Vector<A> &arr)
    {
        heapsort_by(arr, efp::lt_v<A>);
    }

    template <typename A>
    void insertion_sort(Vector<A> &arr)
    {
        insertion_sort_by(arr, efp::lt_v<A>);
    }

    template <typename A>
    void introsort(Vector<A> &arr)
    {
        introsort_by(arr, efp::lt_v<A>);
    }

    template <typename A>
    void timsort(Vector<A> &arr)
    {
        timsort_by(arr, efp::lt_v<A>);
    }

    // General sort

    template <typename A, typename F>
    void sort_by(Vector<A> &arr, const F &comp)
    {
        timsort_by(arr, comp);
    }

    template <typename A, typename F>
    void sort_unstable_by(Vector<A> &arr, const F &comp)
    {
        introsort_by(arr, comp);
    }

    template <typename A>
    void sort(Vector<A> &arr)
    {
        timsort_by(arr, efp::lt_v<A>);
    }

    template <typename A>
    void sort_unstable(Vector<A> &arr)
    {
        introsort_by(arr, efp::lt_v<A>);
    }

}

#endif