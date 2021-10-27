cmake_minimum_required (VERSION 3.9)

project(SA VERSION      0.0.1
        DESCRIPTION "SA"
        HOMEPAGE_URL https://github.com/marwik15/stunning-adventure
        LANGUAGES CXX )

add_subdirectory(dependencies/googletest)

add_executable (SA "src/SA/main.cpp" )
add_executable (SA_test "src/test/main.cpp" )

if(MSVC)
    include (cmake/config/win-vs.cmake)
endif()

target_compile_features(SA PUBLIC cxx_std_17)
target_compile_features(SA_test PUBLIC cxx_std_17)

target_link_libraries(SA_test gtest_main)

target_include_directories(SA PUBLIC "include/")
target_include_directories(SA_test PUBLIC "include/" "dependencies/googletest/googletest/include")




