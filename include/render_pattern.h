#ifndef MANTARAY_RENDER_PATTERN_H
#define MANTARAY_RENDER_PATTERN_H

#include "object_reference_node.h"

#include "manta_math.h"

namespace manta {

    class JobQueue;
    class CameraRayEmitterGroup;
    class Scene;
    class ImagePlane;

    class RenderPattern : public ObjectReferenceNode<RenderPattern> {
    public:
        struct RenderBlock {
            int x;
            int y;
        };

    public:
        RenderPattern();
        virtual ~RenderPattern();

        struct PatternParameters {
            const Scene *scene;
            CameraRayEmitterGroup *group;
            ImagePlane *target;
        };

        void generateJobs(const PatternParameters &parameters, JobQueue *target);

        void setBlockWidth(int width) { m_blockWidth = width; }
        int getBlockWidth() const { return m_blockWidth; }

        void setBlockHeight(int height) { m_blockHeight = height; }
        int getBlockHeight() const { return m_blockHeight; }
        
    protected:
        virtual void generatePattern(int horizontalBlocks, int verticalBlocks, std::vector<RenderBlock> &blocks) = 0;

    protected:
        virtual void _initialize();
        virtual void _evaluate();
        virtual void registerInputs();

        piranha::pNodeInput m_blockWidthInput;
        piranha::pNodeInput m_blockHeightInput;

        int m_blockWidth;
        int m_blockHeight;
    };

} /* namespace manta */

#endif /* MANTARAY_RENDER_PATTERN_H */
