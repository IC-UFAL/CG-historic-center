cmake_minimum_required(VERSION 3.13)
project(historic-center)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/mingw64")

add_executable(historic-center "historic-center.cpp" Point.h Point.cpp Camera.h Camera.cpp Model.h Model.cpp Face.h Face.cpp Color.h Color.cpp Cube.cpp Cube.h Cylinder.cpp Cylinder.h Texture.h)

include_directories("C:/Program Files (x86)/mingw64/include")

target_link_libraries(historic-center -lOpenGL32 -lfreeGLUT -lglu32)
