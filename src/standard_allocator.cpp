#include <standard_allocator.h>

#include <malloc.h>

manta::StandardAllocator *manta::StandardAllocator::s_global = nullptr;

manta::StandardAllocator *manta::StandardAllocator::Global() {
	if (s_global == nullptr) {
		return s_global = new StandardAllocator;
	}
	else {
		return s_global;
	}
}

manta::StandardAllocator::StandardAllocator() {
	m_allocationLedger = 0;

	m_maxUsage = 0;
	m_currentUsage = 0;
}

manta::StandardAllocator::~StandardAllocator() {
	assert(m_allocationLedger == 0);
	assert(m_currentUsage == 0);
}

void manta::StandardAllocator::initialize() {
	m_allocationLedger = 0;

	m_maxUsage = 0;
	m_currentUsage = 0;
}
