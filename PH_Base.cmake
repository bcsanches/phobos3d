cmake_minimum_required( VERSION 3.0 )

project ( PH_Base CXX )

set( PROJECT_TARGET "${PROJECT_NAME}" )

set( SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )

set( RUNTIME_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib )
set( ARCHIVE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIR} ) # dll and exe

set( BOOST_DIR "" CACHE PATH "Path to boost dir." )

if ( BOOST_DIR STREQUAL "" )
    message( FATAL_ERROR "Boost dir was not correctly configured!" )
endif()

set( ALL_CONFIGS_DEF PH_BASE_EXPORTS PH_SDL )
set( DEBUG_CONFIG_DEF PH_CHECK_ASSERT ${ALL_CONFIGS_DEF} )
set( RELEASE_CONFIG_DEF PH_IGNORE_ASSERT ${ALL_CONFIGS_DEF} )

if ( MSVC )
    set( PROJECT_DEFINITIONS $<$<CONFIG:Debug>: ${DEBUG_CONFIG_DEF}>
                             $<$<CONFIG:Release>: ${RELEASE_CONFIG_DEF}> )
                             
    # this ensure that all configs will put bin/lib/archive files on right place
    # get from: http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2
    foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
        string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
        set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RUNTIME_OUTPUT_DIR} )
        set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBRARY_OUTPUT_DIR} )
        set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${ARCHIVE_OUTPUT_DIR} )
    endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
    
else()
    set( PROJECT_DEFINITIONS debug ${DEBUG_CONFIG_DEF}
                             optimized ${RELEASE_CONFIG_DEF} )
endif()

set( CMAKE_DEBUG_POSTFIX "_d" )

file( GLOB_RECURSE PROJECT_SOURCES "${SOURCE_DIR}/Base/*.c" 
                                   "${SOURCE_DIR}/Base/*.cpp" )
                                   
file( GLOB PROJECT_INCLUDES "${INCLUDE_DIR}/Phobos/*.h"
                            "${INCLUDE_DIR}/Phobos/*.hpp" )

include_directories( ${INCLUDE_DIR}
                     ${BOOST_DIR} )

add_library ( ${PROJECT_TARGET} SHARED ${PROJECT_SOURCES}
                                       ${PROJECT_INCLUDES} )

target_compile_definitions( ${PROJECT_TARGET} PRIVATE ${PROJECT_DEFINITIONS} )
