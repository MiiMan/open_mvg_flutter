add_library(openMVG_testing INTERFACE)
target_include_directories(openMVG_testing INTERFACE ${CMAKE_SOURCE_DIR};${EIGEN_INCLUDE_DIRS})
target_link_libraries(openMVG_testing INTERFACE openMVG_numeric)
