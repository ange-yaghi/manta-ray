#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <vector_material_node.h>

#include <manta_math.h>

struct SDL_Surface;

namespace manta {

	struct Pixel {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};

	struct IntersectionPoint;

	class TextureNode : public VectorMaterialNode {
	public:
		TextureNode();
		virtual ~TextureNode();

		void loadFile(const char *fname, math::real gamma);

		virtual math::Vector sample(const IntersectionPoint *surfaceInteraction) const;

	protected:
		static void getPixel(const SDL_Surface *surface, int x, int y, Pixel *pixel);

		int m_width;
		int m_height;

		math::real m_gamma;

		Pixel **m_imageData;
	};

} /* namespace manta */

#endif /* TEXTURE_MAP_H */
