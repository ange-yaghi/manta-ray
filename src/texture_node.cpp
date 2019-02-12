#include <texture_node.h>

#include <intersection_point.h>

#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>

manta::TextureNode::TextureNode() {
	m_width = 0;
	m_height = 0;
	m_imageData = nullptr;
}

manta::TextureNode::~TextureNode() {

}

void manta::TextureNode::loadFile(const char *fname, math::real gamma) {
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

manta::math::Vector manta::TextureNode::sample(const IntersectionPoint *surfaceInteraction) const {
	math::real u = math::getX(surfaceInteraction->m_textureCoodinates);
	math::real v = math::getY(surfaceInteraction->m_textureCoodinates);

	// Simple sampling for now
	int pixelY = (int)(v * (m_height - 1)) % m_height;
	int pixelX = (int)(u * (m_width - 1)) % m_width;

	if (pixelY < 0) pixelY += m_height;
	if (pixelX < 0) pixelX += m_width;

	pixelY = m_height - pixelY - 1;

	assert(pixelY >= 0 && pixelY < m_height);
	assert(pixelX >= 0 && pixelX < m_width);

	Pixel *pixel = &m_imageData[pixelY][pixelX];

	math::real rr = pow(pixel->r / (math::real)255.0, m_gamma);
	math::real rg = pow(pixel->g / (math::real)255.0, m_gamma);
	math::real rb = pow(pixel->b / (math::real)255.0, m_gamma);

	return math::loadVector(rr, rg, rb);
}

void manta::TextureNode::getPixel(const SDL_Surface *surface, int x, int y, Pixel *pixelOut) {
	Uint32 color = 0;

	Uint8 *pixel = (Uint8*)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint8) * surface->format->BytesPerPixel);
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
