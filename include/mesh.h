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
		Plane plane;
		Plane edgePlaneVW;
		Plane edgePlaneWU;
	};

	class Mesh : public SceneGeometry {
	public:
		Mesh();
		~Mesh();

		void initialize(int faceCount, int vertexCount);
		void precomputeValues();

		virtual void detectIntersection(const LightRay *ray, IntersectionPoint *p) const;

		int getFaceCount() const { return m_faceCount; }
		int getVertexCount() const { return m_vertexCount; }

		Face *getFaces() { return m_faces; }
		SimpleVertex *getVertices() { return m_vertices; }

	protected:
		bool detectIntersection(int faceIndex, math::real earlyExitDepthHint, const LightRay *ray, IntersectionPoint *p) const;

		void computePlane(const math::Vector &n, const math::Vector &p, Plane *plane) const;

		Face *m_faces;
		SimpleVertex *m_vertices;

		int m_faceCount;
		int m_vertexCount;

		// Precomputed values
		PrecomputedValues *m_precomputedValues;
	};

}

#endif /* MESH_H */
