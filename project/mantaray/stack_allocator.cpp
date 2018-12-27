#include <stack_allocator.h>

#include <malloc.h>

manta::StackAllocator::StackAllocator() {
	m_buffer = nullptr;
	m_stackPointer = nullptr;

	m_allocationLedger = 0;
}

manta::StackAllocator::~StackAllocator() {
	assert(m_allocationLedger == 0);

	::free(m_buffer);
}

void manta::StackAllocator::initialize(unsigned int size) {
	m_size = size;

	m_buffer = malloc(size);
	m_stackPointer = m_buffer;
	m_allocationLedger = 0;
}
