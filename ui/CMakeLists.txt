add_executable(mantaray_ui WIN32
    # Source Files
    src/application.cpp
    src/bounding_box.cpp
    src/compiler.cpp
    src/compiler_thread.cpp
    src/grid.cpp
    src/main.cpp
    src/preview.cpp
    src/ui_console.cpp

    # Include Files
    include/application.h
    include/bounding_box.h
    include/compiler.h
    include/compiler_thread.h
    include/configuration.h
    include/delta.h
    include/grid.h
    include/preview.h
    include/ui_console.h
)

target_link_libraries(mantaray_ui
    mantaray
    delta-basic
)
