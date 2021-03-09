#ifndef MANTARAY_CONSOLE_H
#define MANTARAY_CONSOLE_H

#include "manta_math.h"

#include <string>

namespace manta {

    class Console {
    public:
        Console();
        virtual ~Console();

        virtual void out(const std::string &text, const math::Vector &color = math::constants::One);
    };

} /* namespace manta */

#endif /* MANTARAY_CONSOLE_H */
