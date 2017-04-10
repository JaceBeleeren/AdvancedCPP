# build using:
#   BOOST_ROOT=../boost_1_55_0 cmake .
# later verbose output from make:
#   make VERBOSE=1
# /usr/bin/c++ -I.../boost_1_55_0 -std=gnu++11 -o CMakeFiles/udpechoserver.dir/udpechoserver.cpp.o -c udpechoserver.cpp
# /usr/bin/c++ CMakeFiles/udpechoserver.dir/udpechoserver.cpp.o -o udpechoserver -rdynamic .../boost_1_55_0/stage/lib/libboost_system.so -Wl,-rpath,.../boost_1_55_0/stage/lib

# the property CXX_STANDARD is first mentioned in cmake 3.1.3
cmake_minimum_required (VERSION 3.1.3)

project (DigitalLibrary)

# internal list of mandatory boost components
SET(BOOST_MANDATORY_COMPONENTS system)

find_package(Boost "1.52.0" COMPONENTS ${BOOST_MANDATORY_COMPONENTS} REQUIRED)

# add include path for boost to the compiler
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# executable name
set(EXEC_NAME "DigitalLibraryServer")

	file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/Server/*.cpp)
	file(GLOB_RECURSE HDR_FILES ${PROJECT_SOURCE_DIR}/Server/*.h)
	file(GLOB_RECURSE COMMON_SRC_FILES ${PROJECT_SOURCE_DIR}/Common/*.cpp)
	file(GLOB_RECURSE COMMON_HDR_FILES ${PROJECT_SOURCE_DIR}/Common/*.h)
	add_executable(${EXEC_NAME} ${SRC_FILES} ${HDR_FILES} ${COMMON_SRC_FILES} ${COMMON_HDR_FILES})
	set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 11)
	set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD_REQUIRED true)

	# now link against the boost library system
	target_link_libraries(${EXEC_NAME} ${Boost_SYSTEM_LIBRARY})
	
set(EXEC_NAME "DigitalLibraryClient")

	file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/Client/*.cpp)
	file(GLOB_RECURSE HDR_FILES ${PROJECT_SOURCE_DIR}/Client/*.h)
	file(GLOB_RECURSE COMMON_SRC_FILES ${PROJECT_SOURCE_DIR}/Common/*.cpp)
	file(GLOB_RECURSE COMMON_HDR_FILES ${PROJECT_SOURCE_DIR}/Common/*.h)
	add_executable(${EXEC_NAME} ${SRC_FILES} ${HDR_FILES} ${COMMON_SRC_FILES} ${COMMON_HDR_FILES})
	set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 11)
	set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD_REQUIRED true)

	# now link against the boost library system
	target_link_libraries(${EXEC_NAME} ${Boost_SYSTEM_LIBRARY})
