#ifndef COMPLEX_MATH_H
#define COMPLEX_MATH_H

#include <manta_math.h>
#include <manta_real.h>

namespace manta {
	namespace math {

		class Complex {
		public:
			Complex() { r = (real_d)0.0; i = (real_d)0.0; }
			Complex(real_d r, real_d i) : r(r), i(i) {}
			~Complex() {}

			real_d r;
			real_d i;

			Complex operator+(const Complex &b) const {
				return { r + b.r, i + b.i };
			}

			Complex operator-(const Complex &b) const {
				return { r + b.r, i + b.i };
			}

			Complex operator*(const Complex &b) const {
				return { r * b.r - i * b.i, i * b.r + r * b.i };
			}

			Complex operator*(math::real b) const {
				return { r * b, i * b };
			}

			Complex operator/(const Complex &b) const {
				real_d r_part = r * b.r + i * b.i;
				real_d i_part = i * b.r + r * b.i;
				real_d m = 1 / (b.r * b.r + b.i * b.i);
				return { r_part * m, i_part * m };
			}

			Complex conjugate() const {
				return { r, -i };
			}
		};

	} /* namespace math */

} /* namespace manta */

#endif /* COMPLEX_MATH_H */
