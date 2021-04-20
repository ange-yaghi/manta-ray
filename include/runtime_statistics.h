#ifndef MANTARAY_RUNTIME_STATISTICS_H
#define MANTARAY_RUNTIME_STATISTICS_H

#include "manta_build_conf.h"

// Definitions for detailed statistics gathering
#if ENABLE_DETAILED_STATISTICS
#define STATISTICS_PROTOTYPE , RuntimeStatistics *stats
#define STATISTICS_PARAM_INPUT , stats
#define STATISTICS_NULL_INPUT , nullptr
#define STATISTICS_ROOT(stats) , (stats)
#define INCREMENT_COUNTER(counter) if (stats != nullptr) { stats->incrementCounter((counter)); }
#define INCREMENT_COUNTER_EXPLICIT(counter, amount) if (stats != nullptr) { stats->incrementCounter((counter), (amount)); }
#else
#define STATISTICS_PROTOTYPE
#define STATISTICS_PARAM_INPUT
#define INCREMENT_COUNTER(counter)
#define INCREMENT_COUNTER_EXPLICIT(counter, amount)
#define STATISTICS_NULL_INPUT
#define STATISTICS_ROOT(stats)
#endif /* ENABLE_DETAILED_STATISTICS */

namespace manta {

    struct RuntimeStatistics {
        enum class Counter {
            // Counters
            TriangleTests,
            UnecessaryTriangleTests,
            KdLeafNodeTraversals,
            KdEmptyLeafNodeTraversals,
            KdInnerNodeTraversals,
            QuadTests,
            RaysCast,
            TotalBvTests,
            TotalBvHits,

            // Special label for counter count
            Count
        };

        // Increment an individual counter
        inline void incrementCounter(Counter counter) { counters[(int)counter]++; }
        inline void incrementCounter(Counter counter, int amount) { counters[(int)counter] += amount; }
        void reset() {
            for (int i = 0; i < (int)Counter::Count; i++) {
                counters[i] = 0;
            }
        }

        void add(const RuntimeStatistics *r) {
            for (int i = 0; i < (int)Counter::Count; i++) {
                counters[i] += r->counters[i];
            }
        }

        const char *getCounterName(Counter counter) const {
            switch (counter) {
            case Counter::TriangleTests:
                return "TRIANGLE TESTS";
            case Counter::QuadTests:
                return "QUAD TESTS";
            case Counter::RaysCast:
                return "RAYS CAST";
            case Counter::TotalBvHits:
                return "TOTAL BV HITS";
            case Counter::TotalBvTests:
                return "TOTAL BV TESTS";
            case Counter::UnecessaryTriangleTests:
                return "UNNECESSARY TRIANGLE TESTS";
            case Counter::KdLeafNodeTraversals:
                return "KD LEAF NODE TRAVERSALS";
            case Counter::KdInnerNodeTraversals:
                return "KD INNER NODE TRAVERSALS";
            case Counter::KdEmptyLeafNodeTraversals:
                return "KD EMPTY LEAF NODE TRAVERSALS";
            default:
                return "UNKNOWN COUNTER";
            }
        }

        // All counters
        unsigned __int64 counters[(int)Counter::Count];
    };

} /* namespace manta */

#endif /* MANTARAY_RUNTIME_STATISTICS_H */
