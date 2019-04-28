#ifndef POLYGONAL_APERTURE_H
#define POLYGONAL_APERTURE_H

#include <aperture.h>

namespace manta {

	class PolygonalAperture : public Aperture {
	public:
		struct Edge {
			math::Vector2 reference;
			math::Vector2 direction;
			math::real cache;
		};

	public:
		PolygonalAperture();
		~PolygonalAperture();

		void initialize(int edges, math::real angle = (math::real)0.0, bool halfOffset=false);
		void destroy();

		virtual bool filter(math::real x, math::real y) const;

		void setBladeCurvature(math::real bladeCurvature) { m_bladeCurvature = bladeCurvature; }
		math::real getBladeCurvature() const { return m_bladeCurvature; }

	protected:
		Edge *m_edges;
		int m_edgeCount;

		math::real m_bladeCurvature;
	};

} /* namespace manta */

#endif /* POLYGONAL_APERTURE_H */
