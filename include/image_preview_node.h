#ifndef MANTARAY_IMAGE_PLANE_PREVIEW_NODE_H
#define MANTARAY_IMAGE_PLANE_PREVIEW_NODE_H

#include "object_reference_node.h"

#include "session.h"

namespace manta {

    class ImagePreviewNode : public ObjectReferenceNode<ImagePreview> {
    public:
        ImagePreviewNode();
        virtual ~ImagePreviewNode();

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void _destroy();

        virtual void registerOutputs();
        virtual void registerInputs();

    protected:
        ImagePreview *m_imagePreview;

        piranha::pNodeInput m_titleInput;
    };

} /* namespace manta */

#endif /* MANTARAY_IMAGE_PLANE_PREVIEW_NODE_H */
