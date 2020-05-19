CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(PROJECT_NAME PCVIEWER)
PROJECT(${PROJECT_NAME})

SET(CMAKE_CXX_STANDARD 14)
enable_testing()

# Downloading necessary dependenies: Pangolin Sopuhs Eigen
execute_process(COMMAND git clone https://github.com/stevenlovegrove/Pangolin.git ${PROJECT_SOURCE_DIR}/thirdparty/Pangolin)
execute_process(COMMAND git clone https://github.com/strasdat/Sophus.git ${PROJECT_SOURCE_DIR}/thirdparty/Sophus)
# for Eigen we use wget and extract it since git clone is too slow
execute_process(COMMAND wget -O eigen-3.3.7.tar.bz2 http://bitbucket.org/eigen/eigen/get/3.3.7.tar.bz2)
execute_process(COMMAND tar -xvjf eigen-3.3.7.tar.bz2)
execute_process(COMMAND mv eigen-eigen-323c052e1731 ${PROJECT_SOURCE_DIR}/thirdparty/eigen)
execute_process(COMMAND rm eigen-3.3.7.tar.bz2)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
                     -Werror=delete-non-virtual-dtor \
                     -Werror=return-type \
                     -Werror=format-security \
                     -Werror=deprecated-declarations \
                     -Werror=delete-incomplete")

IF (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
    MESSAGE(FATAL_ERROR "The point cloud viewer requires GCC 5.0 or higher")
ENDIF ()

set(BUILD_TYPE Release)

# compiler flags
MESSAGE("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
ADD_COMPILE_OPTIONS(-O3 -g)

ADD_DEFINITIONS("-DENABLE_SSE")
ADD_DEFINITIONS("-DSOPHUS_DISABLE_ENSURES")

set(EXECUTABLE_OUTPUT_PATH bin)
set(LIBRARY_OUTPUT_PATH lib)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Disable compiling unnecessary components of Pangolin
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/Pangolin ${PROJECT_SOURCE_DIR}/thirdparty/Pangolin/build EXCLUDE_FROM_ALL)

# Sources files
set(pcViewer_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/Visualizer/KeyFrameDisplay.cpp
        ${PROJECT_SOURCE_DIR}/src/Visualizer/PangolinViewer.cpp
        ${PROJECT_SOURCE_DIR}/src/InputWrapper/ASlamData.cpp
        ${PROJECT_SOURCE_DIR}/src/InputWrapper/KeyFrameData.cpp
        ${PROJECT_SOURCE_DIR}/src/DataParser/DataParser.cpp
        ${PROJECT_SOURCE_DIR}/src/util/settings.cpp
        ${PROJECT_SOURCE_DIR}/src/util/Filesystem.cpp)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/thirdparty/eigen
        ${PROJECT_SOURCE_DIR}/thirdparty/Sophus
        ${PROJECT_SOURCE_DIR}/thirdparty/Pangolin/include
        ${PROJECT_SOURCE_DIR}/thirdparty/Pangolin/build/src/include
)

set(TargetLibs
        stdc++fs
        pangolin
        )
link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/thirdparty/Pangolin/build/src/lib)

# compile main library.
add_library(pcViewer ${pcViewer_SOURCE_FILES})

# build main executable
message("--- compiling pcViewer_dataset.")
add_executable(pcViewer_dataset ${PROJECT_SOURCE_DIR}/src/main_pcViewer.cpp)
target_link_libraries(pcViewer_dataset pcViewer ${TargetLibs})
