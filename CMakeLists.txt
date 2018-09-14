cmake_minimum_required (VERSION 3.3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BasicTutorial1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GameObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Ball.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EventManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EntityManager.cpp
)
set(HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GameObject.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Ball.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EntityManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ComponentManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EventManager.h
)

project(OgreTutorialBootstrap)

## [discover_ogre]
# specify which version and components you need
find_package(OGRE 1.11 REQUIRED COMPONENTS Bites RTShaderSystem)

# copy resource.cfg next to our binaries where OGRE looks for it
file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_BINARY_DIR})
## [discover_ogre]

add_executable(Project1 ${HDRS} ${SRCS})
target_link_libraries(Project1 ${OGRE_LIBRARIES} ${OIS_LIBRARIES})
