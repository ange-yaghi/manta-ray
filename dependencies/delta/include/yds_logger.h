#ifndef YDS_LOGGER_H
#define YDS_LOGGER_H

#include "yds_dynamic_array.h"
#include "yds_logger_output.h"

class ysLoggerMessageLevel
{

public:

    ysLoggerMessageLevel();
    ysLoggerMessageLevel(const char *name, int level);
    ~ysLoggerMessageLevel();

    char m_name[256];
    int m_level;
    bool m_valid;

};

class ysLogger
{

protected:

    static ysLogger *g_instance;

public:

    static void LoggerCreate(ysLogger *logger) { g_instance = logger; g_instance->Initialize(); }
    static ysLogger *Logger() { return g_instance; }

public:

    ysLogger();
    ~ysLogger();

    template<typename LoggerType>
    LoggerType *NewLoggerOutput()
    {

        LoggerType *newOutput = m_loggerOutputs.NewGeneric<LoggerType>();
        newOutput->SetLogger(this);

        return newOutput;

    }

    void LogMessage(const char *message, const char *fname, int line, int level, ...);

    void AddMessageLevel(int level, const char *name);

    // Get the name corresponding to a level
    const char *GetLevelName(int level) { return m_messageLevels[level].m_name; }

    // Start the logging session
    void Start();

    // End the logging session
    void End();

protected:

    ysDynamicArray<ysLoggerOutput, 4> m_loggerOutputs;
    ysLoggerMessageLevel m_messageLevels[256];

    void Initialize();

};

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef YS_ENABLE_LOGGING

#define ysLogMessage(message, level, ...) \
    ysLogger::Logger()->LogMessage(message, __FILENAME__, __LINE__, level, __VA_ARGS__);

#else

#define ysLogMessage(message, level) ;

#endif

#endif