cmake_minimum_required(VERSION 3.5.1)
project(cartridge)

set(SOURCE_FILES
  src/cartridge.cpp
  src/meta_cartridge.cpp
  )

add_library(cartridge STATIC ${SOURCE_FILES})

include_directories(
  ${CMAKE_SOURCE_DIR}/src/util/include
  ${CMAKE_SOURCE_DIR}/src/memory/include
)

target_include_directories(cartridge PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
