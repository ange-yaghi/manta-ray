#ifndef YDS_OBJECT_DATA_H
#define YDS_OBJECT_DATA_H

#include "yds_math.h"

#include "yds_expanding_array.h"

class ysObjectData {
public:
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

    enum ANIMATION_KEY {
        ANIMATION_KEY_POSITION_X,
        ANIMATION_KEY_POSITION_Y,
        ANIMATION_KEY_POSITION_Z,

        ANIMATION_KEY_ROTATION_X,
        ANIMATION_KEY_ROTATION_Y,
        ANIMATION_KEY_ROTATION_Z,

        NUM_KEY_TYPES,
    };

    struct UVChannel {
        ysExpandingArray<ysVector2> m_coordinates;
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

    struct BoneWeights {
        ysExpandingArray<int> m_boneIndices;
        ysExpandingArray<float> m_boneWeights;
    };

    struct UVCoordinateIndexSet {
        ysExpandingArray<IndexSet> UVIndexSets;
    };

    struct AnimationKey {
        int Time;    // ie frame
        float Value;
    };

    struct KeySeries {
        ANIMATION_KEY Type;
        int NumKeys;

        AnimationKey *Keys;
    };

public:
    ysObjectData();
    ~ysObjectData();

    void Clear();

    // Header
    char m_name[64];
    char m_materialName[64];

    struct ObjectInformation {
        int ModelIndex;
        int ParentIndex;
        int ParentInstance;
        ObjectType ObjectType;
        int UsesBones;
        int SkeletonIndex;
    } m_objectInformation;

    struct ObjectTransformation {
        ysVector3 Position;
        ysVector3 OrientationEuler;
        ysVector4 Orientation;
        ysVector3 Scale;
    } m_objectTransformation;

    struct ObjectStatistics {
        int NumUVChannels;
        int NumVertices;
        int NumFaces;
    } m_objectStatistics;

    // Vertex Data
    ysExpandingArray<ysVector3> m_vertices;
    ysExpandingArray<int> m_materialList;
    ysExpandingArray<BoneWeights> m_boneWeights;

    // UV Data
    ysExpandingArray<UVChannel> m_channels;

    // Face Data
    ysExpandingArray<IndexSet> m_vertexIndexSet;
    ysExpandingArray<int> m_smoothingGroups;
    ysExpandingArray<int> m_extendedSmoothingGroups;
    int m_numExtendedSmoothingGroups;
    ysExpandingArray<UVCoordinateIndexSet> m_UVIndexSets;

    // Bone Indices
    ysExpandingArray<int> m_boneIndices;

    // Animation Data
    KeySeries m_animationKeySeries[NUM_KEY_TYPES];

    // Calculated Values
    ysExpandingArray<ysVector3> m_normals;
    ysExpandingArray<ysVector4> m_tangents;

    bool m_flipNormals;

    // Primitive Data
    union {
        float m_width;
        float m_radius;
    };

    float m_height;
    float m_length;

public:
    // DATA CACHE
    ysVector *m_hardNormalCache;
};

#endif /* YDS_OBJECT_DATA_H */
