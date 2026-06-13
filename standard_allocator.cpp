using namespace std;

template<typename T>
struct PoolAllocator {
	T* allocate() {
		return new T();
	}

	void deallocate(T* ptr) {
		if (! ptr) return;
		delete ptr;
		ptr = nullptr;
	}
};

