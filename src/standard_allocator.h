using namespace std;

struct StandardAllocator {
	int* allocate(int k) {
		if (k != sizeof(int)) return nullptr;
		return new int; 
	}

	void deallocate(int* ptr) {
		if (! ptr) return;
		delete ptr;
	}

	void reset() {}
};
