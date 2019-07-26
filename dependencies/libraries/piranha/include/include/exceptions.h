#ifndef PIRANHA_EXCEPTIONS_H
#define PIRANHA_EXCEPTIONS_H

#include <exception>

namespace piranha {

    struct MissingConversion : public std::exception {
        virtual const char *what() const throw() {
            return "A required conversion is missing from the language specification";
        }
    };

} /* namespace piranha */

#endif /* PIRANHA_EXCEPTIONS_H */
