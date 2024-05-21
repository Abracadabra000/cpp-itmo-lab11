include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    search_tests
    search_tests.cpp
)

target_link_libraries(
    search_tests
    invert_index
    search_engine
    reader
    GTest::gtest_main
)

target_include_directories(search_tests PUBLIC ${PROJECT_SOURCE_DIR})

target_include_directories(search_tests PUBLIC ${PROJECT_SOURCE_DIR}/tests)

include(GoogleTest)

file(COPY ${PROJECT_SOURCE_DIR}/tests/testdir1 DESTINATION ${PROJECT_BINARY_DIR})

file(COPY ${PROJECT_SOURCE_DIR}/tests/testdir2 DESTINATION ${PROJECT_BINARY_DIR})

gtest_discover_tests(search_tests)