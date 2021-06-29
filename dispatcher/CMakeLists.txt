cmake_minimum_required(VERSION 3.10)

project(dispatcher)
message(STATUS "task dispatcher.")

set(SRC
    ${PROJECT_SOURCE_DIR}/source.cpp
)

if(WIN32)
	if($ENV{LIB_BASE})
	else()
		include_directories(
			$ENV{LIB_BASE}/include
		)
		link_directories(
			$ENV{LIB_BASE}/lib
		)
	endif()
endif()
if(UNIX)
	add_compile_options("-static")
endif()

add_executable(${PROJECT_NAME} STATIC ${SRC})
target_link_libraries(${PROJECT_NAME} dpd zmq)
if(WIN32)
endif()
if(UNIX)
endif()