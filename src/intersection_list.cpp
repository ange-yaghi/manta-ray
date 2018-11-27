#include <intersection_list.h>

#include <stack_allocator.h>

#include <assert.h>

manta::IntersectionList::IntersectionList() {
	m_stack = nullptr;
	m_list = nullptr;
	m_intersectionCount = 0;
}

manta::IntersectionList::~IntersectionList() {
	assert(m_list == nullptr);
	assert(m_intersectionCount == 0);
}

manta::CoarseIntersection * manta::IntersectionList::newIntersection() {
	CoarseIntersection *newIntersection = nullptr;
	if (isUsingStack()) {
		newIntersection = (CoarseIntersection *)m_stack->allocate(sizeof(CoarseIntersection));

		// If the list of intersections has not yet been made, set the beginning to be at the new intersection
		// NOTE - this is okay because the stack guarantees contiguous allocation
		if (m_intersectionCount == 0) m_list = newIntersection;
	}
	else {
		// If the list of intersections is not yet initialized, create the new vector
		if (m_intersectionCount == 0) 

		newIntersection = new CoarseIntersection;
		m_intersectionVector->push_back(newIntersection);
	}

	m_intersectionCount++;

	return newIntersection;
}

void manta::IntersectionList::destroy() {
	if (isUsingStack()) {
		// Destroy the list in reverse order
		for (int i = m_intersectionCount - 1; i >= 0; i--) {
			m_stack->free((void *)&m_list[i]);
		}
		m_list = nullptr;
	}
	else {
		for (int i = 0; i < m_intersectionCount; i++) {
			delete m_intersectionVector->at(i);
		}
		delete m_intersectionVector;
		m_intersectionVector = nullptr;
	}

	m_intersectionCount = 0;
}

manta::CoarseIntersection *manta::IntersectionList::getIntersection(int i) {
	if (isUsingStack()) {
		return &m_list[i];
	}
	else {
		return m_intersectionVector->at(i);
	}
}
