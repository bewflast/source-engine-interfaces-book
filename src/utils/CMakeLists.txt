if(WIN32)
    set(UTILS_SOURCE_FILE src/windows/source-engine-interfaces-utils.cpp)
else()
    set(UTILS_SOURCE_FILE src/unix/source-engine-interfaces-utils.cpp)
endif()

add_library(source-engine-interfaces-utils STATIC  ${UTILS_SOURCE_FILE})
target_include_directories(source-engine-interfaces-utils PUBLIC include)
set_target_properties(source-engine-interfaces-utils PROPERTIES PUBLIC_HEADER include/utils/source-engine-interfaces-utils.hpp)