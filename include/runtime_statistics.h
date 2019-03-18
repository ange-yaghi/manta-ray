#ifndef RUNTIME_STATISTICS_H
#define RUNTIME_STATISTICS_H

#include <manta_build_conf.h>

// Definitions for detailed statistics gathering
#if ENABLE_DETAILED_STATISTICS
#define STATISTICS_PROTOTYPE , RuntimeStatistics *stats
#define STATISTICS_PARAM_INPUT , stats
#define STATISTICS_NULL_INPUT , nullptr
#define STATISTICS_ROOT(stats) , (stats)
#define INCREMENT_COUNTER(counter) if (stats != nullptr) { stats->incrementCounter((counter)); }
#else
#define STATISTICS_PROTOTYPE
#define STATISTICS_PARAM_INPUT
#define INCREMENT_COUNTER(counter)
#define STATISTICS_NULL_INPUT
#define STATISTICS_ROOT(stats)
#endif /* ENABLE_DETAILED_STATISTICS */

namespace manta {

	struct RuntimeStatistics {
		enum COUNTER {
			// Counters
			TRIANGLE_TESTS,
			QUAD_TESTS,
			RAYS_CAST,
			TOTAL_BV_TESTS,
			TOTAL_BV_HITS,

			// Special label for counter count
			COUNTER_COUNT
		};

		// Increment an individual counter
		inline void incrementCounter(COUNTER counter) { counters[(int)counter]++; }
		void reset() {
			for (int i = 0; i < COUNTER_COUNT; i++) {
				counters[i] = 0;
			}
		}

		void add(const RuntimeStatistics *r) {
			for (int i = 0; i < COUNTER_COUNT; i++) {
				counters[i] += r->counters[i];
			}
		}

		const char *getCounterName(COUNTER counter) const {
			switch (counter) {
			case TRIANGLE_TESTS:
				return "TRIANGLE TESTS";
			case QUAD_TESTS:
				return "QUAD TESTS";
			case RAYS_CAST:
				return "RAYS CAST";
			case TOTAL_BV_HITS:
				return "TOTAL BV HITS";
			case TOTAL_BV_TESTS:
				return "TOTAL BV TESTS";
			default:
				return "UNKNOWN COUNTER";
			}
		}

		// All counters
		unsigned __int64 counters[COUNTER_COUNT];
	};

} /* namespace manta */

#endif /* RUNTIME_STATISTICS_H */
