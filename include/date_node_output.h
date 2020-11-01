#ifndef MANTARAY_DATE_NODE_OUTPUT_H
#define MANTARAY_DATE_NODE_OUTPUT_H

#include <piranha.h>

#include <time.h>

namespace manta {

    class DateNodeOutput : public piranha::NodeOutput {
    public:
        static const piranha::ChannelType DateType;

    public:
        DateNodeOutput();
        virtual ~DateNodeOutput();

        virtual void fullCompute(void *target) const;

        void setTime(const time_t &t) { m_time = t; }
        time_t getTime() const { return m_time; }

    protected:
        time_t m_time;

    protected:
        virtual piranha::Node *newInterface(piranha::NodeAllocator *nodeAllocator);
    };

} /* namespace manta */

#endif /* MANTARAY_DATE_NODE_OUTPUT_H */
