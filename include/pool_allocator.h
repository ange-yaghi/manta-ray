#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

namespace manta {

	template<typename T, int alignment=1>
	class PoolAllocator {
	public:
		PoolAllocator();
		~PoolAllocator();

		void initialize(unsigned int n);
		T *allocate();


	protected:
		T *m_pool;
		T **m_freeBlocks;

		int m_freeBlocks;
	};

}

#endif /* POOL_ALLOCATOR_H */
