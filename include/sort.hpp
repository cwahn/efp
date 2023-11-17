#ifndef SORT_HPP_
#define SORT_HPP_

#include <functional>

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

    // Insertion sort using a comparison function

    template <typename A, typename F = bool (*)(const A &, const A &)>
    void insertion_sort_by(Vector<A> &arr, const F &comp)
    {
        for (size_t i = 1; i < arr.size(); i++)
        {
            A key = std::move(arr[i]);
            int j = i - 1;

            while (j >= 0 && comp(key, arr[j]))
            {
                arr[j + 1] = std::move(arr[j]);
                j--;
            }
            arr[j + 1] = std::move(key);
        }
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

}

#endif