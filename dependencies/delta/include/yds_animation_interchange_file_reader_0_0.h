#ifndef YDS_ANIMATION_INTERCHANGE_FILE_READER_0_0_H
#define YDS_ANIMATION_INTERCHANGE_FILE_READER_0_0_H

#include "yds_animation_interchange_file_reader.h"

#include "yds_animation_curve.h"
#include "yds_math.h"

#include <fstream>

class ysAnimationAction;

class ysAnimationInterchangeFileReader_0_0 : 
    public ysAnimationInterchangeFileReader 
{
public:
    struct IdHeader {
        unsigned int MagicNumber;
        unsigned int MajorVersion;
        unsigned int MinorVersion;
        unsigned int EditorId;
        unsigned int CompilationStatus;
    };

    struct FileHeader {
        unsigned int ActionCount;
    };

    struct ActionHeader {
        char Name[256];
        unsigned int CurveCount;
    };

    struct CurveHeader {
        char TargetBone[256];
        unsigned int KeyframeCount;
        unsigned int CurveType;
    };

    struct Keyframe {
        float Timestamp;
        float Value;
    };

public:
    ysAnimationInterchangeFileReader_0_0();
    ~ysAnimationInterchangeFileReader_0_0();

    virtual unsigned int GetActionCount() const { return m_actionCount; }

    virtual ysError ReadHeader(std::fstream &f);
    virtual ysError ReadAction(std::fstream &f, ysAnimationAction *action);

    static ysAnimationCurve::CurveType InterpretCurveType(unsigned int curveType);

protected:
    unsigned int m_actionCount;
};

#endif /* YDS_ANIMATION_INTERCHANGE_FILE_READER_0_0_H */
