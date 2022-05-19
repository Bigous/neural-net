cmake_minimum_required(VERSION 3.0.0)
project(neural-net VERSION 0.1.0)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 23)

include(CTest)
enable_testing()

add_executable(neural-net
	src/main.cpp
	src/utils/Img.cpp
	src/utils/ProgressBar.cpp
	src/deep_learning/NeuralNetwork.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
