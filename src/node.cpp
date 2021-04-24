#include "../include/node.h"

#include "../include/path.h"

manta::Node::Node() {
    /* void */
}

manta::Node::~Node() {
    /* void */
}

void manta::Node::resolvePath(const Path *relative, Path *target) const {
    if (relative->isAbsolute()) {
        *target = *relative;
        return;
    }

    piranha::Path unitFile = getProgram()->getRootUnit()->getPath();
    piranha::Path home;
    unitFile.getParentPath(&home);

    *target = Path(home.toString()).append(*relative);
}

void manta::Node::_initialize() {
    /* void */
}

void manta::Node::_evaluate() {
    /* void */
}

void manta::Node::_destroy() {
    /* void */
}
