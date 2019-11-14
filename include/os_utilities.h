#ifndef MANTARAY_OS_UTILITIES_H
#define MANTARAY_OS_UTILITIES_H

#include "path.h"

namespace manta {

    void createFolder(const char *folder);
    void showConsoleCursor(bool show);
    void sleep(int milliseconds);

    Path getModuleDirectory();

} /* namespace manta */

#endif /* MANTARAY_OS_UTILITIES_H */
