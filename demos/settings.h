#ifndef SETTINGS_H
#define SETTINGS_H

#define VISUAL_STUDIO	0x01
#define STANDALONE		0x02

#define EXECUTION_ENV	VISUAL_STUDIO

// Paths

#if EXECUTION_ENV == VISUAL_STUDIO

#define WORKSPACE_PATH "../../workspace/"
#define TMP_PATH (WORKSPACE_PATH "tmp/")
#define MODEL_PATH "../../demos/models/"
#define TEXTURE_PATH "../../demos/textures/"
#define RENDER_OUTPUT (WORKSPACE_PATH "render/")
#define CMF_PATH "../../demos/cmfs/"

#elif EXECUTION_ENV == STANDALONE

#error "Standalone execution environment not defined"

#else

#error "Undefined execution environment"

#endif /* EXECUTION_ENV */

#endif /* SETTINGS_H */
