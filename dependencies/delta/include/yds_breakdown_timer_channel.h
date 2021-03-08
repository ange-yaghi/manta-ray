#ifndef YDS_BREAKDOWN_TIMER_CHANNEL_H
#define YDS_BREAKDOWN_TIMER_CHANNEL_H

#include "yds_base.h"

#include <string>

class ysBreakdownTimerChannel : public ysObject {
public:
    ysBreakdownTimerChannel();
    ~ysBreakdownTimerChannel();

    void Initialize(int bufferSize);
    void Reset();
    void Destroy();

    void SetName(const std::string &name) { m_name = name; }
    std::string GetName() const { return m_name; }

    int GetEntryCount() const { return m_entryCount; }
    uint64_t GetFrameCount() const { return m_frameCount; }

    void RecordSample(double s);
    double GetSample(int i) const;
    double GetLastSample() const;

    uint64_t GetLastMeasurementStart() const { return m_lastMeasurementStart; }

    void StartMeasurement(uint64_t timestamp);
    void EndMeasurement(uint64_t timestamp);

    bool IsMidMeasurement() const { return m_midMeasurement; }

protected:
    std::string m_name;
    double *m_sampleBuffer;
    int m_bufferSize;
    int m_currentWriteIndex;
    int m_entryCount;

    uint64_t m_frameCount;

    bool m_midMeasurement;

protected:
    uint64_t m_lastMeasurementStart;
};

#endif /* YDS_BREAKDOWN_TIMER_CHANNEL_H */
