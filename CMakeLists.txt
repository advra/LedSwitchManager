cmake_minimum_required(VERSION 3.5.1)
project(ledswitchexample)

add_executable(example.a 
    main.cpp
    ledswitchmanager.cpp
    ledswitchmanager.hpp
)
target_compile_features(example.a PRIVATE cxx_std_17)

target_include_directories(
    example.a PRIVATE
)
target_link_libraries(example.a LINK_PUBLIC 
    pthread
    wiringPi
)