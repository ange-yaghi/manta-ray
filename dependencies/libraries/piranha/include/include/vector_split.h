#ifndef PIRANHA_VECTOR_SPLIT_H
#define PIRANHA_VECTOR_SPLIT_H

#include "fundamental_output.h"

namespace piranha {

    template<int index>
    class VectorSplitOutput : public FloatValueOutput {
    public:
        VectorSplitOutput() { m_input = nullptr; }
        ~VectorSplitOutput() { /* void */ }

        virtual void fullCompute(void *_target) const {
            piranha::native_float *target = reinterpret_cast<piranha::native_float *>(_target);

            piranha::native_vector data;
            m_input->fullCompute((void *)&data);

            switch (index) {
            case 0: *target = data.x; return;
            case 1: *target = data.y; return;
            case 2: *target = data.z; return;
            case 3: *target = data.w; return;
            default: *target = 0.0; return;
            }
        }

        virtual void registerInputs() {
            registerInput(&m_input);
        }

        pNodeInput *getInputConnection() { return &m_input; }

    protected:
        pNodeInput m_input;
    };

} /* namespace piranha */

#endif /* PIRANHA_VECTOR_SPLIT_H */
