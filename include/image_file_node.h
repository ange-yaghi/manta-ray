#ifndef MANTARAY_IMAGE_FILE_NODE_H
#define MANTARAY_IMAGE_FILE_NODE_H

#include "node.h"

#include "vector_map_2d.h"
#include "vector_map_2d_node_output.h"

struct SDL_Surface;

namespace manta {

    class ImageFileNode : public Node {
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
        static void getPixel(const SDL_Surface *surface, int x, int y, Pixel *pixel);

    protected:
        VectorMap2D m_imageMap;

        std::string m_filename;
        bool m_correctGamma;

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();
        virtual void registerInputs();
        virtual void registerOutputs();

        VectorMap2DNodeOutput m_output;

        piranha::pNodeInput m_filenameInput;
        piranha::pNodeInput m_correctGammaInput;
    };

} /* namespace manta */

#endif /* MANTARAY_IMAGE_FILE_NODE_H */
