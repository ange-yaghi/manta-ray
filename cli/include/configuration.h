#ifndef MANTARAY_CLI_CONFIGURATION_H
#define MANTARAY_CLI_CONFIGURATION_H

namespace mantaray_cli {

#ifdef MANTARAY_CLI_STANDALONE
#define MANTARAY_STANDARD_LIB_DIR "sdl/manta-lib/"
#else
#define MANTARAY_STANDARD_LIB_DIR "../../sdl/manta-lib/"
#endif /* MANTARAY_CLI_STANDALONE */

} /* namespace mantaray_cli */

#endif /* MANTARAY_CLI_CONFIGURATION_H */
