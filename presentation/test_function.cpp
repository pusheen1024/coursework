enum AllocType { Linear, Stack, Pool, Standard };

template<typename Allocator>
void test(Allocator& allocator, int n,
		  chrono::duration<double> &overhead,
		  AllocType type)
