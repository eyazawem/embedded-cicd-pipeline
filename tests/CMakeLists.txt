find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(unit_tests
    test_moving_average.cpp
    test_signal_validator.cpp)
target_link_libraries(unit_tests PRIVATE cicd_core cicd_warnings GTest::gtest_main)

gtest_discover_tests(unit_tests)
