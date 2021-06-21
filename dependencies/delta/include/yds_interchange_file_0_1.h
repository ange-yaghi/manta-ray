#ifndef YDS_INTERCHANGE_FILE_0_1_H
#define YDS_INTERCHANGE_FILE_0_1_H

#include "yds_base.h"

#include "yds_interchange_object.h"
#include "yds_math.h"

#include <fstream>

class ysInterchangeFile0_1 : public ysObject {
public:
    static constexpr int MAJOR_VERSION = 0x00;
    static constexpr int MINOR_VERSION = 0x01;
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
        int InstanceIndex;
        int ObjectType;
    };

    struct ObjectTransformation {
        ysVector3 Position;
        ysVector3 OrientationEuler;
        ysVector4 Orientation;
        ysVector3 Scale;
    };

    struct LightInformation {
        int LightType;
        float Intensity;
        float CutoffDistance;
        float Distance;
        ysVector3 Color;

        float SpotAngularSize;
        float SpotFade;
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
    ysInterchangeFile0_1();
    ~ysInterchangeFile0_1();

    ysError Open(const char *fname);
    ysError Close();

    unsigned int GetMinorVersion() const { return m_minorVersion; }
    unsigned int GetMajorVersion() const { return m_majorVersion; }

    unsigned int GetObjectCount() const { return m_objectCount; }

    unsigned int GetToolId() const { return m_toolId; }
    bool GetCompilationStatus() const { return m_compilationStatus; }

    ysError ReadObject(ysInterchangeObject *object);

    static ysInterchangeObject::ObjectType InterpretType(int i);

protected:
    std::fstream m_file;

    unsigned int m_minorVersion;
    unsigned int m_majorVersion;

    unsigned int m_toolId;

    unsigned int m_objectCount;

    bool m_compilationStatus;
};

#endif /* YDS_INTERCHANGE_FILE_0_1_H */

