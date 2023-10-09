if(WIN32)
    set(UTILS_SOURCE_FILE windows.cpp)
else()
    set(UTILS_SOURCE_FILE linux.cpp)
endif()

add_library(ifacesbook-utils STATIC  ${UTILS_SOURCE_FILE})
target_include_directories(ifacesbook-utils PUBLIC include)
set_target_properties(ifacesbook-utils PROPERTIES PUBLIC_HEADER include/utils/utils.hpp)