cmake_minimum_required(VERSION 3.1)

project(ttds_example CXX C)

add_executable(coroutine_example1 example1.cpp)

target_include_directories(coroutine_example1 PUBLIC include )

if(CMAKE_COMPILER_IS_GNUCXX)
target_link_libraries(coroutine_example1 PUBLIC stdc++)
target_compile_options(coroutine_example1 PUBLIC -O2 )
target_compile_definitions(coroutine_example1 PUBLIC -D_FORTIFY_SOURCE=0 )
#target_link_options(coroutine_example1 PUBLIC -Wl,--stack=0x500000 )
endif()

if (MSVC)
target_link_options(coroutine_example1 PUBLIC /STACK:0x500000 )
endif()