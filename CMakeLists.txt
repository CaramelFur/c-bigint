# Yes the CMAKE file produces warnings, but it is just cmake being mad that not every standard was followed
# So for now it does not matter

project (bigint
  LANGUAGES C 
)

cmake_minimum_required (VERSION 3.5.1)

set (CMAKE_C_COMPILER gcc)

add_definitions("-Wall" "-Wextra" "-Wconversion" "-Wpedantic" "-ggdb" "-O3")

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} m)
