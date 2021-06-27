cmake_minimum_required(VERSION 3.10)

project(JTLComponentChecker)

if(MSVC)
  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
endif(MSVC)

find_package(Tesseract CONFIG REQUIRED)

# add the executable
add_executable(JTLComponentChecker src/main.cpp 
 src/read.cpp 
 src/parse.cpp
 src/imgui/imgui.cpp
 src/imgui/imgui_draw.cpp 
 src/imgui/imgui_impl_win32.cpp 
 src/imgui/imgui_impl_dx9.cpp 
 src/imgui/imgui_tables.cpp
 src/imgui/imgui_demo.cpp
 src/imgui/imgui_widgets.cpp)

target_link_libraries(JTLComponentChecker PRIVATE libtesseract)
target_link_libraries(JTLComponentChecker PRIVATE d3d9.lib)

target_include_directories(JTLComponentChecker PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(JTLComponentChecker PRIVATE cxx_std_17)