struct ColumnAllocator {
	list* free_x; // список свободных блоков для координаты x
	list* free_y; // список свободных блоков для координаты y
	void* pool_x; // память под x
	void* pool_y; // память под y
	int block_count; // количество блоков
	int block_size; // размер одного блока

	ColumnAllocator(int object_size, int n) { // конструктор
		block_size = max<int>(object_size, sizeof(list));
		block_count = n;
		pool_x = malloc(block_size * n);
		pool_y = malloc(block_size * n);
		free_x = (list*)pool_x;
		free_y = (list*)pool_y;
		list* cur_x = free_x;
		list* cur_y = free_y;
		for (int i = 0; i < n - 1; i++) {
			cur_x->next = (list*)((char*)cur_x + block_size);
			cur_x = cur_x->next;
			cur_y->next = (list*)((char*)cur_y + block_size);
			cur_y = cur_y->next;
		}
		cur_x->next = nullptr;
		cur_y->next = nullptr;
	}

	~ColumnAllocator() { // деструктор
		free(pool_x);
		free(pool_y);
	}
	int* allocate_x(int k) {
		if (! free_x) return nullptr;
		list* block_x = free_x;
		free_x = free_x->next;
		return (int*)block_x;
	}
	void deallocate_x(int* ptr) {
		if (! ptr || ((char*)ptr - (char*)pool_x) % block_size != 0) return;
		list* block_x = (list*)ptr;
		block_x->next = free_x;
		free_x = block_x;
	}
	int* allocate_y(int k) {}
	void deallocate_y(int* ptr) {}
};
