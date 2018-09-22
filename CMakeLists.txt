cmake_minimum_required (VERSION 3.3.0)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.cpp
)

set(HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Game.h
)

project(Project1)

# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})

add_executable(Project1 ${HDRS} ${SRCS})

target_include_directories(
    Project1 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)
target_link_libraries(Project1 ${OGRE_LIBRARIES} ${OIS_LIBRARIES})

