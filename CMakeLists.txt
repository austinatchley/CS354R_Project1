cmake_minimum_required (VERSION 3.3.0)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(OGRE_BUILD_SAMPLES TRUE)
set(OGRE_STATIC TRUE)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Ball.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SoundManager.cpp
)

set(HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Ball.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SoundManager.h
)

project(Project1)

# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${OGRE_CONFIG_DIR}/samples.cfg DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Project1 ${HDRS} ${SRCS})

find_package(SDL2) 
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)

find_package(SDL2_mixer QUIET) 
if (NOT SDL2MIXER_FOUND) # this is for the lab machines, where find_package doesn't work
    set(SDL2MIXER_LIBRARIES "/usr/lib/x86_64-linux-gnu/libSDL2_mixer-2.0.so.0")
endif (NOT SDL2MIXER_FOUND)

target_include_directories(
    Project1 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(Project1
    ${OGRE_LIBRARIES}
    ${OIS_LIBRARIES}
    ${SDL2_LIBRARIES}
    ${SDL2MIXER_LIBRARIES}
)
