# Tests configuration
add_executable(cayene_tests
    decoder_test.cpp
)

target_link_libraries(cayene_tests
    PRIVATE
        cayene::decoder
        GTest::gtest
        GTest::gtest_main
        cayene_warnings
        cayene_sanitizers
)

include(GoogleTest)
gtest_discover_tests(cayene_tests)
