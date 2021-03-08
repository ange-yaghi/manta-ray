#ifndef YDS_INTERCHANGE_FILE_0_0_H
#define YDS_INTERCHANGE_FILE_0_0_H

#include "yds_base.h"

#include "yds_interchange_object.h"
#include "yds_math.h"

#include <fstream>

class ysInterchangeFile0_0 : public ysObject {
public:
    static constexpr int MAJOR_VERSION = 0;
    static constexpr int MINOR_VERSION = 0;
    static constexpr int MAGIC_NUMBER = 0xFEA4A;

public:
    struct IdHeader {
        unsigned int MagicNumber;
        unsigned int MajorVersion;
        unsigned int MinorVersion;
        unsigned int EditorId;
        unsigned int CompilationStatus;
    };

    struct ObjectInformation {
        char Name[256];
        char MaterialName[256];
        int ModelIndex;
        int ParentIndex;
    };

    struct ObjectTransformation {
        ysVector3 Position;
        ysVector3 OrientationEuler;
        ysVector4 Orientation;
        ysVector3 Scale;
    };

    struct SceneHeader {
        unsigned int ObjectCount;
    };

    struct GeometryInformation {
        unsigned int UVChannelCount;

        unsigned int VertexCount;
        unsigned int NormalCount;
        unsigned int TangentCount;
        unsigned int FaceCount;
    };

    struct UVChannel {
        unsigned int UVCount;
    };

    struct IndexSet {
        int u, v, w;
    };

public:
    ysInterchangeFile0_0();
    ~ysInterchangeFile0_0();

    ysError Open(const char *fname);
    ysError Close();

    unsigned int GetMinorVersion() const { return m_minorVersion; }
    unsigned int GetMajorVersion() const { return m_majorVersion; }

    unsigned int GetObjectCount() const { return m_objectCount; }

    unsigned int GetToolId() const { return m_toolId; }
    bool GetCompilationStatus() const { return m_compilationStatus; }

    ysError ReadObject(ysInterchangeObject *object);

protected:
    std::fstream m_file;

    unsigned int m_minorVersion;
    unsigned int m_majorVersion;

    unsigned int m_toolId;

    unsigned int m_objectCount;

    bool m_compilationStatus;
};

#endif /* YDS_INTERCHANGE_FILE_0_0_H */
