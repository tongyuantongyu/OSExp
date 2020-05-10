cmake_minimum_required(VERSION 3.16)
project(OSExp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    add_compile_options("/Zc:__cplusplus")
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