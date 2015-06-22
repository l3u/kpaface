find_package(OpenCV REQUIRED)

find_package(CLandmark REQUIRED)

set(kpaface_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/FaceDetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenCVFaceDetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FlandmarkDetector.cpp
)

add_library(kpaface SHARED ${kpaface_SRCS})
target_link_libraries(kpaface ${QT_LIBRARIES})
target_link_libraries(kpaface ${OpenCV_LIBS})
target_link_libraries(kpaface CLandmark::clandmark CLandmark::flandmark)
