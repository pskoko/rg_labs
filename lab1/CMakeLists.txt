CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

#======================================================================
# Version
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

SET(CMAKE_CXX_STANDARD 17)  # enable C++11 standard 
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

PROJECT (BSpline)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(glfw3 3.2 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include )

add_executable(BSpline src/main.cpp src/glad.c src/Shader.cpp src/BSpline.cpp src/Model.cpp)
target_link_libraries(BSpline glfw dl) 
