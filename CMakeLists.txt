cmake_minimum_required(VERSION 3.16)
project(OSExp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC Currently refused to enable floor2 due to absence of CPU optimize. But we just need it.
if (MSVC)
    add_definitions(-D__cpp_lib_bitops=201907L)
endif()

# MSVC doesn't set __cplusplus by default.
if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    add_compile_options("/Zc:__cplusplus")
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" AND "x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC")
    message(WARNING "Clang currently doesn't support MSVC concepts so it won't compile. If you sure situation has changed, ignore this message.")
endif()

add_executable(ProcessSchedule mock_proc_schedule.cpp proc_schedule.cpp proc_schedule.h misc.h)
add_executable(MemoryAllocator mock_mem_alloc.cpp mem_alloc.cpp mem_alloc.h misc.h)
add_executable(DiskFileSystem mock_disk_fs.cpp disk_fs.cpp disk_fs.h misc.h)

if(WIN32)
    add_executable(ProcessCreate process_create_win.cpp)
else()
    add_executable(ProcessCreate process_create.cpp)
endif()

add_executable(BankerAlgorithm banker_alg.cpp)

if (CLANG_FORCE_LIBCPP)
    if (CMAKE_CXX_COMPILER_ID MATCHES "[cC][lL][aA][nN][gG]") #Case insensitive match
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
    else ()
        message(FATAL_ERROR "Not using clang. Can't use libc++.")
    endif()
endif()