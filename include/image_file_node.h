#ifndef MANTARAY_IMAGE_FILE_NODE_H
#define MANTARAY_IMAGE_FILE_NODE_H

#include <piranha.h>

#include "vector_map_2d.h"

struct SDL_Surface;

namespace manta {

	class ImageFileNode : public piranha::Node {
	public:
		struct Pixel {
			unsigned char r;
			unsigned char g;
			unsigned char b;
		};

	public:
		ImageFileNode();
		~ImageFileNode();

		void setFilename(const std::string &filename) { m_filename = filename; }
		const std::string &getFilename() const { return m_filename; }

		void setCorrectGamma(bool correctGamma) { m_correctGamma = correctGamma; }
		bool getCorrectGamma() const { return m_correctGamma; }

		const VectorMap2D *getMap() const { return &m_imageMap; }

	protected:
		virtual void _evaluate();
		virtual void _destroy();

	protected:
		static void getPixel(const SDL_Surface *surface, int x, int y, Pixel *pixel);

	protected:
		VectorMap2D m_imageMap;

		std::string m_filename;
		bool m_correctGamma;
	};

} /* namespace manta */

#endif /* MANTARAY_IMAGE_FILE_NODE_H */
