cmake_minimum_required(VERSION 3.15)

project(vxworks C CXX)

include_directories(include)

if (WIN32)
	add_subdirectory(submodules/pthread_win/)
	include_directories(submodules/pthread_win/include)
else()

endif()

add_library(vxworks STATIC src/taskLib.c src/sysLib.c src/semLib.c src/helpers.c src/msgQLib.c src/wdLib.c src/tickLib.c)
add_library(cpp_vxworks STATIC src/cpp/WatchDog.cpp
        src/cpp/VXConcurrentQueue.cpp
        include/cpp/VXConcurrentQueue.h
        src/cpp/Semaphore.cpp
        include/cpp/Semaphore.h
        include/cpp/Timestamp.h)

if (WIN32)
	target_link_libraries(vxworks pthread)
	target_link_libraries(cpp_vxworks vxworks pthread)
else()

endif()




