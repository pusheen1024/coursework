#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>

#include "pool_allocator.h"
#include "standard_allocator.h"
#include "linear_allocator.h"

mt19937 rnd(time(NULL));

template<typename Allocator>
void test(Allocator& allocator, int n, bool lifo_order, bool linear, chrono::duration<double> &overhead) {
    int* array[n];
    for (int i = 0; i < n; i++) {
        array[i] = (int*)allocator.allocate();
        if (array[i]) *array[i] = 6 * i + 7;
    }

	auto start = chrono::high_resolution_clock::now();
  	vector<int> order(n);
	iota(order.begin(), order.end(), 0);
	// для пуллового и стандартного аллокатора тестируем освобождение памяти в произвольном порядке
	if (! lifo_order) shuffle(order.begin(), order.end(), rnd);
	// для стекового аллокатора - в порядке, обратном выделению
	else reverse(order.begin(), order.end()); 
	auto end = chrono::high_resolution_clock::now();
	overhead = end - start; // лишние операции не участвует в измерении времени

	for (int x : order) {
		allocator.deallocate(array[x]);
	}
	if (linear) allocator.reset();
}

template<typename Allocator>
void eval_time(Allocator& allocator, int n, bool lifo_order, bool linear) {
	auto start = chrono::high_resolution_clock::now();
	chrono::duration<double> overhead;
	test(allocator, n, lifo_order, linear, overhead);
    auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> time = end - start - overhead;
	cout << "Выделение и освобождение памяти под " << n << " объектов потребовало " << time << '\n';
}

int main() {
	const int N = 1000000;

	cout << "Пулловый аллокатор:" << '\n';
   	PoolAllocator pool_allocator(sizeof(int), N);
	eval_time(pool_allocator, N, 0, 0);
	cout << '\n';
	
	cout << "Линейный аллокатор:" << '\n';
   	LinearAllocator linear_allocator(sizeof(int) * N);
	eval_time(linear_allocator, N, 1, 1);
	cout << '\n';

	cout << "Стандартный аллокатор:" << '\n';
   	StandardAllocator stand_allocator;
	eval_time(stand_allocator, N, 0, 0);
}
