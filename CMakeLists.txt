cmake_minimum_required(VERSION 3.18)
project(ifaces_book LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES Release Debug)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(source/utils)

set(SOURCES 
            source/IFacesBook.cpp
)

add_library(ifacesbook STATIC  ${SOURCES})

set_target_properties(ifacesbook PROPERTIES PUBLIC_HEADER include/source-engine-interfaces-book/IFacesBook.hpp)
target_include_directories(ifacesbook PUBLIC include)
target_link_libraries(ifacesbook PRIVATE ifacesbook-utils)
