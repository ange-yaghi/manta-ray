#ifndef PIRANHA_STANDARD_ALLOCATOR_H
#define PIRANHA_STANDARD_ALLOCATOR_H

#include <assert.h>
#include <new>

#include <iostream>

#define STD_ALLOC_ENABLE_LEDGER (true)

namespace piranha {

    class StandardAllocator {
    public:
        StandardAllocator();
        ~StandardAllocator();

        void initialize();

        template <typename t_alloc>
        t_alloc *allocate(unsigned int n = 1, unsigned int alignment = 1) {
#ifdef STD_ALLOC_ENABLE_LEDGER
            m_allocationLedger++;
#endif /* STD_ALLOC_ENABLE_LEDGER */

            unsigned int usage = sizeof(t_alloc) * n;
            m_currentUsage += usage;
            if (m_currentUsage > m_maxUsage) m_maxUsage = m_currentUsage;
            t_alloc *newObject;

            if (alignment == 1) {
                if (n == 1) {
                    newObject = new t_alloc;
                }
                else {
                    newObject = new t_alloc[n];
                }
            }
            else {
                void *buffer = _aligned_malloc(sizeof(t_alloc) * n, alignment);
                if (n == 1) {
                    newObject = new (buffer) t_alloc;
                }
                else {
                    newObject = (t_alloc *)buffer;
                    for (unsigned int i = 0; i < n; i++) {
                        new ((t_alloc *)buffer + i) t_alloc;
                    }
                }
            }

            return newObject;
        }

        template <typename t_alloc>
        void free(t_alloc *memory, int n = 1) {
            if (memory == nullptr) return;

#ifdef STD_ALLOC_ENABLE_LEDGER
            m_allocationLedger--;
            assert(m_allocationLedger >= 0);
#endif /* STD_ALLOC_ENABLE_LEDGER */
            assert(m_currentUsage >= sizeof(t_alloc) * n);

            m_currentUsage -= sizeof(t_alloc) * n;

            if (n == 1) {
                delete memory;
            }
            else {
                delete[] memory;
            }
        }

        template <typename t_alloc>
        void aligned_free(t_alloc *memory, int n = 1) {
            if (memory == nullptr) return;

#ifdef STD_ALLOC_ENABLE_LEDGER
            m_allocationLedger--;
            assert(m_allocationLedger >= 0);
#endif /* STD_ALLOC_ENABLE_LEDGER */
            assert(m_currentUsage >= sizeof(t_alloc));

            m_currentUsage -= sizeof(t_alloc) * n;

            for (int i = 0; i < n; i++) {
                memory[i].~t_alloc();
            }

            _aligned_free((void *)memory);
        }

        unsigned int getMaxUsage() const { return m_maxUsage; }

        int getLedger() const { return m_allocationLedger; }
        unsigned int getCurrentUsage() const { return m_currentUsage; }

    protected:
        // Statistics counters
        int m_allocationLedger;
        unsigned int m_currentUsage;
        unsigned int m_maxUsage;

    public:
        // Singleton implementation
        static StandardAllocator *s_global;
        static StandardAllocator *Global();
    };

} /* namespace piranha */

#endif /* PIRANHA_STANDARD_ALLOCATOR_H */
