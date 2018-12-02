#ifndef SCENE_BUFFER_H
#define SCENE_BUFFER_H

#include <manta_math.h>

namespace manta {

	class SceneBuffer {
	public:
		SceneBuffer();
		~SceneBuffer();

		void initialize(int width, int height);
		void destroy();

		inline void set(const math::Vector &v, int x, int y);
		inline math::Vector sample(int x, int y) const;

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		bool isInitialized() const { return m_buffer != nullptr; }

		void clone(SceneBuffer *target) const;

		const math::Vector *getBuffer() const { return m_buffer; }

		// ----- Analysis and Manipulation -----

		// Get max intensity in the scene buffer
		math::real getMax() const;

		// Get min intensity in the scene buffer
		math::real getMin() const;

		// Get the average value in the scene buffer
		math::Vector getAverage() const;

		// Apply gamma
		void applyGammaCurve(math::real gamma);
		
		// Scale all values in the scene buffer
		void scale(math::real scale);

	protected:
		int m_width;
		int m_height;
		math::Vector *m_buffer;
	};

} /* namespace manta */

#endif /* SCENE_BUFFER_H */
