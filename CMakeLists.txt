cmake_minimum_required(VERSION 3.21)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
project(mdtty VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Options (default ON when this is the top-level project)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(_mdtty_toplevel ON)
else()
  set(_mdtty_toplevel OFF)
endif()
option(MDTTY_BUILD_EXAMPLES "Build examples" ${_mdtty_toplevel})

# --- CPM dependencies ---
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME utf8proc
  GITHUB_REPOSITORY JuliaStrings/utf8proc
  VERSION 2.9.0
  OPTIONS "UTF8PROC_INSTALL ON"
)

# --- Generated version header ---
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.hpp.in
  ${CMAKE_CURRENT_BINARY_DIR}/include/mdtty/version.hpp
  @ONLY
)

# --- Warning flags ---
set(MDTTY_WARNINGS
  -Wall -Wextra -Wpedantic -Wconversion -Wshadow
  -Wdouble-promotion -Wformat=2 -Wnull-dereference
)

# --- Debug sanitizers ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

# --- Release LTO ---
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

# --- Library targets ---
foreach(_type STATIC SHARED)
  if(_type STREQUAL "STATIC")
    set(_target mdtty_static)
  else()
    set(_target mdtty_shared)
  endif()

  add_library(${_target} ${_type} src/mdtty.cpp)
  set_target_properties(${_target} PROPERTIES OUTPUT_NAME mdtty)

  if(_type STREQUAL "SHARED")
    set_target_properties(${_target} PROPERTIES
      VERSION   ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
    )
  endif()

  target_include_directories(${_target}
    PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  target_compile_features(${_target} PUBLIC cxx_std_23)
  target_compile_options(${_target} PRIVATE ${MDTTY_WARNINGS})
  target_link_libraries(${_target} PRIVATE $<BUILD_INTERFACE:utf8proc>)

  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${_target} PRIVATE -march=native)
  endif()
endforeach()

# --- Alias ---
add_library(mdtty::mdtty ALIAS mdtty_static)

# --- Install (only when built standalone) ---
if(_mdtty_toplevel)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  install(TARGETS mdtty_static mdtty_shared
    EXPORT mdttyTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(DIRECTORY include/mdtty DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/mdtty/version.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mdtty)

  install(EXPORT mdttyTargets
    NAMESPACE mdtty::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mdtty
  )

  configure_package_config_file(
    cmake/mdttyConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/mdttyConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mdtty
  )

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/mdttyConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
  )

  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/mdttyConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/mdttyConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mdtty
  )
endif()

# --- Subdirectories ---
if(MDTTY_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
