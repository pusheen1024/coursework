#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>
#include <cstdlib>

#include "pool_allocator.h"
#include "linear_allocator.h"
#include "stack_allocator.h"
#include "standard_allocator.h"

mt19937 rnd(time(NULL));
enum AllocType { Linear, Stack, Pool, Standard };

template<typename Allocator>
void test(Allocator& allocator, int n, chrono::duration<double> &overhead, AllocType type) {
    int* array[n];
    for (int i = 0; i < n; i++) {
        array[i] = (int*)allocator.allocate(sizeof(int));
        if (array[i]) *array[i] = 6 * i + 7;
    }
	auto start = chrono::high_resolution_clock::now();
  	vector<int> order(n);
	iota(order.begin(), order.end(), 0);
	// для пуллового и стандартного аллокатора тестируем освобождение памяти в произвольном порядке
	if (type == Pool || type == Standard) shuffle(order.begin(), order.end(), rnd);
	// для стекового аллокатора тестируем освобождение памяти в порядке, обратном выделению
	else if (type == Stack) reverse(order.begin(), order.end()); 
	auto end = chrono::high_resolution_clock::now();
	overhead = end - start; // лишние операции не участвуют в измерении времени

	for (int x : order) {
		allocator.deallocate(array[x]);
	}
	allocator.reset();
	// тестируем, что память была освобождена корректно
	array[10] = (int*)allocator.allocate(sizeof(int));
	if (array[10]) *array[10] = 1024;
}

template<typename Allocator>
void eval_time(Allocator& allocator, int n, AllocType type) {
	auto start = chrono::high_resolution_clock::now();
	chrono::duration<double> overhead;
	test(allocator, n, overhead, type);
    auto end = chrono::high_resolution_clock::now();
	chrono::duration<double> time = end - start - overhead;
	cout << "Выделение и освобождение памяти под " << n << " объектов потребовало " << time << '\n';
}

int main() {
	const int N = 1000000;

	cout << "Пулловый аллокатор:" << '\n';
   	PoolAllocator pool_allocator(sizeof(int), N);
	eval_time(pool_allocator, N, Pool);
	cout << '\n';
	
	cout << "Линейный аллокатор:" << '\n';
   	LinearAllocator linear_allocator(sizeof(int) * N);
	eval_time(linear_allocator, N, Linear);
	cout << '\n';

	cout << "Стековый аллокатор:" << '\n';
   	StackAllocator stack_allocator(sizeof(int) * N);
	eval_time(stack_allocator, N, Stack);
	cout << '\n';

	cout << "Стандартный аллокатор:" << '\n';
   	StandardAllocator stand_allocator;
	eval_time(stand_allocator, N, Standard);
}
