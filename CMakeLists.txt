cmake_minimum_required(VERSION 2.6)
project(Mako)

#########################################################################
## Find Libraries
#########################################################################

#find_path(JPEGLIB_INCLUDE_DIR jpeglib.h)
#find_library(JPEGLIB_LIB_PATH jpeglib.lib)

#find_path(PNGLIB_INCLUDE_DIR pnglib.h)
#find_library(PNGLIB_LIB_PATH pnglib.lib)

find_path(FREETYPE2_INCLUDE_DIR ft2build.h)
find_path(FREETYPE2_LIB_DIR freetype239MT.lib)

find_path(PHYSX_SDKS_DIR release_notes.html)
find_path(PHYSX_LIBS_DIR physx.lib)


#########################################################################
## Variables
#########################################################################
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Output)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Output)

set(MAKO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/zlib)
set(JPEGLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/jpeglib)
set(PNGLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source/pnglib)

add_definitions(-DUNICODE)

#########################################################################
## Macros
#########################################################################
macro(AddAllSubDirs)
	file(GLOB CONTENTS ./*)
	foreach(THING ${CONTENTS})
		if(IS_DIRECTORY ${THING})
			add_subdirectory(${THING})
		endif()
	endforeach(THING)
endmacro(AddAllSubDirs)

macro(CopyDir srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    @ONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(CopyDir)

#########################################################################
## Add Subdirectories
#########################################################################
add_subdirectory(Source)
add_subdirectory(Demos)
add_subdirectory(Extras)