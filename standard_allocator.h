using namespace std;

struct StandardAllocator {
	int* allocate() {
		return new int; 
	}

	void deallocate(int* ptr) {
		if (! ptr) return;
		delete ptr;
	}
};

