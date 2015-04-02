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

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

set(kpaface_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/libkpaface/FaceDetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/libkpaface/OpenCVFaceDetector.cpp
)

add_library(kpaface SHARED ${kpaface_SRCS})
target_link_libraries(kpaface ${QT_LIBRARIES})
target_link_libraries(kpaface ${OpenCV_LIBS})

set(test_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/ImageDisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/LibkfaceFaceDetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/test/DetectedFace.cpp
)

add_executable(test ${test_SRCS})
target_link_libraries(test ${QT_LIBRARIES})
target_link_libraries(test ${KFACE_LIBRARIES})
target_link_libraries(test ${OpenCV_LIBS})
target_link_libraries(test kpaface)
