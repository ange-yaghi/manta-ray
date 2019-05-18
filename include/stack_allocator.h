#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include <memory_management.h>

#include <assert.h>
#include <new>

#include <iostream>

#define ENABLE_BOUNDS_CHECKING (false)
#define ENABLE_LEDGER (false)

namespace manta {

	class StackAllocator {
	public:
		StackAllocator();
		~StackAllocator();

		void initialize(mem_size size);

		void *allocate(mem_size size, unsigned int alignment = 1) {
#if ENABLE_LEDGER
			m_allocationLedger++;
#endif /* ENABLE_LEDGER */

			void *realAddress = m_stackPointer;

			// Find an aligned address
			mem_size stackPtr = (mem_size)m_stackPointer;
			unsigned int mod = (alignment - stackPtr % alignment) % alignment;

			void *basePointer = (void *)((char *)m_stackPointer + mod);
			void *newObject = basePointer;

#if ENABLE_BOUNDS_CHECKING
			bool outOfMemory =
				((mem_size)((char *)newObject) - ((mem_size)(char *)m_buffer) + size) <= m_size;
			assert(outOfMemory == false);

			if (((mem_size)((char *)newObject) - ((mem_size)(char *)m_buffer) + size) > m_size) {
				std::cout << "Out of memory! " << m_allocationLedger << std::endl;
			}
#endif

			// Update previous block pointers
			m_stackPointer = (void *)((char *)basePointer + size);

			if ((mem_size)m_stackPointer > (mem_size)m_maxStackPointer) 
				m_maxStackPointer = m_stackPointer;

			return newObject;
		}

		void free(void *memory) {
#if ENABLE_LEDGER
			m_allocationLedger--;
			assert(m_allocationLedger >= 0);
#endif

#if ENABLE_BOUNDS_CHECKING
			assert((mem_size)memory < (mem_size)m_stackPointer);
#endif

			// Return the stack pointer to the beginning of the block
			m_stackPointer = memory;
		}

		mem_size getMaxUsage() const { return ((mem_size)m_maxStackPointer - (mem_size)m_buffer); }

	protected:
		void *m_buffer;
		void *m_stackPointer;

		mem_size m_size;

		// Statistics counters
		int m_allocationLedger;
		void *m_maxStackPointer;
	};

} /* namespace manta */

#endif /* STACK_ALLOCATOR_H */
