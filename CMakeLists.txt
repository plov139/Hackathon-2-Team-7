cmake_minimum_required(VERSION 3.20)
project(hackathon C CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz)
FetchContent_MakeAvailable(raylib)

# ==========================================================
#  class demo: example.cpp
#  (your copy said "add_executable(example main.cpp)" -- it must be example.cpp)
# ==========================================================
add_executable(example example.cpp)
target_link_libraries(example PRIVATE raylib)

# ==========================================================
#  HACKATHON: your program. List every .cpp file here.
#  Made a new class? Add its .cpp file to this list.
#  (Don't list .h files, only .cpp files.)
# ==========================================================
add_executable(main
    main.cpp
    Tower.cpp
    Link.cpp
)
target_link_libraries(main PRIVATE raylib)
