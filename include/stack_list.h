#ifndef STACK_LIST_H
#define STACK_LIST_H

#include <stack_allocator.h>

#include <vector>
#include <assert.h>

namespace manta {

	template <typename T>
	class StackList {
	public:
		StackList() {
			m_stack = nullptr;
			m_list = nullptr;
			m_itemCount = 0;
			m_alignment = 1;
		}
		~StackList() {
			assert(m_list == nullptr);
			assert(m_itemCount == 0);
		}

		T *newItem() {
			T *newItem = nullptr;
			if (isUsingStack()) {
				void *buffer = m_stack->allocate(sizeof(T), m_alignment);
				newItem = new (buffer) T;

				if (m_itemCount == 0) m_list = newItem;
			}
			else {
				if (m_itemCount == 0) m_vector = new std::vector<T *>;

				void *buffer = _aligned_malloc(sizeof(T), m_alignment);
				newItem = new (buffer) T;
				m_vector->push_back(newItem);
			}
			m_itemCount++;

			return newItem;
		}

		void fillBuffer(T *buffer) {
			for (int i = 0; i < m_itemCount; i++) {
				buffer[i] = *getItem(i);
			}
		}

		void destroy() {
			if (isUsingStack()) {
				// Destroy the list in reverse order
				for (int i = m_itemCount - 1; i >= 0; i--) {
					m_stack->free((void *)&m_list[i]);
				}
				m_list = nullptr;
			}
			else {
				for (int i = 0; i < m_itemCount; i++) {
					m_vector->at(i)->~T();
					_aligned_free(m_vector->at(i));
				}
				delete m_vector;
				m_vector = nullptr;
			}

			m_itemCount = 0;
		}

		void setStack(StackAllocator *stack) { m_stack = stack; }
		StackAllocator *getStack() const { return m_stack; }
		inline bool isUsingStack() const { return m_stack != nullptr; }

		T *getItem(int i) {
			if (isUsingStack()) {
				return &m_list[i];
			}
			else {
				return m_vector->at(i);
			}
		}

		void setItem(int i, const T &item) {
			if (isUsingStack()) {
				m_list[i] = item;
			}
			else {
				*m_vector->at(i) = item;
			}
		}

		int getItemCount() const { return m_itemCount; }

		void setAlignment(int alignment) {
			m_alignment = alignment;
		}

		int getAlignment() {
			return m_alignment;
		}

		void swapElements(int i, int j) {
			const T *temp = getItem(i);
			setItem(i, *getItem(j));
			setItem(j, *temp);
		}

	protected:
		StackAllocator *m_stack;

		union {
			T *m_list;

			// If not using the stack, use a vector
			std::vector<T *> *m_vector;
		};

		int m_itemCount;
		int m_alignment;
	};

}

#endif /* STACK_LIST_H */
