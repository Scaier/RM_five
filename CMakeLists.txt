cmake_minimum_required(VERSION 3.0.0)
project(task2.1 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(OpenCV REQUIRED)
find_package(Ceres REQUIRED)

aux_source_directory(./src ALL_SRCS)
include_directories(include
${OpenCV_INCLUDE_DIRS}
${CERES_INCLUDE_DIRS})
set(SRC ./cc)

add_executable(task2.1 main.cpp ${ALL_SRCS})
target_link_libraries(task2.1 ${OpenCV_LIBS} ${CERES_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
