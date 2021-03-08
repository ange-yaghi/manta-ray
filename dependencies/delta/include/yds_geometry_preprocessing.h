#ifndef YDS_GEOMETRY_PREPROCESSING_H
#define YDS_GEOMETRY_PREPROCESSING_H

#include "yds_object_data.h"

namespace ysGeometryPreprocessing {

    bool ConnectedFaces(ysObjectData *object, int face1, int face2);
    bool SameSmoothingGroup(ysObjectData *object, int face1, int face2);
    bool IncludesVertex(ysObjectData *object, int face, int vertex);
    bool SameUVGroup(ysObjectData *object, int face1, int face2, int vertex, int mapChannel);

    void SwapVertex(ysObjectData *object, int face, int vertex, int replacement);
    int GetVertexIndex(ysObjectData *data, int face, int vertex);

    int CreateVertexCopy(ysObjectData *data, int vertex);

    // Processing
    void ResolveSmoothingGroupAmbiguity(ysObjectData *object);
    void CreateAutomaticSmoothingGroups(ysObjectData *object);

    void SeparateByUVGroups(ysObjectData *object, int mapChannel);
    void SeparateBySmoothingGroups(ysObjectData *object);

    void CalculateNormals(ysObjectData *object);
    ysVector *CalculateHardNormals(ysObjectData *object);

    ysVector *CalculateHardTangents(ysObjectData *object, int mapChannel);
    void CalculateTangents(ysObjectData *object, int mapChannel);

    void SpreadSmoothingGroup(ysObjectData *object, int face, int group, ysVector *tempNormals, int *count);

    void SortBoneWeights(ysObjectData *object, bool normalize = true, int maxBoneCount = 3);

    void UniformScale(ysObjectData *object, float scale);

};

#endif /* YDS_GEOMETRY_PREPROCESSING_H */
