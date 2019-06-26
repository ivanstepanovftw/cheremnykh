cmake_minimum_required(VERSION 3.0)
project(cheremnykh)

find_package(Boost 1.45.0 REQUIRED COMPONENTS iostreams thread)
find_package(Threads REQUIRED)

#
# unit1
#
set(TARGET unit1_generator)
add_executable(${TARGET} unit1/generator.cpp)
set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 20
        )
target_compile_options(${TARGET} PUBLIC
        -O3
        -g3 -ggdb3
        )
target_link_libraries(${TARGET} PUBLIC
        stdc++fs
        )

set(TARGET unit1_sort)
add_executable(${TARGET} unit1/sort.cpp)
set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 20
        )
target_compile_options(${TARGET} PUBLIC
        -O3
        -g3 -ggdb3
        )
target_link_libraries(${TARGET} PUBLIC
        stdc++fs
        Boost::iostreams
        )


#
# unit2
#
set(TARGET unit2_shared_mutex)
add_executable(${TARGET} unit2/shared_mutex.cpp)
set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 20  # TODO change to C++11
#        CXX_STANDARD 11
        )
target_compile_options(${TARGET} PUBLIC
        -O3
        -g3 -ggdb3
        )
target_link_libraries(${TARGET} PUBLIC
        Threads::Threads
        Boost::boost
        Boost::thread
        )


#
# unit3
#
set(TARGET unit3_find_pattern)
add_executable(${TARGET} unit3/find_pattern.cpp)
set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 20
        )
target_compile_options(${TARGET} PUBLIC
        -O3
        -g3 -ggdb3
        )


#
# unit4 (IMPORTANT: library only)
#
set(TARGET unit4_db_cache)
#add_executable(${TARGET} unit4/db_shared_cached.cpp)
add_library(${TARGET} SHARED unit4/db_shared_cached.cpp)
set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 20
        )
target_compile_options(${TARGET} PUBLIC
        -O0
        -g3 -ggdb3
        )
target_link_libraries(${TARGET} PUBLIC
        Threads::Threads
        )
