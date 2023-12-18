cmake_minimum_required(VERSION 3.0)
project(CrossyRoady C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(ConsoleEngine)
file(GLOB sources CrossyRoady/*.cpp)
add_executable(${PROJECT_NAME} ${sources})

target_link_libraries(${PROJECT_NAME} PUBLIC ConsoleEngine)

target_include_directories(${PROJECT_NAME} PUBLIC 
"${PROJECT_SOURCE_DIR}/ConsoleEngine/include")

