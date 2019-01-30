#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace manta {

	struct Face {
		int u;
		int w;
		int v;
	};

	struct AuxFaceData {
		int nu;
		int nv;
		int nw;

		int tu;
		int tv;
		int tw;

		int material;
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
