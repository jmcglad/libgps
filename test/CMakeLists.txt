find_package(Catch2 3 REQUIRED)
add_executable(tests test_gps.cpp benchmark_gps.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain PRIVATE ${PROJECT_NAME})

include(CTest)
include(Catch)
catch_discover_tests(tests)
