cmake_minimum_required(VERSION 3.20)

project(SoftadastraRegistry
    VERSION 0.0.1
    LANGUAGES CXX
)

# =========================
# C++ standard & defaults
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(VIX_ENABLE_SANITIZERS "Enable ASan/UBSan (dev only)" OFF)
option(REGISTRY_BUILD_TESTS "Build tests" ON)
option(REGISTRY_BUILD_EXAMPLES "Build examples" OFF)

# ======================================================
# MySQLCppConn real imported target for Vix ORM
# ======================================================
if (NOT TARGET MySQLCppConn::MySQLCppConn)
    find_library(MYSQLCPPCONN_LIB
        NAMES mysqlcppconn8 mysqlcppconn
        PATHS
            /usr/lib
            /usr/lib/x86_64-linux-gnu
            /usr/local/lib
    )

    if (NOT MYSQLCPPCONN_LIB)
        message(FATAL_ERROR "MySQL Connector/C++ library not found. Install libmysqlcppconn-dev (or equivalent) and rerun CMake.")
    endif()

    add_library(MySQLCppConn::MySQLCppConn UNKNOWN IMPORTED)
    set_target_properties(MySQLCppConn::MySQLCppConn PROPERTIES
        IMPORTED_LOCATION "${MYSQLCPPCONN_LIB}"
    )
endif()

# =========================
# Prefixes (Vix install)
# =========================
list(APPEND CMAKE_PREFIX_PATH 
  "/usr/local"
  "/usr/local/lib/cmake"
  "/usr/local/lib/cmake/vix" 
  "/usr/local/lib/cmake/Vix" 
)

# =========================
# System deps
# =========================
find_package(Threads REQUIRED)
find_package(OpenSSL QUIET)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)

if (NOT TARGET Boost::filesystem)
  if (Boost_FOUND AND Boost_FILESYSTEM_LIBRARY)
    add_library(Boost::filesystem UNKNOWN IMPORTED)
    set_target_properties(Boost::filesystem PROPERTIES
      IMPORTED_LOCATION "${Boost_FILESYSTEM_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}")
    message(STATUS "Shim: Using Boost::filesystem from ${Boost_FILESYSTEM_LIBRARY}")
  else()
    message(FATAL_ERROR "Boost::filesystem not found and no library path provided.")
  endif()
endif()

# =========================
# fmt shim (before Vix)
# =========================
find_package(fmt QUIET)
if (TARGET fmt::fmt-header-only AND NOT TARGET fmt::fmt)
  add_library(fmt::fmt ALIAS fmt::fmt-header-only)
endif()
if (NOT TARGET fmt::fmt)
  add_library(fmt::fmt INTERFACE IMPORTED)
  target_include_directories(fmt::fmt INTERFACE "/usr/include" "/usr/local/include")
endif()

# =========================
# Vix (core + utils + json + websocket)
# =========================
set(vix_FOUND FALSE)
find_package(vix QUIET CONFIG) 
if (vix_FOUND)
  message(STATUS "Found vix (lowercase) package config")
else()
  find_package(Vix QUIET CONFIG)  
  if (Vix_FOUND)
    message(STATUS "Found Vix (legacy) package config")
    set(vix_FOUND TRUE)
  endif()
endif()

if (NOT vix_FOUND)
  message(FATAL_ERROR "Could not find Vix/vix package config. Set CMAKE_PREFIX_PATH to your install prefix.")
endif()

if (TARGET vix::vix)
  set(VIX_MAIN_TARGET vix::vix)
elseif (TARGET Vix::vix)
  set(VIX_MAIN_TARGET Vix::vix)
else()
  message(FATAL_ERROR "Neither vix::vix nor Vix::vix target exists. Check your Vix/vix installation.")
endif()

# =========================
# Sources / Includes
# =========================
set(REGISTRY_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(REGISTRY_SRC_DIR     "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(REGISTRY_CORE_SOURCES
    ${REGISTRY_SRC_DIR}/App.cpp

    ${REGISTRY_SRC_DIR}/http/HttpServer.cpp
    ${REGISTRY_SRC_DIR}/http/Routes.cpp
    ${REGISTRY_SRC_DIR}/http/Middleware.cpp

    ${REGISTRY_SRC_DIR}/domain/Package.cpp
    ${REGISTRY_SRC_DIR}/domain/Version.cpp
    ${REGISTRY_SRC_DIR}/domain/User.cpp
    ${REGISTRY_SRC_DIR}/domain/Token.cpp

    ${REGISTRY_SRC_DIR}/services/PackageService.cpp
    ${REGISTRY_SRC_DIR}/services/VersionService.cpp
    ${REGISTRY_SRC_DIR}/services/AuthService.cpp

    ${REGISTRY_SRC_DIR}/storage/LocalFileStorage.cpp
    ${REGISTRY_SRC_DIR}/storage/S3Storage.cpp  

    ${REGISTRY_SRC_DIR}/db/Database.cpp
    ${REGISTRY_SRC_DIR}/db/PackageRepository.cpp
    ${REGISTRY_SRC_DIR}/db/UserRepository.cpp
)

# =========================
# Core library
# =========================
add_library(registry_core STATIC
    ${REGISTRY_CORE_SOURCES}
)

target_include_directories(registry_core
    PUBLIC
        ${REGISTRY_INCLUDE_DIR}
)

target_link_libraries(registry_core
    PUBLIC
        ${VIX_MAIN_TARGET}
        Boost::system Boost::thread Boost::filesystem
        Threads::Threads
        fmt::fmt
)

if (TARGET vix::orm)
    message(STATUS "Linking registry_core with vix::orm (lowercase target)")
    target_link_libraries(registry_core PUBLIC vix::orm)
elseif (TARGET Vix::orm)
    message(STATUS "Linking registry_core with Vix::orm (legacy target)")
    target_link_libraries(registry_core PUBLIC Vix::orm)
else()
    message(STATUS "vix::orm not found, continuing without direct ORM linkage (for now).")
endif()

if (OpenSSL_FOUND)
  target_link_libraries(registry_core PUBLIC OpenSSL::SSL OpenSSL::Crypto)
endif()

if (UNIX AND NOT APPLE)
  target_link_libraries(registry_core PUBLIC dl)
endif()

if (MSVC)
  target_compile_options(registry_core PRIVATE /W4 /permissive-)
else()
  target_compile_options(registry_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (VIX_ENABLE_SANITIZERS AND NOT MSVC)
  target_compile_options(registry_core PRIVATE -O1 -g -fno-omit-frame-pointer -fsanitize=address,undefined)
  target_link_options(registry_core PRIVATE -fsanitize=address,undefined)
endif()

# =========================
# Executable (main)
# =========================
add_executable(registry
    ${REGISTRY_SRC_DIR}/main.cpp
)

target_link_libraries(registry
    PRIVATE
        registry_core
)

set_target_properties(registry PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    INSTALL_RPATH_USE_LINK_PATH ON
)

add_custom_target(run
  COMMAND $<TARGET_FILE:registry>
  DEPENDS registry
  USES_TERMINAL
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# =========================
# Tests
# =========================
if (REGISTRY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if (REGISTRY_BUILD_EXAMPLES)
    # add_subdirectory(examples) 
endif()
