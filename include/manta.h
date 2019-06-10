#ifndef MANTA_H
#define MANTA_H

// Main manta include file

#pragma comment(lib,"turbojpeg.lib")
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2_image.lib")

#ifdef _M_IX86
#pragma comment(lib,"libboost_filesystem-vc141-mt-x32-1_68.lib")
#endif /* _M_IX86 */

#ifdef _M_X64
#pragma comment(lib,"libboost_filesystem-vc141-mt-x64-1_68.lib")
#endif

#endif /* MANTA_H */
