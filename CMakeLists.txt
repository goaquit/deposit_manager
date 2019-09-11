cmake_minimum_required(VERSION 3.14)

option(VCPKG_ENABLE "Use vcpkg manager" ON)
if(VCPKG_ENABLE)

   set(VCPKG_ROOT CACHE PATH "")

   if(DEFINED ENV{VCPKG_ROOT})

      set(VCPKG_ROOT $ENV{VCPKG_ROOT})

   endif()

   if(NOT DEFINED VCPKG_ROOT)
      message(FATAL_ERROR "VCPKG_ROOT variable environment not set. Please set the path to the root directory vcpkg.")
   endif()

   set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()



project(deposit_manager VERSION 0.1 LANGUAGES CXX)



set(THIRD_PARTY_DIR "" CACHE PATH "Path to third party library directory")

option(BUILD_QT "Enable Qt library" OFF)

if(BUILD_QT)

   set(CMAKE_AUTOMOC ON)
   SET(CMAKE_AUTOUIC ON)

   find_package(Qt5 COMPONENTS Core REQUIRED)   

endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 17)

include(GenerateExportHeader)

option(USE_GNU_INSTALL_DIR "Use GNU standard installation directories" ON)

if(USE_GNU_INSTALL_DIR)
   include(GNUInstallDirs)

   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

endif()

if(UNIX)

   if(CMAKE_BUILD_TYPE STREQUAL Debug)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
   endif()

elseif(WIN32)

endif()


add_subdirectory("projects")

option(BUILD_LIBRARY "Building library" ON)
if(BUILD_LIBRARY)
   add_subdirectory("library")

endif()

option(BUILD_TEST "Building test" ON)
if(BUILD_TEST)
   enable_testing()
   include(CTest)
   set(GTEST_ROOT ${THIRD_PARTY_DIR} CACHE PATH "Path to gtest library")
   find_package(GTest REQUIRED)
   include_directories(${GTEST_INCLUDE_DIRS})


   find_package(cpprestsdk CONFIG REQUIRED)


   add_subdirectory("tests")

endif()
