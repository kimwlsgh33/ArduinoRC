cmake_minimum_required(VERSION 3.10)
project(arduinoRc)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(APPLE)
  # macos include paths
  set(OpenCV_DIR "/opt/homebrew/Cellar/opencv/4.9.0_8")
  set(OpenCV_INCLUDE_DIRS "/opt/homebrew/Cellar/opencv/4.9.0_8/include")
  # elseif(UNIX AND NOT APPLE)
  #   # linux include paths
  #   set(OpenCV_DIR "/usr/")
  #   set(OpenCV_INCLUDE_DIRS "/usr/")
endif()
set(INCLUDE_DIRS "../include")
# find_library: find OpenCV's object files
# find_package: find OpenCV's CMakeLists.txt
find_package(OpenCV REQUIRED)

file(GLOB SOURCES "src/*.cpp")

foreach(SOURCE ${SOURCES})
  get_filename_component(EXE_NAME ${SOURCE} NAME_WE)
  add_executable(${EXE_NAME} ${SOURCE})
  target_link_libraries(${EXE_NAME} ${OpenCV_LIBS})
  target_include_directories(${EXE_NAME} PUBLIC ${OpenCV_LIB_DIR} PUBLIC ${INCLUDE_DIRS})
endforeach()
