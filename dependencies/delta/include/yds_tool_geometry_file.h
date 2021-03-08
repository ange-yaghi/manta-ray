#ifndef YDS_TOOL_GEOMETRY_FILE_H
#define YDS_TOOL_GEOMETRY_FILE_H

#include "yds_base.h"

#include "yds_object_data.h"

#include <fstream>

class ysToolGeometryFile : public ysObject {
public:
    // Tool File Headers
    struct HeaderVersion000 {
        char AnimationData;
        char BoneData;
        char TextureCoordinateData;
        char MaterialDataCheck;
        char SmoothingDataCheck;

        unsigned int ObjectCount;
    };

    struct ObjectInformation_000_002 {
        int ModelIndex;
        int ParentIndex;
        ysObjectData::ObjectType ObjectType;
        int UsesBones;
    };

    struct ObjectInformation_003_004 {
        int ModelIndex;
        int ParentIndex;
        int ParentInstance;
        ysObjectData::ObjectType ObjectType;
        int UsesBones;
    };

    struct ObjectInformation_005 {
        int ModelIndex;
        int ParentIndex;
        int ParentInstance;
        ysObjectData::ObjectType ObjectType;
        int UsesBones;
        int SkeletonIndex;
    };

    struct ObjectTransformation_000_003 {
        ysVector3 Position;
        ysVector3 Orientation;
        ysVector3 Scale;
    };

    struct ObjectTransformation_004_005 {
        ysVector3 Position;
        ysVector3 OrientationEuler;
        ysVector4 Orientation;
        ysVector3 Scale;
    };

    struct ObjectStatistics_000_005 {
        int NumUVChannels;
        int NumVertices;
        int NumFaces;
    };

    // Keep track of allocations
    class Allocation : public ysDynamicArrayElement {
    public:
        void *m_allocation;
    };

    // File Information
    static const int CURRENT_FILE_VERSION = 5;
    static const unsigned int MAGIC_NUMBER = 0x50F1A;

    // Software codes
    enum class EditorId {
        Undefined = 0x0,

        Autodesk3dsMax = 0x1,
        DeltaStudiosCompiler = 0x2,
        Blender = 0x3,

        Count
    };

    // Compilation status
    enum class CompilationStatus {
        Undefined = 0x0,
        Raw = 0x1,
        Compiled = 0x2,

        Count
    };

public:
    ysToolGeometryFile();
    ~ysToolGeometryFile();

    ysError Open(const char *fname);
    ysError Close();

    int GetFileVersion() const { return m_fileVersion; }
    int GetObjectCount() const;
    EditorId GetLastEditor() const { return m_lastEditor; }
    CompilationStatus GetCompilationStatus() const { return m_compilationStatus; }

    bool MaterialData() const;
    bool SmoothingData() const;

    ysError ReadObject(ysObjectData **object);
    ysError UpdateCompilationStatus(CompilationStatus status);

protected:
    ysError ReadHeader(int fileVersion);
    ysError ReadString(char *dest);

    ysError ReadObjectVersion000(ysObjectData *object);
    ysError ReadObjectVersion001(ysObjectData *object);
    ysError ReadObjectVersion002(ysObjectData *object);
    ysError ReadObjectVersion003(ysObjectData *object);
    ysError ReadObjectVersion004(ysObjectData *object);
    ysError ReadObjectVersion005(ysObjectData *object);

    ysError ReadObjectDataVersion000_005(ysObjectData *object);

    template<typename Type>
    Type *Allocate(int count) {
        Type *ret = new Type[count];
        Allocation *track = m_allocationTracker.New();
        track->m_allocation = (void *)ret;

        return ret;
    }

    void DestroyMemory();
    void ReleaseMemory();

protected:
    void *m_header;
    int m_fileVersion;
    EditorId m_lastEditor;
    CompilationStatus m_compilationStatus;

    // File
    std::fstream m_file;

    ysDynamicArray<Allocation, 4> m_allocationTracker;
};

#endif /* YDS_TOOL_GEOMETRY_FILE_H */
