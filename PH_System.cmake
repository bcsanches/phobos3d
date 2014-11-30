cmake_minimum_required( VERSION 3.0 )

project ( PH_System CXX )

set( PROJECT_TARGET "${PROJECT_NAME}" )

set( SOURCE_DIR ${PROJECT_SOURCE_DIR}/src )
set( INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include )

set( RUNTIME_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin )
set( LIBRARY_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib )
set( ARCHIVE_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/lib )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIR} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUNTIME_OUTPUT_DIR} ) # dll and exe

# cache variable are globals
set( BOOST_DIR "" CACHE PATH "Path to boost dir." )
set( PHOBOS_DEPS_DIR "" CACHE PATH "Path to phobos3d dependencies dir." )

set( PHOBOS_DEPS_LIB_DIR ${PHOBOS_DEPS_DIR}/lib )
set( PHOBOS_DEPS_BIN_DIR ${PHOBOS_DEPS_DIR}/bin )
set( SDL_INCLUDE_DIR ${PHOBOS_DEPS_DIR}/SDL-1.2.15/include )

if ( BOOST_DIR STREQUAL "" )
    message( FATAL_ERROR "Boost dir was not correctly configured!" )
endif()

if ( PHOBOS_DEPS_DIR STREQUAL "" )
    message( FATAL_ERROR "Phobos3d dependencies dir was not correctly configured!" )
endif()

# preprocessors
set( ALL_CONFIGS_DEFS PH_SYSTEM_EXPORTS PH_SDL )
set( DEBUG_CONFIG_DEFS PH_CHECK_ASSERT ${ALL_CONFIGS_DEFS} )
set( RELEASE_CONFIG_DEFS PH_IGNORE_ASSERT ${ALL_CONFIGS_DEFS} )

# libs
 set( ALL_CONFIGS_LIBS )
set( DEBUG_CONFIG_LIBS SDL_d ${ALL_CONFIGS_LIBS} )
set( RELEASE_CONFIG_LIBS SDL ${ALL_CONFIGS_LIBS} )

if ( MSVC )
    set( PROJECT_DEFINITIONS $<$<CONFIG:Debug>: ${DEBUG_CONFIG_DEFS}>
                             $<$<CONFIG:Release>: ${RELEASE_CONFIG_DEFS}> )
                             
    set( PROJECT_LIBS $<$<CONFIG:Debug>: ${DEBUG_CONFIG_LIBS}>
                      $<$<CONFIG:Release>: ${RELEASE_CONFIG_LIBS}> )
                      
    # this ensure that all configs will put bin/lib/archive files in right place
    # get from: http://stackoverflow.com/questions/7747857/in-cmake-how-do-i-work-around-the-debug-and-release-directories-visual-studio-2
    foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
        string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
        set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${RUNTIME_OUTPUT_DIR} )
        set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LIBRARY_OUTPUT_DIR} )
        set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${ARCHIVE_OUTPUT_DIR} )
    endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
    
else()
    set( PROJECT_DEFINITIONS debug ${DEBUG_CONFIG_DEFS}
                             optimized ${RELEASE_CONFIG_DEFS} )
                             
    set( PROJECT_LIBS debug ${DEBUG_CONFIG_LIBS}
                      optimized ${RELEASE_CONFIG_LIBS} )
endif()

set( CMAKE_DEBUG_POSTFIX "_d" )

file( GLOB PROJECT_SOURCES "${SOURCE_DIR}/System/*.c" 
                           "${SOURCE_DIR}/System/*.cpp"
                           "${SOURCE_DIR}/System/SDL/*.c"
                           "${SOURCE_DIR}/System/SDL/*.cpp" )
                                   
file( GLOB PROJECT_INCLUDES "${INCLUDE_DIR}/Phobos/System/*.h"
                            "${INCLUDE_DIR}/Phobos/System/*.hpp" )

include_directories( ${INCLUDE_DIR}
                     ${BOOST_DIR}
                     ${SDL_INCLUDE_DIR} )
                     
link_directories( ${PHOBOS_DEPS_LIB_DIR} )

add_library ( ${PROJECT_TARGET} SHARED ${PROJECT_SOURCES}
                                       ${PROJECT_INCLUDES} )

target_compile_definitions( ${PROJECT_TARGET} PRIVATE ${PROJECT_DEFINITIONS} )

target_link_libraries( ${PROJECT_TARGET}
                       ${PROJECT_LIBS}
                       PH_Base )
