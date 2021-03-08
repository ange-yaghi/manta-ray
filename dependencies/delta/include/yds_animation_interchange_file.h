#ifndef YDS_ANIMATION_INTERCHANGE_FILE_0_0_H
#define YDS_ANIMATION_INTERCHANGE_FILE_0_0_H

#include "yds_base.h"

#include "yds_animation_curve.h"
#include "yds_animation_interchange_file_reader.h"
#include "yds_math.h"

#include <fstream>

class ysAnimationAction;

class ysAnimationInterchangeFile : public ysObject {
public:
    static constexpr int MAGIC_NUMBER = 0xFEA4AA;

public:
    struct IdHeader {
        unsigned int MagicNumber;
        unsigned int MajorVersion;
        unsigned int MinorVersion;
        unsigned int EditorId;
        unsigned int CompilationStatus;
    };

public:
    ysAnimationInterchangeFile();
    virtual ~ysAnimationInterchangeFile();

    ysError Open(const char *fname);
    ysError Close();

    unsigned int GetMinorVersion() const { return m_minorVersion; }
    unsigned int GetMajorVersion() const { return m_majorVersion; }

    unsigned int GetActionCount() const;

    unsigned int GetToolId() const { return m_toolId; }
    bool GetCompilationStatus() const { return m_compilationStatus; }

    ysError ReadAction(ysAnimationAction *action);

protected:
    ysAnimationInterchangeFileReader *m_reader;
    std::fstream m_file;

    unsigned int m_minorVersion;
    unsigned int m_majorVersion;

    unsigned int m_toolId;

    bool m_compilationStatus;
};

#endif /* YDS_ANIMATION_INTERCHANGE_FILE_0_0_H */

