#ifndef MESH_H
#define MESH_H

#include <scene_geometry.h>

#include <geometry.h>
#include <manta_math.h>
#include <primitives.h>

namespace manta {

	class ObjFileLoader;
	class MaterialManager;

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

		virtual bool findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const;
		virtual void fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const;
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

		void loadObjFileData(ObjFileLoader *data, MaterialManager *materialLibrary = nullptr, int defaultMaterialIndex = -1, unsigned int globalId = 0);

		void merge(const Mesh *mesh);

		bool detectTriangleIntersection(int faceIndex, math::real minDepth, math::real maxDepth, const LightRay *ray, CoarseCollisionOutput *output) const;
		bool detectQuadIntersection(int faceIndex, math::real minDepth, math::real maxDepth, const LightRay *ray, CoarseCollisionOutput *output) const;

		bool findClosestIntersection(int *faceList, int faceCount, const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s /**/ STATISTICS_PROTOTYPE) const;

		bool checkFaceAABB(int faceIndex, const AABB &bounds) const;
		void calculateFaceAABB(int faceIndex, AABB *target) const;

	protected:
		bool checkFaceAABB(const math::Vector &v0, const math::Vector &v1, const math::Vector &v2, const AABB &bounds) const;

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

#endif /* MESH_H */
