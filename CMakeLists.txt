cmake_minimum_required(VERSION 2.8)

project(libkpaface)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

find_package(OpenCV REQUIRED)

find_package(Qt4 COMPONENTS QtCore QtGui REQUIRED)
include(${QT_USE_FILE})
add_definitions(
    ${QT_DEFINITIONS}
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_TO_ASCII
)
set(CMAKE_AUTOMOC ON)

find_package(Kface REQUIRED)

find_package(CLandmark REQUIRED)
include_directories(${FLANDMARK_INCLUDE_DIRS} ${CLANDMARK_INCLUDE_DIRS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(kpaface_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/libkpaface/FaceDetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libkpaface/OpenCVFaceDetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libkpaface/FlandmarkDetector.cpp
)

add_library(kpaface SHARED ${kpaface_SRCS})
target_link_libraries(kpaface ${QT_LIBRARIES})
target_link_libraries(kpaface ${OpenCV_LIBS})
target_link_libraries(kpaface CLandmark::clandmark CLandmark::flandmark)

set(demo_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demo/demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demo/ImageDisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demo/LibkfaceFaceDetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demo/DetectedFace.cpp
)

add_executable(demo ${demo_SRCS})
target_link_libraries(demo ${QT_LIBRARIES})
target_link_libraries(demo ${KFACE_LIBRARIES})
target_link_libraries(demo kpaface)
