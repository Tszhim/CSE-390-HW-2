cmake_minimum_required(VERSION 3.5)
project(Assignment_TA)

########### Do not change these lines #########
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
###############################################

# Find all header and cpp files. 
file(GLOB CONCRETE_SOURCES "../src/concrete/*.cpp")
file(GLOB MAIN_SOURCES "../src/main/*.cpp")
file(GLOB UTILS_SOURCES "../src/utils/*.cpp")
file(GLOB MAIN_SOURCE "../src/*.cpp")

set(SOURCES
    ${CONCRETE_SOURCES}
    ${MAIN_SOURCES}
    ${UTILS_SOURCES}
    ${MAIN_SOURCE}
)

file(GLOB ABSTRACT_HEADERS "../include/abstract/*.h")
file(GLOB CONCRETE_HEADERS "../include/concrete/*.h")
file(GLOB ENUMS_HEADERS "../include/enums/*.h")
file(GLOB MAIN_HEADERS "../include/main/*.h")
file(GLOB UTILS_HEADERS "../include/utils/*.h")

set(HEADERS
    ${ABSTRACT_HEADERS}
    ${CONCRETE_HEADERS}
    ${ENUMS_HEADERS}
    ${MAIN_HEADERS}
    ${UTILS_HEADERS}
)

# Compile found files.
add_executable(robot ${SOURCES} ${HEADERS})

# Send executable to root directory.
set_target_properties(robot
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../"
)

# Help compilation find imports.
target_include_directories(robot PUBLIC ../include/abstract)
target_include_directories(robot PUBLIC ../include/concrete)
target_include_directories(robot PUBLIC ../include/enums)
target_include_directories(robot PUBLIC ../include/main)
target_include_directories(robot PUBLIC ../include/utils)

# Custom clean-all command to delete build files and executable.
add_custom_target(clean-all
    COMMAND find ${CMAKE_BINARY_DIR} -mindepth 1 -not -name CMakeLists.txt -delete
    COMMAND ${CMAKE_COMMAND} -E remove "${CMAKE_CURRENT_SOURCE_DIR}/../robot"
    COMMENT "Cleaning up build files."
)

# Custom debug command to compile with debug symbols.
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target robot
    COMMENT "Building with debug symbols."
)
