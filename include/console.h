#ifndef MANTARAY_CONSOLE_H
#define MANTARAY_CONSOLE_H

#include <string>

namespace manta {

    class Console {
    public:
        Console();
        virtual ~Console();

        virtual void out(const std::string &text);
    };

} /* namespace manta */

#endif /* MANTARAY_CONSOLE_H */
