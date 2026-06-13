using namespace std;

struct list { // односвязный список 
	list* next;
}

template<typename T>
struct PoolAllocator {
	list* free_list; // список свободных блоков
	void* pool; // адрес начала памяти, изначально отданной аллокатору
	int block_count; // количество блоков
	int block_size; // размер одного блока

	PoolAllocator(int object_size, int n) { // конструктор
		block_size = max(object_size, sizeof(next));
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

	~PoolAllocator() { // деструктор
		free(pool);
	}

	void* allocate() {
		if (! free_list) return nullptr;
		list* block = free_list;
		free_list = free_list->next;
		return block;
	}

	void deallocate(void* ptr) {
		if (! ptr) return;
		if ((ptr - pool) % block_size != 0) return;
		list* block = (list*)ptr;
		block->next = free_list;
		free_list = block;
	}
};

