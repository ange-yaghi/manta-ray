#ifndef YDS_CORE_H
#define YDS_CORE_H

// Base
#include "yds_base.h"
#include "yds_object_data.h"

// Audio
#include "yds_audio_buffer.h"
#include "yds_audio_source.h"
#include "yds_audio_system.h"
#include "yds_windows_audio_wave_file.h"
#include "yds_audio_device.h"

// Input
#include "yds_input_system.h"
#include "yds_keyboard.h"

// Animation
#include "yds_object_animation_data.h"
#include "yds_tool_animation_file.h"
#include "yds_animation_action.h"
#include "yds_animation_action_binding.h"
#include "yds_animation_curve.h"
#include "yds_animation_mixer.h"
#include "yds_animation_target.h"
#include "yds_animation_interchange_file.h"

// Timing
#include "yds_timing.h"
#include "yds_breakdown_timer.h"
#include "yds_breakdown_timer_channel.h"

// Math
#include "yds_math.h"

// Memory management
#include "yds_expanding_array.h"

// Textures
#include "yds_texture.h"

// GPU Buffers
#include "yds_gpu_buffer.h"

// Utilities
#include "yds_registry.h"

// Geometry
#include "yds_interchange_file_0_0.h"
#include "yds_interchange_file_0_1.h"
#include "yds_tool_geometry_file.h"
#include "yds_geometry_preprocessing.h"
#include "yds_geometry_export_file.h"

// Object
#include "yds_transform.h"

// Graphics API
#include "yds_device.h"

// Color
#include "yds_color.h"

// OS
#include "yds_window_event_handler.h"

// ========================================================
// Dependencies
// ========================================================

// Main library
#pragma comment(lib, "delta-core.lib")

// External dependencies
#pragma comment(lib, "comctl32.lib")

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "windowscodecs.lib")

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_Image.lib")

#pragma comment(lib, "OpenGL32.lib")

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx9d.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx10d.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx11d.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dxguid.lib")

#endif /* YDS_CORE_H */
