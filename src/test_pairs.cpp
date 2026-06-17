#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>
#include <cstdlib>

#include "list.h"
#include "pool_allocator.h"
#include "column_allocator.h"
#include "standard_allocator.h"

mt19937 rnd(time(NULL));
enum AllocType { Pool, Column };

void test(ColumnAllocator& allocator, int n, chrono::duration<double> &overhead, AllocType type) {
    pair<int*, int*> array[n];
    for (int i = 0; i < n; i++) {
        array[i].first = allocator.allocate_x(sizeof(int));
		array[i].second = allocator.allocate_y(sizeof(int));
  		if (array[i].first) {
			*array[i].first = 5 * i - 6;
		}
  		if (array[i].second) {
			*array[i].second = 6 * i + 7;
		}
    }
	auto start = chrono::high_resolution_clock::now();
  	vector<int> order(n);
	iota(order.begin(), order.end(), 0);
	shuffle(order.begin(), order.end(), rnd);
	auto end = chrono::high_resolution_clock::now();
	overhead = end - start;
	/*for (int x : order) {
		allocator.deallocate_x(array[x].first);
	}
	for (int x : order) {
		allocator.deallocate_y(array[x].second);
	}*/
	for (int x : order) {
		allocator.deallocate_x(array[x].first);
		allocator.deallocate_y(array[x].second);
	}
}

void test(PoolAllocator& allocator, int n, chrono::duration<double> &overhead, AllocType type) {
    pair<int*, int*> array[n];
    for (int i = 0; i < n; i++) {
        array[i] = make_pair((int*)allocator.allocate(sizeof(int)),
							 (int*)allocator.allocate(sizeof(int)));
        if (array[i].first && array[i].second) {
			*array[i].first = 5 * i - 6;
			*array[i].second = 6 * i + 7;
		}
    }
	auto start = chrono::high_resolution_clock::now();
  	vector<int> order(n);
	iota(order.begin(), order.end(), 0);
	shuffle(order.begin(), order.end(), rnd);
	auto end = chrono::high_resolution_clock::now();
	overhead = end - start;
	for (int x : order) {
		allocator.deallocate(array[x].first);
	}
	for (int x : order) {
		allocator.deallocate(array[x].second);
	}
	/*for (int x : order) {
		allocator.deallocate(array[x].first);
		allocator.deallocate(array[x].second);
	}*/
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
	const int N = 500000;
	cout << "Пулловый аллокатор:" << '\n';
   	PoolAllocator pool_allocator(sizeof(int), 2 * N);
	eval_time(pool_allocator, N, Pool);
	cout << '\n';

	cout << "Колоночный аллокатор:" << '\n';
   	ColumnAllocator column_allocator(sizeof(int), N);
	eval_time(column_allocator, N, Column);
	cout << '\n';
}
