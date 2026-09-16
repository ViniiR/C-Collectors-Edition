cmake_minimum_required(VERSION 4.1)

project(stdce LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_EXTENSIONS ON)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.c"
)

# find_package(PkgConfig REQUIRED)

add_executable(
    ${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${INCLUDE_DIR}
)

# target_link_libraries(
#     ${PROJECT_NAME}
# )
