cmake_minimum_required(VERSION 3.15)

project(vxworks C CXX)

include_directories(include)

if (MINGW)
	set(${IL} "pthread")
	find_package(Threads REQUIRED)
elseif (WIN32)
	set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
#	set(PTHREAD_BUILD_SHARED OFF)
#	set(PTHREAD_LIBRARY_TYPE STATIC)
	add_subdirectory(submodules/pthread_win/)
	include_directories(submodules/pthread_win/include)
	set(${IL} "ws2_32")
else()
	set(${IL} "pthread")
	find_package(Threads REQUIRED)
endif()

add_library(vxworks OBJECT src/taskLib.c src/sysLib.c src/semLib.c src/helpers.c src/msgQLib.c src/wdLib.c src/tickLib.c)
add_library(cpp_vxworks OBJECT src/cpp/WatchDog.cpp
        src/cpp/VXConcurrentQueue.cpp
        include/cpp/VXConcurrentQueue.h
        src/cpp/Semaphore.cpp
        include/cpp/Semaphore.h
        include/cpp/Timestamp.h)

#target_link_libraries(vxworks ${IL} pthread )
#target_link_libraries(cpp_vxworks ${IL} vxworks pthread)




