#include <texture_map.h>

#include <SDL.h>
#include <SDL_image.h>

manta::TextureMap::TextureMap() {
	m_width = 0;
	m_height = 0;
	m_imageData = nullptr;
}

manta::TextureMap::~TextureMap() {

}

void manta::TextureMap::loadFile(const char *fname, math::real gamma) {
	SDL_Surface *image;
	image = IMG_Load(fname);

	m_imageData = new Pixel *[image->h];

	for (int i = 0; i < image->h; i++) {
		m_imageData[i] = new Pixel[image->w];
	}

	for (int i = 0; i < image->w; i++) {
		for (int j = 0; j < image->h; j++) {
			getPixel(image, i, j, &m_imageData[j][i]);
		}
	}

	m_width = image->w;
	m_height = image->h;

	m_gamma = gamma;

	SDL_free((void *)image);
}

manta::math::Vector manta::TextureMap::sample(math::real u, math::real v) {
	// Simple sampling for now
	if (u > 1.0) u = 1.0;
	if (u < 0.0) u = 0.0;

	if (v > 1.0) v = 1.0;
	if (v < 0.0) v = 0.0;

	Pixel *pixel = &m_imageData[(int)((1.0 - v) * (m_height - 1))][(int)(u * (m_width - 1))];

	math::real rr = pow(pixel->r / (math::real)255.0, m_gamma);
	math::real rg = pow(pixel->g / (math::real)255.0, m_gamma);
	math::real rb = pow(pixel->b / (math::real)255.0, m_gamma);

	return math::loadVector(rr, rg, rb);
}

void manta::TextureMap::getPixel(const SDL_Surface *surface, int x, int y, Pixel *pixelOut) {
	Uint32 color = 0;

	Uint8 *pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint8) * 4);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	pixelOut->r = pixel[0];
	pixelOut->g = pixel[1];
	pixelOut->b = pixel[2];
#else
	pixelOut->r = pixel[0];
	pixelOut->g = pixel[1];
	pixelOut->b = pixel[2];
#endif
}
