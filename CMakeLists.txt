cmake_minimum_required(VERSION 3.7...3.26)

# Allow IDE Source Tree Visualization
if(${CMAKE_VERSION} VERSION_LESS 3.26)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
else()
    cmake_policy(SET CMP0143 NEW)
endif()

project(taxxevasion)

set(SRCS
    "src/utils/static_vector.h"

    "src/attacks.cpp"
    "src/attacks.h"
    "src/bitboard.h"
    "src/board.cpp"
    "src/board.h"
    "src/defs.h"
    "src/main.cpp"
    "src/movegen.cpp"
    "src/movegen.h"
)

add_executable(taxx-evasion ${SRCS})

target_compile_features(taxx-evasion PRIVATE cxx_std_20)

# for Visual Studio/MSVC
set_target_properties(taxx-evasion PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})