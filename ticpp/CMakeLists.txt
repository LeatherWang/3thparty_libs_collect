cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
project(ticpp)

# ==========================[ set cmake flags ]==========================
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS}")
endif()

message(WARNING "Build type: " ${CMAKE_BUILD_TYPE})

# ==========================[ set C/C++ options ]==========================
set(CMAKE_CXX_FLAGS "-std=c++11 -fPIC -Wall" )
SET(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")

# ==========================[ set version ]==========================
set(TICPP_VERSION_MAJOR 1)
set(TICPP_VERSION_MINOR 0)
set(TICPP_VERSION_REVISION 0)
set(TICPP_VERSION ${TICPP_VERSION_MAJOR}.${TICPP_VERSION_MINOR}.${TICPP_VERSION_REVISION})

# ==========================[ set rpath ]==========================
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/libs/${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# ==========================[ find package ]==========================
list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )
message("-- Found CMake Modules in ${PROJECT_SOURCE_DIR}/cmake_modules")

include_directories( 
    include
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Cmake's include directories is in:")
foreach(dir ${dirs})
    message(STATUS "  '${dir}'")
endforeach()

# ==========================[ set outpath ]==========================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_PROCESSOR})

message(STATUS "Archive is outputed to: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
message(STATUS "Library is outputed to: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "Executable is outputed to: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# ==========================[ add library ]==========================
AUX_SOURCE_DIRECTORY(./src/   SOURCE_FILES)

add_library(${PROJECT_NAME}_lib SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME}_lib PROPERTIES VERSION
#target_link_libraries(${PROJECT_NAME}_lib "")
${TICPP_VERSION} SOVERSION ${TICPP_VERSION_MAJOR}.${TICPP_VERSION_MINOR})

target_compile_definitions(${PROJECT_NAME}_lib
    PUBLIC TIXML_USE_TICPP
)

# ==========================[ add executable ]==========================
add_executable(rw_urdf  ./example/rw_urdf.cpp  )
target_link_libraries( rw_urdf
    ${PROJECT_NAME}_lib
)

# ==========================[ install ]==========================
install(TARGETS ${PROJECT_NAME}_lib
        RUNTIME DESTINATION bin/${PROJECT_NAME}/${CMAKE_SYSTEM_PROCESSOR}
        LIBRARY DESTINATION libs/${CMAKE_SYSTEM_PROCESSOR}
        ARCHIVE DESTINATION libs/${CMAKE_SYSTEM_PROCESSOR}
        PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION include/${PROJECT_NAME}
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

# ==========================[ export ]==========================
set(LW_EXPORT_PROJECT_NAME TICPP)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/CMakeConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${LW_EXPORT_PROJECT_NAME}Config.cmake" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LW_EXPORT_PROJECT_NAME}Config.cmake"
    DESTINATION "share/${PROJECT_NAME}")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/CMakeConfigVersion.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${LW_EXPORT_PROJECT_NAME}ConfigVersion.cmake" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${LW_EXPORT_PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "share/${PROJECT_NAME}")

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules # Install find_package scripts for dependencies.
        DESTINATION share/${PROJECT_NAME}
        FILES_MATCHING PATTERN "Find*.cmake")

# ==========================[ uninstall ]==========================
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/CMakeUninstall.cmake.in"
               "${CMAKE_CURRENT_BINARY_DIR}/CMakeUninstall.cmake"
               IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/CMakeUninstall.cmake)

# ==========================[ cpack ]==========================
include (InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "")
SET(CPACK_PACKAGE_VENDOR "leatherwang")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${TICPP_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${TICPP_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${TICPP_VERSION_REVISION}")
include (CPack)