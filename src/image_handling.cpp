#include <image_handling.h>

#include <SDL.h>
#include <SDL_image.h>

#include <manta_math.h>

#include <math.h>

Uint32 GetPixel24(SDL_Surface *surface, int x, int y) {
	Uint32 color = 0;

	Uint8 *pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint8) * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	color |= pixel[0] << 16;
	color |= pixel[1] << 8;
	color |= pixel[2];
#else
	color |= pixel[2] << 16;
	color |= pixel[1] << 8;
	color |= pixel[0];
#endif

	return color;
}

void PlacePixel24(SDL_Surface * surface, int x, int y, const manta::math::Vector &color) {
	Uint32 color32 = 0;

	manta::math::real r = manta::math::getX(color);
	manta::math::real g = manta::math::getY(color);
	manta::math::real b = manta::math::getZ(color);

	manta::math::real gamma = (manta::math::real)1.0;

	r = (manta::math::real)pow(manta::math::clamp(r), 1 / gamma);
	g = (manta::math::real)pow(manta::math::clamp(g), 1 / gamma);
	b = (manta::math::real)pow(manta::math::clamp(b), 1 / gamma);

	color32 |= lround(r * 255);
	color32 |= (lround(g * 255) << 8);
	color32 |= (lround(b * 255) << 16);

	Uint8 * pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint8) * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	pixel[0] = (color32 >> 24) & 0xFF;
	pixel[1] = (color32 >> 16) & 0xFF;
	pixel[2] = (color32 >> 8) & 0xFF;
#else
	pixel[0] = color32 & 0xFF;
	pixel[1] = (color32 >> 8) & 0xFF;
	pixel[2] = (color32 >> 16) & 0xFF;
#endif
	int a = 0;
}

void manta::SaveImageData(const math::Vector *target, int width, int height, const char *fname) {

	/* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
	as expected by OpenGL for textures */
	SDL_Surface *surface;
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	surface = SDL_CreateRGBSurface(0, width, height, 24,
		rmask, gmask, bmask, amask);

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			PlacePixel24(surface, i, j, target[j * width + i]);
			const char *err = SDL_GetError();
		}
	}

	SDL_SaveBMP(surface, fname);
	const char *err = SDL_GetError();
}