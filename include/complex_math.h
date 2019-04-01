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

			inline Complex operator+(const Complex &b) const {
				return { r + b.r, i + b.i };
			}

			inline Complex operator-(const Complex &b) const {
				return { r + b.r, i + b.i };
			}

			inline Complex operator*(const Complex &b) const {
				return { r * b.r - i * b.i, i * b.r + r * b.i };
			}

			inline Complex operator*(math::real b) const {
				return { r * b, i * b };
			}

			inline Complex operator/(const Complex &b) const {
				real_d r_part = r * b.r + i * b.i;
				real_d i_part = i * b.r + r * b.i;
				real_d m = 1 / (r_part * r_part + i_part * i_part);
				return { r_part * m, i_part * m };
			}

			inline Complex conjugate(const Complex &b) const {
				return { r, -i };
			}
		};

	} /* namespace math */
} /* namespace manta */

#endif /* COMPLEX_MATH_H */
