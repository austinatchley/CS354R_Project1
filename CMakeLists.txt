cmake_minimum_required (VERSION 3.3.0)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/lib/x86_64-linux-gnu/")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SoundManager.cpp
)

set(HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SoundManager.h
)

project(Project1)

# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Project1 ${HDRS} ${SRCS})

#INCLUDE(FindPkgConfig)

#PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
#PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer)

find_package(SDL2) 
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)

find_package(SDL2_mixer QUIET) 
if (NOT SDL2MIXER_FOUND)
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

