#ifndef YDS_LOGGER_OUTPUT_H
#define YDS_LOGGER_OUTPUT_H

#include "yds_base.h"

class ysLogger;

class ysLoggerOutput : public ysObject {
    friend ysLogger;

public:
    enum FORMAT_PARAMETER {
        LOG_DATE =        0x00000001,
        LOG_TIME =        0x00000002,
        LOG_MODULE =    0x00000004,
        LOG_LEVEL =        0x00000008,
        LOG_LINE =        0x00000010,

        LOG_DEFAULT =    LOG_DATE | LOG_TIME | LOG_MODULE | LOG_LEVEL | LOG_LINE,

        LOG_ALL =        LOG_DATE | LOG_TIME | LOG_MODULE | LOG_LEVEL | LOG_LINE,
        LOG_INVALID =    ~LOG_ALL,
    };

public:
    ysLoggerOutput();
    ysLoggerOutput(const char *typeID);
    ~ysLoggerOutput();

    /* Initialize the output */
    virtual void Initialize() = 0;

    // Close the output
    virtual void Close() = 0;

    // Enable or disable this output
    void SetEnable(bool enable) { m_enabled = enable; }

    // Get whether this output is enabled or disabled
    bool GetEnable() const { return m_enabled; }

    // Log a message
    void LogMessage(const char *message, const char *fname, int line, int level);

    // Set format parameters
    void SetFormatParameters(unsigned int formatParameters) { m_formatParameters = formatParameters; }

protected:
    // Write a message
    virtual void Write(const char *data) = 0;

    // Set the parent logger class used by this output
    void SetLogger(ysLogger *logger);

protected:
    // Flag indicating whether this output is enabled.
    bool m_enabled;

    // The minimum level required to log a message
    int m_level;

    // Output formatting parameters
    unsigned int m_formatParameters;

    // A reference to the higher level logger class
    ysLogger *m_parentLogger;

protected:
    // Clear the internal buffer
    void ClearBuffer();

    // Write to the internal buffer
    void WriteToBuffer(const char *data, int width = 0);

    // Interal buffer
    char m_buffer[1024];

    // Internal buffer length
    int m_bufferLength;
};

#endif /* YDS_LOGGER_OUTPUT_H */
