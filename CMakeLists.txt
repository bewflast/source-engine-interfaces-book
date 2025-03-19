cmake_minimum_required(VERSION 3.18)
project(SourceEngineInterfacesBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CONFIGURATION_TYPES Release Debug)



add_subdirectory(src/utils)

set(SOURCES 
            src/SourceEngineInterfacesBook.cpp
)

add_library(SourceEngineInterfacesBook STATIC  ${SOURCES})

set_target_properties(SourceEngineInterfacesBook PROPERTIES PUBLIC_HEADER 
    include/source-engine-interfaces-book/SourceEngineInterfacesBook.hpp
)

target_include_directories(SourceEngineInterfacesBook PUBLIC include)
target_link_libraries(SourceEngineInterfacesBook PRIVATE source-engine-interfaces-utils)
