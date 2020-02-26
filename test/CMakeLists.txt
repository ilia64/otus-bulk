find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(TEST_NAME project_test)

add_executable(${TEST_NAME} test.cpp)

set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
        )

set_target_properties(${TEST_NAME} PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

target_link_libraries(${TEST_NAME} ${Boost_LIBRARIES})

add_test(${TEST_NAME} ${TEST_NAME})

enable_testing()
