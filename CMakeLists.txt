cmake_minimum_required( VERSION 3.13 )

project( Beato-Texture-Format-Compiler )

## STD 17
set( CMAKE_CXX_STANDARD 17 )


##
##
##
find_package( SDL3 REQUIRED )
find_package( SDL3_image REQUIRED )
find_package( SDL3_ttf REQUIRED )

##
## 
## Base include directories
include_directories( ${CMAKE_SOURCE_DIR}/source )
include_directories( ${CMAKE_SOURCE_DIR}/contrib/SDL3PlusPlus )

##
## crglLib 
## OpenGL helper lib
add_subdirectory( contrib/crglLib )
include_directories( ${CMAKE_SOURCE_DIR}/contrib/crglLib/include )

## 
## 
## Program Source

set( BTF_TEXTURE
    ${CMAKE_SOURCE_DIR}/source/BTFtexture/BTFStructures.hpp
    ${CMAKE_SOURCE_DIR}/source/BTFtexture/BTFLoader.cpp
    ${CMAKE_SOURCE_DIR}/source/BTFtexture/BTFLoader.hpp
    )

set( MAIN_SOURCES 
    ${CMAKE_SOURCE_DIR}/source/precompiled.hpp
    ${CMAKE_SOURCE_DIR}/source/main.cpp
    ${CMAKE_SOURCE_DIR}/source/main.hpp
    ${CMAKE_SOURCE_DIR}/source/SDLGLContext.cpp
    ${CMAKE_SOURCE_DIR}/source/SDLGLContext.hpp
    )

set( SOURCES
    ${BTF_TEXTURE}
    ${MAIN_SOURCES}
    ) 

##
##
## Executable 
add_executable( BTF-Edit ${SOURCES} )
target_link_libraries( BTF-Edit PUBLIC crglLib SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf )
target_precompile_headers( BTF-Edit PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/source/precompiled.hpp>" )
