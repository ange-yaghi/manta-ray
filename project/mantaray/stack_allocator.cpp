#include <stack_allocator.h>

#include <malloc.h>

manta::StackAllocator::StackAllocator() {
	m_buffer = nullptr;
	m_stackPointer = nullptr;

	m_size = 0;

	m_allocationLedger = 0;
	m_maxStackPointer = nullptr;
}

manta::StackAllocator::~StackAllocator() {
	assert(m_allocationLedger == 0);

	// TODO: explicit release of memory is preferable
	if (m_buffer != nullptr) {
		::free(m_buffer);
	}
}

void manta::StackAllocator::initialize(mem_size size) {
	m_size = size;

	m_buffer = malloc(size);
	m_stackPointer = m_buffer;
	m_allocationLedger = 0;
}
