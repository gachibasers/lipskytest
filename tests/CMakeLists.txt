add_executable(run_tests test.cpp)

target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(run_tests PRIVATE
        GTest::gtest_main
        GTest::gtest
)

add_test(NAME run_all_tests COMMAND run_tests)