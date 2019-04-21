#ifndef IMAGE_FILE_NODE_H
#define IMAGE_FILE_NODE_H

#include <map_2d_node.h>

struct SDL_Surface;

namespace manta {

	class ImageFileNode : public VectorMap2DNode {
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

#endif /* IMAGE_FILE_NODE_H */
