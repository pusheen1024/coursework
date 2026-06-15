using namespace std;

struct LinearAllocator {
	void* start; // адрес начала памяти, изначально отданной аллокатору
	int end; // размер
	int used; // сколько использовано

	LinearAllocator(int size) { 
		start = malloc(size);
		end = size;
		used = 0;
	}
	
	~LinearAllocator() {
		free(start);
	}
  
	void* allocate(int k) {
    	if (used + k > end) { // не хватает памяти для выделения
			return nullptr;
		}
    	void* ptr = (char*)(start) + used;
    	used += k;
    	return ptr;
	}

	void deallocate(void* ptr) {
	}

  	void reset() { // сброс всего аллокатора
    	used = 0;
  	}
};
