#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>

#include "pool_allocator.h"
#include "standard_allocator.h"

mt19937 rnd(time(NULL));

void test(int n) {
    PoolAllocator allocator(sizeof(int), n);
	StandardAllocator stand_allocator;
    int* array[n];
    for (int i = 0; i < n; i++) {
        array[i] = (int*)allocator.allocate();
        if (array[i]) *array[i] = 6 * i + 7;
    }
	// освобождаем в произвольном порядке, раз пулловый аллокатор это позволяет
	vector<int> order(n);
	iota(order.begin(), order.end(), 0);
	shuffle(order.begin(), order.end(), rnd);
	for (int x : order) {
		allocator.deallocate(array[x]);
	}
}

void eval_time(int n) {
	auto start = chrono::high_resolution_clock::now();
	test(n);
    auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> time = end - start;
	cout << time << '\n';
}

int main() {
	const int N = 1000000;
	eval_time(N);
}
