add_library(decoder_lib STATIC decoder.cpp)
target_include_directories(decoder_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(decoder_lib PRIVATE project_options fmt::fmt)

# -------------------------------------------------------------
add_executable(app main.cpp)
target_link_libraries(app PRIVATE decoder_lib fmt::fmt)
