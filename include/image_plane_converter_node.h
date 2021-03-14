#ifndef MANTARAY_IMAGE_PLANE_CONVERTER_NODE_H
#define MANTARAY_IMAGE_PLANE_CONVERTER_NODE_H

#include <piranha.h>

#include "manta_math.h"
#include "image_file_node.h"
#include "vector_map_2d_node_output.h"

namespace manta {

    class ImagePlane;

    class ImagePlaneConverterNode : public piranha::Node {
    public:
        ImagePlaneConverterNode();
        virtual ~ImagePlaneConverterNode();

        VectorMap2DNodeOutput *getMainOutput() { return &m_textureOutput; }

    protected:
        virtual void registerInputs();
        virtual void registerOutputs();

        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

    protected:
        ImagePlane *m_imagePlane;
        VectorMap2D *m_target;

    protected:
        piranha::pNodeInput m_imagePlaneInput;

        VectorMap2DNodeOutput m_textureOutput;
    };

} /* namespace manta */

#endif /* MANTARAY_IMAGE_PLANE_CONVERTER_NODE_H */
