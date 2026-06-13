struct LinearAllocator {
	void* start; // адрес начала памяти, изначально отданной аллокатору
	int end;
	int used;

	LinearAllocator(int size) { // конструктор
		start = malloc(size);
		end = size;
		used = 0;
	}
	
	~LinearAllocator() { // деструктор
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

  	void free() { // сброс всего аллокатора
    	used = 0;
  	}
};
