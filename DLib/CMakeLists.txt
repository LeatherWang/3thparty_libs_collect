cmake_minimum_required( VERSION 2.8 )
project( lw_dlib )

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
set(LW_DLib_VERSION_MAJOR 1)
set(LW_DLib_VERSION_MINOR 0)
set(LW_DLib_VERSION_REVISION 0)
set(LW_DLib_VERSION ${LW_DLib_VERSION_MAJOR}.${LW_DLib_VERSION_MINOR}.${LW_DLib_VERSION_REVISION})

# ==========================[ set rpath ]==========================
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/")
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
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/${CMAKE_SYSTEM_PROCESSOR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_SYSTEM_PROCESSOR})

message(STATUS "Archive is outputed to: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
message(STATUS "Library is outputed to: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "Executable is outputed to: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# ==========================[ add library ]==========================
AUX_SOURCE_DIRECTORY(./src/   SOURCE_FILES)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION
        ${LW_DLib_VERSION} SOVERSION ${LW_DLib_VERSION_MAJOR}.${LW_DLib_VERSION_MINOR})
#target_link_libraries(${PROJECT_NAME} "")

# ==========================[ install ]==========================
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin/${PROJECT_NAME}/${CMAKE_SYSTEM_PROCESSOR}
        LIBRARY DESTINATION lib/
        ARCHIVE DESTINATION lib/
        PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)
INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION include/${PROJECT_NAME}
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ)

# ==========================[ export ]==========================
set(LW_EXPORT_PROJECT_NAME LW_DLib)
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
option(CPACK_GENERATE_DEB "generate .deb or .tar.gz" ON)

string(TOLOWER "${PROJECT_NAME}" PROJECT_NAME_LOWERCASE)
find_program(DPKG_PROGRAM dpkg DOC "dpkg program of Debian-based systems")
if(DPKG_PROGRAM)
  execute_process(
    COMMAND ${DPKG_PROGRAM} --print-architecture
    OUTPUT_VARIABLE CPACK_DEBIAN_PACKAGE_ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif(DPKG_PROGRAM)

find_program(LSB_RELEASE_PROGRAM lsb_release DOC "lsb_release program of Debian-based systems")
if(LSB_RELEASE_PROGRAM)
  execute_process(COMMAND ${LSB_RELEASE_PROGRAM} -rs
    OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif(LSB_RELEASE_PROGRAM)

string(TIMESTAMP PROJECT_VERSION_REVISION "%Y%m%d%H%M")

if(CPACK_GENERATE_DEB)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "PG.Wang <leatherwang@foxmail.com>")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS ${DEBIAN_DEPENDS})
endif()

include(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ DLib.")
SET(CPACK_PACKAGE_VENDOR "PG.Wang")
SET(CPACK_PACKAGE_VERSION_MAJOR "${LW_DLib_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${LW_DLib_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${LW_DLib_VERSION_REVISION}-${PROJECT_VERSION_REVISION}~${LSB_RELEASE_ID_SHORT}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME_LOWERCASE}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
include(CPack)

