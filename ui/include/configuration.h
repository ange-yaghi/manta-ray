#ifndef MANTARAY_UI_CONFIGURATION_H
#define MANTARAY_UI_CONFIGURATION_H

namespace mantaray_ui {

#ifdef MANTARAY_UI_STANDALONE
#define MANTARAY_STANDARD_LIB_DIR "sdl/manta-lib/"
#else
#define MANTARAY_STANDARD_LIB_DIR "../../sdl/manta-lib/"
#endif /* MANTARAY_UI_STANDALONE */

} /* namespace mantaray_ui */

#endif /* MANTARAY_UI_CONFIGURATION_H */
