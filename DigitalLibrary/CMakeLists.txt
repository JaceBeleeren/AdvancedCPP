# build using:
#   BOOST_ROOT=../boost_1_55_0 cmake .
# later verbose output from make:
#   make VERBOSE=1
# /usr/bin/c++ -I.../boost_1_55_0 -std=gnu++11 -o CMakeFiles/udpechoserver.dir/udpechoserver.cpp.o -c udpechoserver.cpp
# /usr/bin/c++ CMakeFiles/udpechoserver.dir/udpechoserver.cpp.o -o udpechoserver -rdynamic .../boost_1_55_0/stage/lib/libboost_system.so -Wl,-rpath,.../boost_1_55_0/stage/lib

# the property CXX_STANDARD is first mentioned in cmake 3.1.3
cmake_minimum_required (VERSION 3.1.3)

function(SOURCE_GROUP_BY_FOLDER parent sub target)
	SET(SOURCE_GROUP_DELIMITER "/")
    SET(last_dir "")
    SET(files "")
    FOREACH(file ${${target}})
        file(RELATIVE_PATH relative_file "${PROJECT_SOURCE_DIR}/${parent}" ${file})
        GET_FILENAME_COMPONENT(dir "${relative_file}" PATH)
        IF (NOT "${dir}" STREQUAL "${last_dir}")
            IF (files)
                SOURCE_GROUP("${parent}/${last_dir}/${sub}" FILES ${files})
            ENDIF (files)
            SET(files "")
        ENDIF (NOT "${dir}" STREQUAL "${last_dir}")
        SET(files ${files} ${file})
        SET(last_dir "${dir}")
    ENDFOREACH(file)
    IF (files)
        SOURCE_GROUP("${parent}/${last_dir}/${sub}" FILES ${files})
    ENDIF (files)
endfunction(SOURCE_GROUP_BY_FOLDER)

project (DigitalLibrary)

# internal list of mandatory boost components
SET(BOOST_MANDATORY_COMPONENTS system)

find_package(Boost "1.52.0" COMPONENTS ${BOOST_MANDATORY_COMPONENTS} REQUIRED)

# add include path for boost to the compiler
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#search for all header and source files and create filters depending on path and type
file(GLOB_RECURSE SERVER_SOURCE "${PROJECT_SOURCE_DIR}/Server/*.cpp" "${PROJECT_SOURCE_DIR}/Server/*.c")
SOURCE_GROUP_BY_FOLDER("Server" "SOURCE FILES" SERVER_SOURCE)
file(GLOB_RECURSE SERVER_HEADER "${PROJECT_SOURCE_DIR}/Server/*.hpp" "${PROJECT_SOURCE_DIR}/Server/*.h")
SOURCE_GROUP_BY_FOLDER("Server" "HEADER_FILES" SERVER_HEADER)

file(GLOB_RECURSE CLIENT_SOURCE "${PROJECT_SOURCE_DIR}/Client/*.cpp" "${PROJECT_SOURCE_DIR}/Client/*.c")
SOURCE_GROUP_BY_FOLDER("Client" "SOURCE FILES" CLIENT_SOURCE)
file(GLOB_RECURSE CLIENT_HEADER "${PROJECT_SOURCE_DIR}/Client/*.hpp" "${PROJECT_SOURCE_DIR}/Client/*.h")
SOURCE_GROUP_BY_FOLDER("Client" "HEADER_FILES" CLIENT_HEADER)

file(GLOB_RECURSE COMMON_SOURCE "${PROJECT_SOURCE_DIR}/Common/*.cpp" "${PROJECT_SOURCE_DIR}/Common/*.c")
SOURCE_GROUP_BY_FOLDER("Common" "SOURCE FILES" COMMON_SOURCE)
file(GLOB_RECURSE COMMON_HEADER "${PROJECT_SOURCE_DIR}/Common/*.hpp" "${PROJECT_SOURCE_DIR}/Common/*.h")
SOURCE_GROUP_BY_FOLDER("Common" "HEADER_FILES" COMMON_HEADER)

# executable name
set(EXEC_NAME "DigitalLibraryServer")

	add_executable(${EXEC_NAME} ${SERVER_SOURCE} ${SERVER_HEADER} ${COMMON_SOURCE} ${COMMON_HEADER})
	set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 11)
	set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD_REQUIRED true)

	# now link against the boost library system
	target_link_libraries(${EXEC_NAME} ${Boost_SYSTEM_LIBRARY})
	
set(EXEC_NAME "DigitalLibraryClient")

	add_executable(${EXEC_NAME} ${CLIENT_SOURCE} ${CLIENT_HEADER} ${COMMON_SOURCE} ${COMMON_HEADER})
	set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD 11)
	set_property(TARGET ${EXEC_NAME} PROPERTY CXX_STANDARD_REQUIRED true)

	# now link against the boost library system
	target_link_libraries(${EXEC_NAME} ${Boost_SYSTEM_LIBRARY})
	
message([STATUS] "If you add files to Common which are shared by all executebales don't forget to add them to all either manually or by rerunning CMake")