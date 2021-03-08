#ifndef MANTARAY_IMAGE_PLANE_PREVIEW_NODE_H
#define MANTARAY_IMAGE_PLANE_PREVIEW_NODE_H

#include "object_reference_node.h"

#include "session.h"

namespace manta {

    class ImagePlanePreviewNode : public ObjectReferenceNode<ImagePlanePreview> {
    public:
        ImagePlanePreviewNode();
        virtual ~ImagePlanePreviewNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        piranha::pNodeInput m_titleInput;
    };

} /* namespace manta */

#endif /* MANTARAY_IMAGE_PLANE_PREVIEW_NODE_H */
