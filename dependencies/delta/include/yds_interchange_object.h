#ifndef YDS_INTERCHANGE_OBJECT_H
#define YDS_INTERCHANGE_OBJECT_H

#include "yds_math.h"

#include <vector>
#include <string>

struct ysInterchangeObject {
public:
    struct UVChannel {
        std::vector<ysVector2> Coordinates;
    };

    struct Light {
        int LightType;
        float Intensity;
        float CutoffDistance;
        float Distance;
        ysVector3 Color;

        float SpotAngularSize;
        float SpotFade;
    };

    struct IndexSet {
        union {
            struct {
                int x;
                int y;
                int z;
            };

            int indices[3];
        };
    };

    enum class ObjectType {
        Geometry,
        Bone,
        Group,
        Plane,
        Instance,
        Empty,
        Armature,
        Light,
        Undefined = -1
    };

    std::string Name;
    std::string MaterialName;
    ObjectType Type;
    int ModelIndex;
    int ParentIndex;
    int InstanceIndex;

    float Length;
    float Width;

    ysVector3 Position;
    ysVector3 OrientationEuler;
    ysVector4 Orientation;
    ysVector3 Scale;

    std::vector<ysVector3> Vertices;
    std::vector<UVChannel> UVChannels;
    std::vector<ysVector3> Normals;
    std::vector<ysVector3> Tangents;

    std::vector<IndexSet> VertexIndices;
    std::vector<IndexSet> NormalIndices;
    std::vector<std::vector<IndexSet>> UVIndices;
    std::vector<IndexSet> TangentIndices;

    Light LightInformation;

    bool Validate() const;

    void RipByIndexSet(std::vector<IndexSet> &indices);
    void RipByNormals();
    void RipByTangents();
    void RipByUVs();

    void UniformScale(float scale);
};

#endif /* YDS_INTERCHANGE_OBJECT_H */
