using namespace std;

struct stack {
	int size;
	stack* prev;
};

struct StackAllocator {
	void* start; // адрес начала памяти, изначально отданной аллокатору
	int end; // размер
	int used; // сколько использовано
	stack* stack_pointer; // стек выделенных блоков (реализован на односвязном списке)

    StackAllocator(int size) {
        start = malloc(size);
        end = size;
        used = 0;
		stack_pointer = nullptr;
    }

    ~StackAllocator() {
        free(start);
    }

    void* allocate(int k) {
		int block_size = max<int>(k, sizeof(stack));
        if (used + k > end) {
			return nullptr;
        }
		void* ptr = (char*)start + used;
		stack* block = (stack*)ptr;
		block->size = block_size;
		block->prev = stack_pointer;
		stack_pointer = block;
		used += block_size;
		return ptr;
    }

    void deallocate(void* ptr) {
        if (!ptr) return;
		if (ptr != (char*)stack_pointer - stack_pointer->size) return;
		used -= stack_pointer->size;
		stack_pointer = stack_pointer->prev;
    }

	void reset() {
		used = 0;
		stack_pointer = nullptr;
	}
};
