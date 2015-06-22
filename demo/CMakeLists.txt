find_package(Kface REQUIRED)

set(demo_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ImageDisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LibkfaceFaceDetector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DetectedFace.cpp
)

add_executable(demo ${demo_SRCS})
target_link_libraries(demo ${QT_LIBRARIES})
target_link_libraries(demo ${KFACE_LIBRARIES})
target_link_libraries(demo kpaface)

add_custom_command(
    TARGET demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/INDIVIDUAL_FRONTAL_AFLW_SPLIT_1.xml ../
)
