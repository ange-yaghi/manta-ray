#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace manta {

	struct Face {
		unsigned int u;
		unsigned int w;
		unsigned int v;

		unsigned int nu;
		unsigned int nv;
		unsigned int nw;
	};

	struct Plane {
		math::Vector normal;
		math::real d;
	};

	struct PrecomputedValues {
		Plane edgePlaneVW;
		Plane edgePlaneWU;
		Plane edgePlaneVU; // Used only for coarse test
		math::Vector normal;
		math::Vector p0;
		math::real scaleVW;
		math::real scaleWU;
	};

} /* namespace manta */

#endif /* GEOMETRY_H */
