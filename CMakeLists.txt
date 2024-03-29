cmake_minimum_required(VERSION 2.8.11)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(neovim-qt VERSION 0.2.12.0)

INCLUDE(CPack)

if(NOT EXISTS ${NEOVIM_EXEC})
	set(NEOVIM_EXEC nvim)
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR
		${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -std=c++11 -Wfatal-errors")
endif()

option(USE_GCOV "Enable gcov support" OFF)
if(USE_GCOV)
	message(STATUS "Enabling coverage")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# to generate a compile_commands.json usable by ycm and other tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# konsole_wcwidth.h
include_directories(third-party)

# homebrew/macrpots/fink etc.
include(MacOSXPaths)

# Qt
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Test REQUIRED)

# msgpack
option(USE_SYSTEM_MSGPACK "Use system msgpack libraries " OFF)
if(USE_SYSTEM_MSGPACK)
	find_package(Msgpack REQUIRED)
else()
	add_subdirectory(third-party)
endif()
include_directories(${MSGPACK_INCLUDE_DIRS})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# Place targets in bin/ or lib/ for all build configurations
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(CFGNAME ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${CFGNAME} CFGNAME)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFGNAME} ${CMAKE_BINARY_DIR}/lib)
endforeach()

if(MSVC)
  # Allow use of deprecated function names in MSVC (read/write)
  add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()

add_subdirectory(src)
add_subdirectory(doc)

option(ENABLE_TESTS "Build tests" ON)
if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory(test)
	add_subdirectory(src/gui/shellwidget/test)
endif()

# Bindings
find_package(PythonInterp)
if (PYTHONINTERP_FOUND)
	set(NVIM "nvim" CACHE STRING "Path to nvim executable")
	add_custom_target(bindings
		COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/bindings/generate_bindings.py ${NVIM} ${CMAKE_SOURCE_DIR}/src/auto
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generating bindings"
		)

	add_custom_target(bindings-preview
		COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/bindings/generate_bindings.py ${NVIM}
		)
endif()


