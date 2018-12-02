#ifndef SETTINGS_H
#define SETTINGS_H

#define VISUAL_STUDIO	0x01
#define STANDALONE		0x02

#define EXECUTION_ENV	VISUAL_STUDIO

// Paths

#if EXECUTION_ENV == VISUAL_STUDIO

#define MODEL_PATH "../../demos/models/"
#define RENDER_OUTPUT "../../workspace/render/"

#elif EXECUTION_ENV == STANDALONE

#error "Standalone execution environment not defined"

#else

#error "Undefined execution environment"

#endif /* EXECUTION_ENV */

#endif /* CONF_H */
