#ifndef MESH_H
#define MESH_H

#include <scene_geometry.h>

#include <manta_math.h>

namespace manta {

	struct SimpleVertex {
		math::Vector location;
		math::Vector normal;
	};

	struct Face {
		unsigned int u;
		unsigned int w;
		unsigned int v;
	};

	struct Plane {
		math::Vector normal;
		math::real d;
	};

	struct PrecomputedValues {
		Plane edgePlaneVW;
		Plane edgePlaneWU;
		math::Vector normal;
		math::Vector p0;
	};

	class Mesh : public SceneGeometry {
	public:
		Mesh();
		~Mesh();

		void initialize(int faceCount, int vertexCount);
		void fixNormals();
		void precomputeValues();

		virtual void detectIntersection(const LightRay *ray, IntersectionPoint *p) const;
		virtual bool fastIntersection(const LightRay *ray) const;

		int getFaceCount() const { return m_faceCount; }
		int getVertexCount() const { return m_vertexCount; }

		Face *getFaces() { return m_faces; }
		SimpleVertex *getVertices() { return m_vertices; }

		void setFastIntersectEnabled(bool fastIntersect) { m_fastIntersectEnabled = fastIntersect; }
		bool isFastIntersectEnabled() const { return m_fastIntersectEnabled; }

		void setFastIntersectRadius(math::real radius) { m_fastIntersectRadius = radius; }

	protected:
		bool detectIntersection(int faceIndex, math::real earlyExitDepthHint, const math::Vector &rayDir, const math::Vector &rayOrigin, IntersectionPoint *p) const;

		void computePlane(const math::Vector &n, const math::Vector &p, Plane *plane) const;

		Face *m_faces;
		SimpleVertex *m_vertices;

		int m_faceCount;
		int m_vertexCount;

		// Precomputed values
		PrecomputedValues *m_precomputedValues;

		math::real m_fastIntersectRadius;
		bool m_fastIntersectEnabled;

	};

}

#endif /* MESH_H */
