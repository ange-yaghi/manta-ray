add_subdirectory(piranha)

set_property(TARGET piranha PROPERTY FOLDER "piranha")
set_property(TARGET piranha_test PROPERTY FOLDER "piranha")
set_property(TARGET piranha_reference_compiler PROPERTY FOLDER "piranha")

add_subdirectory(delta-studio)
