cmake_minimum_required(VERSION 3.17)

project(nix-c++-template VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
    ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

find_package(Boost REQUIRED COMPONENTS system)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

add_executable(nix-c++-template src/main.cc src/helloworld.cc)

target_link_libraries(nix-c++-template PRIVATE Boost::system ${GTKMM_LIBRARIES})

target_include_directories(nix-c++-template PRIVATE ${GTKMM_INCLUDE_DIRS})
target_link_directories(nix-c++-template PRIVATE ${GTKMM_LIBRARY_DIRS})
target_compile_options(nix-c++-template PRIVATE ${GTKMM_CFLAGS_OTHER})

install(TARGETS nix-c++-template DESTINATION bin)
