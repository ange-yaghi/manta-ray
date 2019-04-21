#include <image_file_node.h>

#include <rgb_space.h>
#include <standard_allocator.h>
#include <vector_map_2d.h>

#include <SDL_image.h>
#include <assert.h>

manta::ImageFileNode::ImageFileNode() {
	m_filename = "";
	m_correctGamma = true;
}

manta::ImageFileNode::~ImageFileNode() {
	/* void */
}

void manta::ImageFileNode::_evaluate() {
	VectorMap2DNode::_evaluate();

	SDL_Surface *image;
	image = IMG_Load(m_filename.c_str());

	// Create a temporary pixel buffer
	Pixel **pixelData = StandardAllocator::Global()->allocate<Pixel *>(image->h);
	for (int i = 0; i < image->h; i++) {
		pixelData[i] = StandardAllocator::Global()->allocate<Pixel>(image->w);;
	}

	for (int i = 0; i < image->w; i++) {
		for (int j = 0; j < image->h; j++) {
			getPixel(image, i, j, &pixelData[j][i]);
		}
	}

	m_imageMap.initialize(image->w, image->h);

	for (int i = 0; i < image->w; i++) {
		for (int j = 0; j < image->h; j++) {
			Pixel &pixel = pixelData[j][i];

			math::real r = pixel.r / (math::real)255;
			math::real g = pixel.g / (math::real)255;
			math::real b = pixel.b / (math::real)255;

			if (m_correctGamma) {
				// Default to SRGB
				// TODO: make gamma correction generic
				r = (math::real)RgbSpace::srgb.inverseGammaSrgb(r);
				g = (math::real)RgbSpace::srgb.inverseGammaSrgb(g);
				b = (math::real)RgbSpace::srgb.inverseGammaSrgb(b);
			}

			m_imageMap.set(math::loadVector(r, g, b), i, j);
		}
	}

	// Free pixel data
	for (int i = 0; i < image->h; i++) {
		StandardAllocator::Global()->free(pixelData[i], image->w);
	}
	StandardAllocator::Global()->free(pixelData, image->h);

	// Free the SDL image
	SDL_free((void *)image);

	setMap(&m_imageMap);
}

void manta::ImageFileNode::_destroy() {
	Map2DNode::_destroy();

	m_imageMap.destroy();
}

void manta::ImageFileNode::getPixel(const SDL_Surface *surface, int x, int y, Pixel *pixelOut) {
	Uint32 color = 0;
	Uint8 *pixel = (Uint8 *)surface->pixels;
	pixel += (y * surface->pitch) + (x * sizeof(Uint8) * surface->format->BytesPerPixel);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	pixelOut->r = pixel[0];
	pixelOut->g = pixel[1];
	pixelOut->b = pixel[2];
#else
	pixelOut->r = pixel[0];
	pixelOut->g = pixel[1];
	pixelOut->b = pixel[2];
#endif /* SDL_BYTEORDER == SDL_BIG_ENDIAN */
}
