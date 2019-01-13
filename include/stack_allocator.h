#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

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

		void initialize(unsigned int size);

		void *allocate(unsigned int size, unsigned int alignment = 1) {
#if ENABLE_LEDGER
			m_allocationLedger++;
#endif /* ENABLE_LEDGER */

			void *realAddress = m_stackPointer;

			// Find an aligned address
			unsigned __int64 stackPtr = (unsigned __int64)m_stackPointer;
			unsigned int mod = (alignment - stackPtr % alignment) % alignment;

			void *basePointer = (void *)((char *)m_stackPointer + mod);
			void *newObject = basePointer;

#if ENABLE_BOUNDS_CHECKING
			assert(((unsigned __int64)((char *)newObject) - ((unsigned __int64)(char *)m_buffer) + size) <= m_size);

			if (((unsigned __int64)((char *)newObject) - ((unsigned __int64)(char *)m_buffer) + size) > m_size) {
				std::cout << "Out of memory! " << m_allocationLedger << std::endl;
			}
#endif

			// Update previous block pointers
			m_stackPointer = (void *)((char *)basePointer + size);

			if ((unsigned __int64)m_stackPointer > (unsigned __int64)m_maxStackPointer) m_maxStackPointer = m_stackPointer;

			return newObject;
		}

		void free(void *memory) {
#if ENABLE_LEDGER
			m_allocationLedger--;
			assert(m_allocationLedger >= 0);
#endif

#if ENABLE_BOUNDS_CHECKING
			assert((unsigned __int64)memory < (unsigned __int64)m_stackPointer);
#endif

			// Return the stack pointer to the beginning of the block
			m_stackPointer = memory;
		}

		unsigned int getMaxUsage() const { return (unsigned int)((unsigned __int64)m_maxStackPointer - (unsigned __int64)m_buffer); }

	protected:
		void *m_buffer;
		void *m_stackPointer;

		unsigned int m_size;

		// Statistics counters
		int m_allocationLedger;
		void *m_maxStackPointer;
	};

}

#endif /* STACK_ALLOCATOR_H */
