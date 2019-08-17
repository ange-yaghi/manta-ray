#ifndef MANTARAY_LANGUAGE_RULES_H
#define MANTARAY_LANGUAGE_RULES_H

#include <piranha.h>

namespace manta {

    class LanguageRules : public piranha::LanguageRules {
    public:
        LanguageRules();
        ~LanguageRules();

    protected:
        virtual void registerBuiltinNodeTypes();
    };

} /* namespace manta */

#endif /* MANTARAY_LANGUAGE_RULES_H */
