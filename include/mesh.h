#ifndef MESH_H
#define MESH_H

#include <scene_geometry.h>

#include <geometry.h>
#include <manta_math.h>

namespace manta {

	class ObjFileLoader;

	struct CoarseCollisionOutput {
		math::real depth;
		math::real u;
		math::real v;
		math::real w;
	};

	class Mesh : public SceneGeometry {
	public:
		Mesh();
		~Mesh();

		void initialize(int faceCount, int vertexCount, int normalCount, int texCoordCount);
		void destroy();
		void precomputeValues();

		virtual bool findClosestIntersection(const LightRay *ray, CoarseIntersection *intersection, math::real minDepth, math::real maxDepth, StackAllocator *s) const;
		virtual math::Vector getClosestPoint(const CoarseIntersection *hint, const math::Vector &p) const;
		virtual void getVicinity(const math::Vector &p, math::real radius, IntersectionList *list) const;
		virtual void fineIntersection(const math::Vector &r, IntersectionPoint *p, const CoarseIntersection *hint) const;
		virtual bool fastIntersection(const LightRay *ray) const;

		int getFaceCount() const { return m_faceCount; }
		int getVertexCount() const { return m_vertexCount; }
		int getNormalCount() const { return m_normalCount; }
		int getTexCoordCount() const { return m_texCoordCount; }

		void setPerVertexNormals(bool perVertexNormals) { m_perVertexNormals = perVertexNormals; }
		void setUseTexCoords(bool useTexCoords) { m_useTextureCoords = useTexCoords; }

		Face *getFaces() { return m_faces; }
		math::Vector *getVertices() { return m_vertices; }
		math::Vector *getNormals() { return m_normals; }
		math::Vector *getTexCoords() { return m_textureCoords; }

		const Face *getFace(int index) const { return &m_faces[index]; }
		const math::Vector *getVertex(int index) const { return &m_vertices[index]; }
		const math::Vector *getNormal(int index) const { return &m_normals[index]; }
		const math::Vector *getTexCoord(int index) const { return &m_textureCoords[index]; }

		void setFastIntersectEnabled(bool fastIntersect) { m_fastIntersectEnabled = fastIntersect; }
		bool isFastIntersectEnabled() const { return m_fastIntersectEnabled; }

		void setFastIntersectRadius(math::real radius) { m_fastIntersectRadius = radius; }
		void setFastIntersectPosition(math::Vector pos) { m_fastIntersectPosition = pos; }

		const PrecomputedValues *getPrecomputedValues() const { return m_precomputedValues; }

		void loadObjFileData(ObjFileLoader *data, int materialIndex = -1, unsigned int globalId = 0);

		void merge(const Mesh *mesh);

		math::Vector getClosestPointOnFace(int faceIndex, const math::Vector &p) const;
		void getClosestPointOnFaceBarycentric(int faceIndex, const math::Vector &p, math::real *u, math::real *v, math::real *w) const;
		bool testClosestPointOnFace(int faceIndex, math::real maxDepth, const math::Vector &p) const;
		bool detectIntersection(int faceIndex, math::real minDepth, math::real maxDepth, const math::Vector &rayDir, const math::Vector &rayOrigin, math::real delta, CoarseCollisionOutput *output) const;
		inline bool detectIntersection(int faceIndex, math::real u, math::real v, math::real w, math::real delta) const;

	protected:
		void computePlane(const math::Vector &n, const math::Vector &p, Plane *plane) const;

		Face *m_faces;
		math::Vector *m_vertices;
		math::Vector *m_normals;
		math::Vector *m_textureCoords;

		int m_rawFaceCount;
		int m_faceCount;
		int m_vertexCount;
		int m_normalCount;
		int m_texCoordCount;

		// Precomputed values
		PrecomputedValues *m_precomputedValues;

		math::real m_fastIntersectRadius;
		math::Vector m_fastIntersectPosition;
		bool m_fastIntersectEnabled;

		bool m_perVertexNormals;
		bool m_useTextureCoords;
	};

}

#endif /* MESH_H */
