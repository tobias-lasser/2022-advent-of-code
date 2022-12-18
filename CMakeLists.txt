cmake_minimum_required(VERSION 3.23)
project(2022_advent_of_code)

find_package(fmt REQUIRED)
find_package(range-v3 REQUIRED)

add_executable(AoC01 01.cpp ParserHelper.h)
target_link_libraries(AoC01 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC01 PRIVATE cxx_std_20)

add_executable(AoC02 02.cpp ParserHelper.h)
target_link_libraries(AoC02 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC02 PRIVATE cxx_std_20)

add_executable(AoC03 03.cpp ParserHelper.h)
target_link_libraries(AoC03 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC03 PRIVATE cxx_std_20)

add_executable(AoC04 04.cpp ParserHelper.h)
target_link_libraries(AoC04 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC04 PRIVATE cxx_std_20)

add_executable(AoC05 05.cpp ParserHelper.h)
target_link_libraries(AoC05 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC05 PRIVATE cxx_std_20)

add_executable(AoC06 06.cpp ParserHelper.h)
target_link_libraries(AoC06 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC06 PRIVATE cxx_std_20)

add_executable(AoC07 07.cpp ParserHelper.h)
target_link_libraries(AoC07 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC07 PRIVATE cxx_std_20)

add_executable(AoC08 08.cpp ParserHelper.h)
target_link_libraries(AoC08 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC08 PRIVATE cxx_std_20)

add_executable(AoC09 09.cpp ParserHelper.h)
target_link_libraries(AoC09 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC09 PRIVATE cxx_std_20)

add_executable(AoC10 10.cpp ParserHelper.h)
target_link_libraries(AoC10 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC10 PRIVATE cxx_std_20)

add_executable(AoC11 11.cpp ParserHelper.h)
target_link_libraries(AoC11 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC11 PRIVATE cxx_std_20)

add_executable(AoC12 12.cpp ParserHelper.h)
target_link_libraries(AoC12 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC12 PRIVATE cxx_std_20)

add_executable(AoC13 13.cpp ParserHelper.h)
target_link_libraries(AoC13 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC13 PRIVATE cxx_std_20)

add_executable(AoC14 14.cpp ParserHelper.h)
target_link_libraries(AoC14 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC14 PRIVATE cxx_std_20)

add_executable(AoC15 15.cpp ParserHelper.h)
target_link_libraries(AoC15 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC15 PRIVATE cxx_std_20)

add_executable(AoC16 16.cpp ParserHelper.h)
target_link_libraries(AoC16 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC16 PRIVATE cxx_std_20)

add_executable(AoC17 17.cpp ParserHelper.h)
target_link_libraries(AoC17 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC17 PRIVATE cxx_std_20)

add_executable(AoC18 18.cpp ParserHelper.h)
target_link_libraries(AoC18 PRIVATE fmt::fmt range-v3::range-v3)
target_compile_features(AoC18 PRIVATE cxx_std_20)
