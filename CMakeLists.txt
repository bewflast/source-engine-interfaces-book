cmake_minimum_required(VERSION 3.18)
project(ifaces_book LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES Release Debug)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES 
            source/utils.cpp
            source/IFacesBook.cpp
)

add_library(ifacesbook OBJECT  ${SOURCES})

if(${CMAKE_SYSTEM} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "-m32")
endif()

target_include_directories(ifacesbook PUBLIC include)
