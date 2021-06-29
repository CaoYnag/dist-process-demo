cmake_minimum_required(VERSION 3.10)

project(dpd)
message("dpd -- common definitions or tools.")

set(SRC
  ${PROJECT_SOURCE_DIR}/task.cpp
  ${PROJECT_SOURCE_DIR}/serializer.cpp
  ${PROJECT_SOURCE_DIR}/json_serializer.cpp
  ${PROJECT_SOURCE_DIR}/serializer_factory.cpp
  ${PROJECT_SOURCE_DIR}/utils.cpp
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
  
  find_package(Boost 1.67 COMPONENTS)
  if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
  endif()
endif()

add_library(${PROJECT_NAME} STATIC ${SRC})
