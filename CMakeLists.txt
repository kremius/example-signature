cmake_minimum_required(VERSION 3.12)
project(ExampleSignature)

set(CMAKE_CXX_STANDARD 17)
set(MAIN_FILE "bin/main.cpp")

set(SOURCES
    "lib/signature.cpp"
)
set(HEADERS
    "lib/signature.h"
)
set(TESTS_SOURCES
    "tests/signature_tests.cpp"
)

set(STRICT_OPTIONS -Wall -Wextra -Werror -ansi -Wno-address)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Seems like conan can't into pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# All src object files
add_library(example-signature-objects OBJECT ${SOURCES} ${HEADERS})
target_compile_options(example-signature-objects PRIVATE ${STRICT_OPTIONS})

# Main executable
add_executable(example-signature ${MAIN_FILE} $<TARGET_OBJECTS:example-signature-objects>)
target_include_directories(example-signature PRIVATE "lib/")
target_compile_options(example-signature PRIVATE ${STRICT_OPTIONS})
conan_target_link_libraries(example-signature)
target_link_libraries(example-signature Threads::Threads)

# Executable with tests
add_executable(example-signature-tests ${TESTS_SOURCES} $<TARGET_OBJECTS:example-signature-objects>)
target_include_directories(example-signature-tests PRIVATE "lib/")
target_compile_options(example-signature-tests PRIVATE ${STRICT_OPTIONS})
set(GTEST_LIBRARIES_LIST
    gtest
    gmock
    gmock_main
)
conan_target_link_libraries(example-signature-tests)
target_link_libraries(example-signature-tests Threads::Threads)
