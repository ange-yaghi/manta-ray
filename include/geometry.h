#ifndef GEOMETRY_H
#define GEOMETRY_H

namespace manta {

	struct Face {
		union {
			struct {
				int u;
				int v;
				int w;
			};
			int indices[3];
		};

		bool hasVertex(int vertex) const {
			for (int i = 0; i < 3; i++) {
				if (indices[i] == vertex) {
					return true;
				}
			}
			return false;
		}

		int getOther(int v0, int v1) const {
			for (int i = 0; i < 3; i++) {
				if (i != v0 && i != v1) return i;
			}
			return -1;
		}

		bool isAttached(const Face &face, int *e0, int *e1, int *e0p, int *e1p) const {
			int sharedVerticies = 0;
			int f1_i, f2_i, f1_v, f2_v;
			for (int v1 = 0; v1 < 3; v1++) {
				for (int v2 = 0; v2 < 3; v2++) {
					if (indices[v1] == face.indices[v2]) {
						if (sharedVerticies == 0) {
							f1_i = v1;
							f2_i = v2;

							*e0 = f1_i;
							*e0p = f2_i;
						}
						else if (sharedVerticies == 1) {
							f1_v = v1;
							f2_v = v2;

							*e1 = f1_v;
							*e1p = f2_v;
						}
						sharedVerticies++;
					}
				}
			}

			return sharedVerticies == 2;
		}
	};

	struct QuadFace {
		union {
			struct {
				int u;
				int v;
				int w;
				int r;
			};
			int indices[4];
		};
	};

	struct AuxData {
		int n;
		int t;
	};

	struct AuxFaceData {
		AuxData data[3];

		short int material;
	};

	struct QuadAuxFaceData {
		AuxData data[4];

		union {
			short int materials[2];
			int material;
		};
	};

} /* namespace manta */

#endif /* GEOMETRY_H */
