#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <manta_math.h>

struct SDL_Surface;

namespace manta {

	struct Pixel {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	class TextureMap {
	public:
		TextureMap();
		~TextureMap();

		void loadFile(const char *fname, math::real gamma);

		math::Vector sample(math::real u, math::real v);

	protected:
		static void getPixel(const SDL_Surface *surface, int x, int y, Pixel *pixel);

		int m_width;
		int m_height;

		math::real m_gamma;

		Pixel **m_imageData;
	};

} /* namespace manta */

#endif /* TEXTURE_MAP_H */
