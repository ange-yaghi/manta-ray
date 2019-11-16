#ifndef MANTARAY_AVERAGE_NODE_OUTPUT_H
#define MANTARAY_AVERAGE_NODE_OUTPUT_H

#include "vector_node_output.h"

namespace manta {

    class AverageNodeOutput : public VectorNodeOutput {
    public:
        AverageNodeOutput() {
            m_input = nullptr;
        }

        virtual ~AverageNodeOutput() {
            /* void */
        }

        virtual void sample(const IntersectionPoint *surfaceInteraction, void *_target) const {
            math::Vector *target = reinterpret_cast<math::Vector *>(_target);

            VectorNodeOutput *input = static_cast<VectorNodeOutput *>(m_input);
            int dimensions = input->getDimensions();

            if (dimensions > 0) {
                if (dimensions == 2) {
                    int width = input->getSize(0);
                    int height = input->getSize(1);

                    int n = 0;
                    math::Vector mn = math::constants::Zero;
                    for (int i = 0; i < width; i++) {
                        for (int j = 0; j < height; j++) {
                            n++;

                            math::Vector v;
                            input->discreteSample2d(i, j, (void *)&v);

                            math::Vector inc = math::sub(v, mn);
                            inc = math::div(inc, math::loadScalar((math::real)n));

                            mn = math::add(mn, inc);
                        }
                    }

                    *target = mn;
                }
                else *target = math::constants::Zero;
            }
            else input->sample(surfaceInteraction, (void *)target);
        }

        virtual void discreteSample2d(int x, int y, void *target) const {
            return sample(nullptr, target);
        }

        virtual void getDataReference(const void **target) const {
            *target = nullptr;
        }

        virtual void _evaluateDimensions() {
            setDimensions(0);
        }

        piranha::pNodeInput *getConnection() { return &m_input; }

    protected:
        virtual void registerInputs() {
            registerInput(&m_input);
        }

    protected:
        piranha::pNodeInput m_input;
    };

    typedef piranha::PipeNode<AverageNodeOutput> AverageNode;

} /* namespace manta */

#endif /* MANTARAY_AVERAGE_OUTPUT_H */
