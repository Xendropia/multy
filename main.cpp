#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <random>
#include <functional>
#include <mutex>

std::mutex mtx; // mutex for critical section
long long total_sum = 0; // global variable to store the total sum

void partial_sum(const std::vector<int>& arr, int start, int end) {
    long long sum = 0;
    for (int i = start; i < end; ++i) {
        sum += arr[i];
    }
    std::lock_guard<std::mutex> lock(mtx);
    total_sum += sum;
}

int main() {
    int N = 1000; // size of the array
    int M = 4; // number of parts (threads)
    
    // Generate a random array of size N
    std::vector<int> arr(N);
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    std::generate(arr.begin(), arr.end(), std::bind(dist, std::ref(rng)));

    std::vector<std::thread> threads;
    int part_size = N / M;

    for (int i = 0; i < M; ++i) {
        int start = i * part_size;
        int end = (i == M - 1) ? N : start + part_size;
        threads.emplace_back(partial_sum, std::ref(arr), start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Total sum: " << total_sum << std::endl;
    return 0;
}