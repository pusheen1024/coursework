using namespace std;

struct PoolAllocator {
	list* free_list; // список свободных блоков
	void* pool; // адрес начала памяти, изначально отданной аллокатору
	int block_count; // количество блоков
	int block_size; // размер одного блока

	PoolAllocator(int object_size, int n) {
		block_size = max<int>(object_size, sizeof(list));
		block_count = n;
		pool = malloc(block_size * n);
		free_list = (list*)pool;
		list* cur = free_list;
		for (int i = 0; i < n - 1; i++) {
			cur->next = (list*)((char*)cur + block_size);
			cur = cur->next;
		}
		cur->next = nullptr;
	}
	~PoolAllocator() {
		free(pool);
	}
	void* allocate(int k) {
		if (! free_list) return nullptr;
		list* block = free_list;
		free_list = free_list->next;
		return block;
	}
	void deallocate(void* ptr) {
		if (! ptr) return;
		if (((char*)ptr - (char*)pool) % block_size != 0) return;
		list* block = (list*)ptr;
		block->next = free_list;
		free_list = block;
	}
	void reset() {}
};
