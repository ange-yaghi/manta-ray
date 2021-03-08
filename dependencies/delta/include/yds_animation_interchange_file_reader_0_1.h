#ifndef YDS_ANIMATION_INTERCHANGE_FILE_READER_0_1_H
#define YDS_ANIMATION_INTERCHANGE_FILE_READER_0_1_H

#include "yds_animation_interchange_file_reader.h"

#include "yds_animation_curve.h"
#include "yds_math.h"

#include <fstream>

class ysAnimationAction;

class ysAnimationInterchangeFileReader_0_1 :
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

        unsigned InterpolationMode;

        float l_handle_x;
        float l_handle_y;

        float r_handle_x;
        float r_handle_y;
    };

public:
    ysAnimationInterchangeFileReader_0_1();
    ~ysAnimationInterchangeFileReader_0_1();

    virtual unsigned int GetActionCount() const { return m_actionCount; }

    virtual ysError ReadHeader(std::fstream &f);
    virtual ysError ReadAction(std::fstream &f, ysAnimationAction *action);

    static ysAnimationCurve::CurveType InterpretCurveType(unsigned int curveType);
    static ysAnimationCurve::CurveHandle::InterpolationMode
        InterpretInterpolationMode(unsigned int interpolationMode);

protected:
    unsigned int m_actionCount;
};

#endif /* YDS_ANIMATION_INTERCHANGE_FILE_READER_0_1_H */
