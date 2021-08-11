add_executable(mantaray-cli
    src/compiler.cpp
    src/main.cpp
)

target_link_libraries(mantaray-cli
    mantaray
)
