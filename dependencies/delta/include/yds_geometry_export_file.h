#ifndef YDS_GEOMETRY_EXPORT_FILE_H
#define YDS_GEOMETRY_EXPORT_FILE_H

#include "yds_base.h"

#include "yds_object_data.h"
#include "yds_interchange_object.h"

#include <fstream>

class ysGeometryExportFile : public ysObject {
public:
    // Flags
    static const unsigned int MDF_NONE = 0x00;
    static const unsigned int MDF_BONES = 0x01;
    static const unsigned int MDF_NORMALS = 0x02;
    static const unsigned int MDF_TANGENTS = 0x04;
    static const unsigned int MDF_TEXTURE_DATA = 0x08;
    static const unsigned int MDF_ANIMATION_DATA = 0x10;

    struct VertexInfo {
        bool IncludeTangents = false;
        bool IncludeNormals = true;
        bool IncludeUVs = true;

        int UVChannels = 1;
    };

    struct ObjectOutputHeader {
        char ObjectName[64];
        char ObjectMaterial[64];

        int ModelIndex;
        int ParentIndex;
        int ParentInstanceIndex;
        int SkeletonIndex;
        int ObjectType;

        ysVector3 Position;
        ysVector3 OrientationEuler;
        ysVector4 Orientation;
        ysVector3 Scale;

        ysVector3 MinExtreme;
        ysVector3 MaxExtreme;

        int NumUVChannels;
        int NumVertices;
        int NumFaces;

        int NumBones;
        int MaxBonesPerVertex;

        unsigned int Flags;

        int VertexDataSize;
    };

public:
    ysGeometryExportFile();
    ~ysGeometryExportFile();

    ysError Open(const char *fname);
    void Close();

    ysError WriteCustomData(void *data, int size);
    ysError WriteObject(ysObjectData *object);
    ysError WriteObject(ysInterchangeObject *object, const VertexInfo *info = nullptr);

protected:
    int GetVertexSize(ysInterchangeObject *object, const VertexInfo *info);

    void WriteIntToBuffer(int value, char **buffer);
    void WriteFloatToBuffer(float value, char **buffer);

    int PackVertexData(ysObjectData *object, int maxBonesPerVertex, void **output);
    int PackVertexData(ysInterchangeObject *object, int maxBonesPerVertex, void **output, const VertexInfo *info);
    void FillOutputHeader(ysObjectData *object, ObjectOutputHeader *header);
    void FillOutputHeader(const ysInterchangeObject *object, const VertexInfo *info, ObjectOutputHeader *header);

protected:
    std::ofstream m_file;
};

#endif /* YDS_GEOMETRY_EXPORT_FILE_H */
