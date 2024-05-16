cmake_minimum_required(VERSION 3.10)
project(arduinoRc)

# General Settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable pthread support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Include directories
set(INCLUDE_DIRS "../include")

# Platform-specific settings
if(APPLE)
  # macos include paths
  set(OpenCV_DIR "/opt/homebrew/Cellar/opencv/4.9.0_8")
  set(OpenCV_INCLUDE_DIRS "/opt/homebrew/Cellar/opencv/4.9.0_8/include")
endif()

# Find OpenCV package
# find_library: find OpenCV's object files
# find_package: find OpenCV's CMakeLists.txt
find_package(OpenCV REQUIRED)

# Gather source files
file(GLOB SOURCES "src/*.cpp")

# Iterate over each source file to create an executable
foreach(SOURCE ${SOURCES})
  get_filename_component(EXE_NAME ${SOURCE} NAME_WE)
  add_executable(${EXE_NAME} ${SOURCE})

  # Link libraries
  target_link_libraries(${EXE_NAME} ${OpenCV_LIBS})

  # Include directories
  target_include_directories(${EXE_NAME} PUBLIC ${OpenCV_LIB_DIR} PUBLIC ${INCLUDE_DIRS})
endforeach()
