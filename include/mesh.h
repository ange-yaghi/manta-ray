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

		void initialize(int faceCount, int vertexCount, int normalCount);
		void precomputeValues();

		virtual const CoarseIntersection *coarseIntersection(const LightRay *ray, IntersectionList *l, SceneObject *object, const CoarseIntersection *reference, math::real epsilon) const;
		virtual void fineIntersection(const LightRay *ray, IntersectionPoint *p, const CoarseIntersection *hint, math::real bleed) const;
		virtual bool fastIntersection(const LightRay *ray) const;

		int getFaceCount() const { return m_faceCount; }
		int getVertexCount() const { return m_vertexCount; }
		int getNormalCount() const { return m_normalCount; }

		Face *getFaces() { return m_faces; }
		math::Vector *getVertices() { return m_vertices; }

		void setFastIntersectEnabled(bool fastIntersect) { m_fastIntersectEnabled = fastIntersect; }
		bool isFastIntersectEnabled() const { return m_fastIntersectEnabled; }

		void setFastIntersectRadius(math::real radius) { m_fastIntersectRadius = radius; }
		void setFastIntersectPosition(math::Vector pos) { m_fastIntersectPosition = pos; }

		const PrecomputedValues *getPrecomputedValues() const { return m_precomputedValues; }

		void loadObjFileData(ObjFileLoader *data);

		bool detectIntersection(int faceIndex, math::real earlyExitDepthHint, const math::Vector &rayDir, const math::Vector &rayOrigin, IntersectionPoint *p, math::real bleed) const;
		bool detectIntersection(int faceIndex, math::real earlyExitDepthHint, const math::Vector &rayDir, const math::Vector &rayOrigin, math::real delta, CoarseCollisionOutput *output) const;
		inline bool detectIntersection(int faceIndex, math::real u, math::real v, math::real w, math::real delta) const;

	protected:
		void computePlane(const math::Vector &n, const math::Vector &p, Plane *plane) const;

		Face *m_faces;
		math::Vector *m_vertices;
		math::Vector *m_normals;

		int m_faceCount;
		int m_vertexCount;
		int m_normalCount;

		// Precomputed values
		PrecomputedValues *m_precomputedValues;

		math::real m_fastIntersectRadius;
		math::Vector m_fastIntersectPosition;
		bool m_fastIntersectEnabled;

		bool m_perVertexNormals;

	};

}

#endif /* MESH_H */
