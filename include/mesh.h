#ifndef MANTARAY_MESH_H
#define MANTARAY_MESH_H

#include "scene_geometry.h"

#include "geometry.h"
#include "manta_math.h"
#include "primitives.h"

namespace manta {

    class ObjFileLoader;
    class MaterialLibrary;

    struct CoarseCollisionOutput {
        math::real depth;
        math::real u;
        math::real v;
        math::real w;

        int subdivisionHint;
    };

    class Mesh : public SceneGeometry {
    public:
        Mesh();
        ~Mesh();

        void initialize(int faceCount, int vertexCount, int normalCount, int texCoordCount);
        void destroy();
        void filterDegenerateFaces();
        void findQuads();
        void computeBounds();

        virtual bool findClosestIntersection(const LightRay *ray, 
            CoarseIntersection *intersection, math::real minDepth, 
            math::real maxDepth, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const;
        virtual void fineIntersection(const math::Vector &r, IntersectionPoint *p, 
            const CoarseIntersection *hint) const;
        virtual bool fastIntersection(const LightRay *ray) const;

        int getFaceCount() const { return m_triangleFaceCount + m_quadFaceCount; }
        int getTriangleFaceCount() const { return m_triangleFaceCount; }
        int getQuadFaceCount() const { return m_quadFaceCount; }
        int getVertexCount() const { return m_vertexCount; }
        int getNormalCount() const { return m_normalCount; }
        int getTexCoordCount() const { return m_texCoordCount; }

        void setPerVertexNormals(bool perVertexNormals) { m_perVertexNormals = perVertexNormals; }
        void setUseTexCoords(bool useTexCoords) { m_useTextureCoords = useTexCoords; }

        Face *getFaces() { return m_faces; }
        AuxFaceData *getAuxFaceData() { return m_auxFaceData; }
        math::Vector *getVertices() { return m_vertices; }
        math::Vector *getNormals() { return m_normals; }
        math::Vector *getTexCoords() { return m_textureCoords; }

        const Face *getFace(int index) const { return &m_faces[index]; }
        const AuxFaceData *getAuxFace(int index) const { return &m_auxFaceData[index]; }
        const QuadFace *getQuadFace(int index) const { return &m_quadFaces[index]; }
        const AABB *getBounds(int index) const { return &m_faceBounds[index]; }
        const math::Vector *getVertex(int index) const { return &m_vertices[index]; }
        const math::Vector *getNormal(int index) const { return &m_normals[index]; }
        const math::Vector *getTexCoord(int index) const { return &m_textureCoords[index]; }

        void setFastIntersectEnabled(bool fastIntersect) { m_fastIntersectEnabled = fastIntersect; }
        bool isFastIntersectEnabled() const { return m_fastIntersectEnabled; }

        void setFastIntersectRadius(math::real radius) { m_fastIntersectRadius = radius; }
        void setFastIntersectPosition(math::Vector pos) { m_fastIntersectPosition = pos; }

        void loadObjFileData(ObjFileLoader *data, MaterialLibrary *materialLibrary = nullptr, 
            int defaultMaterialIndex = -1, unsigned int globalId = 0);

        void merge(const Mesh *mesh);

        __forceinline bool detectTriangleIntersection(int faceIndex, math::real minDepth, 
            math::real maxDepth, const LightRay *ray, CoarseCollisionOutput *output) const 
        {
            Face &face = m_faces[faceIndex];

            math::Vector v0, v1, v2;
            v0 = m_vertices[face.u];
            v1 = m_vertices[face.v];
            v2 = m_vertices[face.w];

            math::Vector rayOrigin = ray->getSource();
            math::Vector p0t = math::sub(v0, rayOrigin);
            math::Vector p1t = math::sub(v1, rayOrigin);
            math::Vector p2t = math::sub(v2, rayOrigin);

            int kz = ray->getKZ();
            int kx = ray->getKX();
            int ky = ray->getKY();
            p0t = math::permute(p0t, kx, ky, kz);
            p1t = math::permute(p1t, kx, ky, kz);
            p2t = math::permute(p2t, kx, ky, kz);

            const math::Vector3 &shear = ray->getShear();
            math::real sx = shear.x;
            math::real sy = shear.y;
            math::real sz = shear.z;

            math::Vector s = math::loadVector(sx, sy);
            math::Vector p0t_z = math::expandZ(p0t);
            math::Vector p1t_z = math::expandZ(p1t);
            math::Vector p2t_z = math::expandZ(p2t);

            p0t = math::add(p0t, math::mul(s, p0t_z));
            p1t = math::add(p1t, math::mul(s, p1t_z));
            p2t = math::add(p2t, math::mul(s, p2t_z));

            math::real e0 = math::getX(p1t) * math::getY(p2t) - math::getY(p1t) * math::getX(p2t);
            math::real e1 = math::getX(p2t) * math::getY(p0t) - math::getY(p2t) * math::getX(p0t);
            math::real e2 = math::getX(p0t) * math::getY(p1t) - math::getY(p0t) * math::getX(p1t);

            if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0)) return false;

            math::real det = e0 + e1 + e2;
            if (det == (math::real)0.0) {
                return false;
            }

            // Compute distance
            math::real p0t_sz = math::getZ(p0t) * sz;
            math::real p1t_sz = math::getZ(p1t) * sz;
            math::real p2t_sz = math::getZ(p2t) * sz;

            math::real t_scaled = e0 * p0t_sz + e1 * p1t_sz + e2 * p2t_sz;

            if (det < 0 && (t_scaled >= 0 || t_scaled < maxDepth * det)) return false;
            else if (det > 0 && (t_scaled <= 0 || t_scaled > maxDepth * det)) return false;

            math::real invDet = 1 / det;
            output->depth = t_scaled * invDet;
            output->u = e0 * invDet;
            output->v = e1 * invDet;
            output->w = e2 * invDet;

            return true;
        }

        bool detectQuadIntersection(int faceIndex, math::real minDepth, 
            math::real maxDepth, const LightRay *ray, CoarseCollisionOutput *output) const;

        bool findClosestIntersection(int *faceList, int faceCount, const LightRay *ray, 
            CoarseIntersection *intersection, math::real minDepth, math::real maxDepth /**/ STATISTICS_PROTOTYPE) const;

        bool checkFaceAABB(int faceIndex, const AABB &bounds) const;
        void calculateFaceAABB(int faceIndex, AABB *target) const;

    protected:
        bool checkFaceAABB(const math::Vector &v0, const math::Vector &v1, 
            const math::Vector &v2, const AABB &bounds) const;

        Face *m_faces;
        AuxFaceData *m_auxFaceData;
        QuadFace *m_quadFaces;
        QuadAuxFaceData *m_auxQuadFaceData;
        AABB *m_faceBounds;

        math::Vector *m_vertices;
        math::Vector *m_normals;
        math::Vector *m_textureCoords;

        int m_rawFaceCount;
        int m_triangleFaceCount;
        int m_quadFaceCount;
        int m_vertexCount;
        int m_normalCount;
        int m_texCoordCount;

        math::real m_fastIntersectRadius;
        math::Vector m_fastIntersectPosition;
        bool m_fastIntersectEnabled;

        bool m_perVertexNormals;
        bool m_useTextureCoords;
    };

} /* namespace manta */

#endif /* MANTARAY_MESH_H */
