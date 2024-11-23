#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>

static constexpr int START = 5000;
static constexpr int END = 100000;
static constexpr int STEP = 1000;

struct TestResult {
    int n;
    long long avg_time;
    TestResult(int n, long long avg_time): n(n), avg_time(avg_time) {}
};

std::ostream& operator <<(std::ostream& out, const TestResult& res) {
    out << res.n << " " << res.avg_time;
    return out;
}

class ArrayGenerator {
public:
    ArrayGenerator(int seed, int min, int max) : gen(seed), value_dist(min, max) {}

    int* generate_random_array(int n) {
        int* array = new int[n];
        for (int i = 0; i < n; ++i) {
            array[i] = value_dist(gen);
        }
        return array;
    }

    int* generate_reversed(int n) {
        int* array = generate_random_array(n);
        std::sort(array, array+n);
        std::reverse(array, array+n);
        return array;
    }

    int* generate_near_sorted(int n, int swap_cnt) {
        int* array = generate_random_array(n);
        std::sort(array, array+n);
        std::uniform_int_distribution<> dist(0, n-1);
        for (int i = 0; i < swap_cnt; ++i) {
            int g1 = dist(gen);
            int g2 = dist(gen);
            std::swap(array[g1], array[g2]);
        }
        return array;
    }

private:
    std::mt19937 gen;
    std::uniform_int_distribution<> value_dist;
};

void heapify(int* A, int i, int n) {
    int max_elem = i;
    int l_child = 2 * i + 1;
    int r_child = 2 * i + 2;
    if (l_child < n && A[l_child] > A[max_elem]) {
        max_elem = l_child;
    }
    if (r_child < n && A[r_child] > A[max_elem]) {
        max_elem = r_child;
    }
    if (max_elem != i) {
        std::swap(A[i], A[max_elem]);
        heapify(A, max_elem, n);
    }
}

void buildMaxHeap(int* first, int* last) {
    int n = last - first;
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(first, i, n);
    }
}

void heapSort(int* first, int* last) {
    int n = last - first;
    buildMaxHeap(first, last);
    for (int i = n - 1; i > 0; --i) {
        std::swap(first[0], first[i]);
        heapify(first, 0, i);
    }
}

void insertion_sort(int* first, int* last) {
    for (int* it = first + 1; it < last; ++it) {
        int* j = it;
        while (j > first && *j < *(j - 1)) {
            std::swap(*j, *(j-1));
            j--;
        }
    }
}

int partition(int* first, int* last) {
    static std::mt19937 gen(42);
    std::uniform_int_distribution<> d(0, last-first-1);
    int p_i = d(gen);
    int pivot = *(first + p_i);
    std::swap(*(first + p_i), *(last - 1));
    int i = -1;
    for (int* j = first; j < last - 1; ++j) {
        if (*j < pivot) {
            ++i;
            std::swap(*(first+i), *j);
        }
    }
    std::swap(*(first + i + 1), *(last-1));
    return i+1;
}

void quick_sort_impl(int* first, int* last, int deep, bool use_optimizations) {
    int len = last - first;
    int max_deep = 2 * std::log2(len);

    if (use_optimizations && len < 16) {
        insertion_sort(first, last);
        return;
    }

    if (use_optimizations && deep > max_deep) {
        heapSort(first, last);
        return;
    }

    if (first < last) {
        int pivot = partition(first, last);
        quick_sort_impl(first, first + pivot, deep + 1, use_optimizations);
        quick_sort_impl(first + pivot + 1, last, deep + 1, use_optimizations);
    }
}

void print_array(int* array, int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << array[i] << ' ';
    }
    std::cout << std::endl;
}

void write_into_file(std::string file_name, const std::vector<TestResult>& data) {
    std::ofstream out(file_name);
    for (const auto& res: data) {
        out << res << std::endl;
    }
}

void quick_sort(int* first, int* last) {
    quick_sort_impl(first, last, 0, false);
}

void intro_sort(int* first, int* last) {
    quick_sort_impl(first, last, 0, true);
}

enum gen_type{
    random,
    sort_reversed,
    part_sort
};

int* copy_mas(int* a, int n) {
    int* copied_mas = new int[n];
    for (int i = 0; i < n; ++i) {
        copied_mas[i] = a[i];
    }
    return copied_mas;
}

class SortTester {
public:
    SortTester(int start, int end, int step) : start(start), end(end), step(step) {}

    std::vector<TestResult> test(gen_type gt, void(*sort)(int*, int*)) {
        ArrayGenerator generator(42, 0, 6000);
        std::vector<TestResult> res;

        int* arr;
        for (int i = start; i <= end; i += step) {
            long long full_time = 0;
            if (gt == random) {
                arr = generator.generate_random_array(i);
            } else if (gt == sort_reversed) {
                arr = generator.generate_reversed(i);
            } else {
                arr = generator.generate_near_sorted(i, 100);
            }
            for (int j = 0; j < 10; ++j) {
                int* arr_copy = copy_mas(arr, i);
                auto start = std::chrono::high_resolution_clock::now();
                sort(arr_copy, arr_copy+i);
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long msec = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
                full_time += msec;
                delete[] arr_copy;
            }
            delete[] arr;
            res.emplace_back(i, full_time / 10);
        }
        return res;
    }
private:
    int start;
    int end;
    int step;
};

int main() {
    SortTester tester(START, END, STEP);
    auto q_s_test1 = tester.test(random, quick_sort);
    auto q_s_test2 = tester.test(sort_reversed, quick_sort);
    auto q_s_test3 = tester.test(part_sort, quick_sort);

    auto i_s_test1 = tester.test(random, intro_sort);
    auto i_s_test2 = tester.test(sort_reversed, intro_sort);
    auto i_s_test3 = tester.test(part_sort, intro_sort);

    write_into_file("q_s_test1.txt", q_s_test1);
    write_into_file("q_s_test2.txt", q_s_test2);
    write_into_file("q_s_test3.txt", q_s_test3);

    write_into_file("i_s_test1.txt", i_s_test1);
    write_into_file("i_s_test2.txt", i_s_test2);
    write_into_file("i_s_test3.txt", i_s_test3);

    return 0;
}