cmake_minimum_required(VERSION 3.15)

project(deposit_manager VERSION 0.1 LANGUAGES CXX)



option(BUILD_QT "Enable Qt library" ON)

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
