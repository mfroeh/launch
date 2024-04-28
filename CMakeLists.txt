cmake_minimum_required(VERSION 3.17)

project(launch VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

find_package(Boost REQUIRED COMPONENTS system)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
find_package(rapidfuzz REQUIRED)
find_package(argparse REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(launch src/main.cc src/exec_mode.cc src/desktop_mode.cc src/hypr_mode.cc)

target_link_libraries(launch PRIVATE Boost::system ${GTKMM_LIBRARIES} rapidfuzz::rapidfuzz argparse::argparse nlohmann_json::nlohmann_json)

target_include_directories(launch PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_directories(launch PRIVATE ${GTKMM_LIBRARY_DIRS})
target_compile_options(launch PRIVATE ${GTKMM_CFLAGS_OTHER})

install(TARGETS launch DESTINATION bin)
