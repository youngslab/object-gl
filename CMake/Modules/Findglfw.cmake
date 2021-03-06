
#
#   Copyright 2013 Pixar
#
#   Licensed under the Apache License, Version 2.0 (the "Apache License")
#   with the following modification; you may not use this file except in
#   compliance with the Apache License and the following modification to it:
#   Section 6. Trademarks. is deleted and replaced with:
#
#   6. Trademarks. This License does not grant permission to use the trade
#      names, trademarks, service marks, or product names of the Licensor
#      and its affiliates, except as required to comply with Section 4(c) of
#      the License and to reproduce the content of the NOTICE file.
#
#   You may obtain a copy of the Apache License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the Apache License with the above modification is
#   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#   KIND, either express or implied. See the Apache License for the specific
#   language governing permissions and limitations under the Apache License.
#

# Try to find GLFW library and include path.
# Once done this will define
#
# GLFW_FOUND
# GLFW_INCLUDE_DIR
# GLFW_LIBRARIES
#


find_path(GLFW_INCLUDE_DIR 
    NAMES
        GLFW/glfw3.h
    HINTS
        "${GLFW_LOCATION}/include"
        "$ENV{GLFW_LOCATION}/include"
    PATHS
        "$ENV{PROGRAMFILES}/GLFW/include"
        "${OPENGL_INCLUDE_DIR}"
        /usr/openwin/share/include
        /usr/openwin/include
        "${GLFW_X11_INCLUDE_DIRS}"
        /opt/graphics/OpenGL/include
        /opt/graphics/OpenGL/contrib/libglfw
        /usr/local/include
        /usr/include/GL
        /usr/include
				/include
    DOC 
        "The directory where GLFW/glfw3.h resides"
)



if (WIN32)
    if(CYGWIN)
        find_library( GLFW_glfw_LIBRARY 
            NAMES
                glfw32
            HINTS
                "${GLFW_LOCATION}/lib"
                "${GLFW_LOCATION}/lib/x64"
                "$ENV{GLFW_LOCATION}/lib"
            PATHS
                "${OPENGL_LIBRARY_DIR}"
                /usr/lib
                /usr/lib/w32api
                /usr/local/lib
                "${GLFW_X11_LIB_DIRS}"
            DOC 
                "The GLFW library"
        )
    else()
        find_library( GLFW_glfw_LIBRARY
            NAMES 
                glfw32 
                glfw32s 
                glfw
                glfw3
            HINTS
                "${GLFW_LOCATION}/lib"
                "${GLFW_LOCATION}/lib/x64"
                "${GLFW_LOCATION}/lib-msvc110"
                "${GLFW_LOCATION}/lib-vc2012"
                "$ENV{GLFW_LOCATION}/lib"
                "$ENV{GLFW_LOCATION}/lib/x64"
                "$ENV{GLFW_LOCATION}/lib-msvc110"
                "$ENV{GLFW_LOCATION}/lib-vc2012"
            PATHS
                "$ENV{PROGRAMFILES}/GLFW/lib"
                "${OPENGL_LIBRARY_DIR}"
            DOC 
                "The GLFW library"
        )
    endif()
else ()
    if (APPLE)
        find_library( GLFW_glfw_LIBRARY glfw
            NAMES 
                glfw
                glfw3
            HINTS
                "${GLFW_LOCATION}/lib"
                "${GLFW_LOCATION}/lib/cocoa"
                "$ENV{GLFW_LOCATION}/lib"
                "$ENV{GLFW_LOCATION}/lib/cocoa"
            PATHS
                /usr/local/lib
        )
        set(GLFW_cocoa_LIBRARY "-framework Cocoa" CACHE STRING "Cocoa framework for OSX")
        set(GLFW_corevideo_LIBRARY "-framework CoreVideo" CACHE STRING "CoreVideo framework for OSX")
        set(GLFW_iokit_LIBRARY "-framework IOKit" CACHE STRING "IOKit framework for OSX")
    else ()
        # (*)NIX
			 # WAYLANDS
       find_package(ECM)

       if(${ECM_FOUND})
          list(APPEND CMAKE_MODULE_PATH "${ECM_MODULE_PATH}")
          find_package(Wayland)
        endif()

        find_package(Threads REQUIRED)

        find_library( GLFW_glfw_LIBRARY
            NAMES 
                glfw
                glfw3
            HINTS
                "${GLFW_LOCATION}/lib"
                "$ENV{GLFW_LOCATION}/lib"
                "${GLFW_LOCATION}/lib/x11"
                "$ENV{GLFW_LOCATION}/lib/x11"
            PATHS
								/lib
                /usr/lib64
                /usr/lib
                /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}
                /usr/local/lib64
                /usr/local/lib
                /usr/local/lib/${CMAKE_LIBRARY_ARCHITECTURE}
                /usr/openwin/lib
                "${GLFW_X11_LIB_DIRS}"
            DOC 
                "The GLFW library"
        )
    endif (APPLE)
endif (WIN32)

set( GLFW_FOUND "NO" )

if(GLFW_INCLUDE_DIR)

    if(GLFW_glfw_LIBRARY)
        set( GLFW_LIBRARIES "${GLFW_glfw_LIBRARY}"
                            "${GLFW_x11_LIBRARY}"
                            "${GLFW_cocoa_LIBRARY}"
                            "${GLFW_iokit_LIBRARY}"
                            "${GLFW_corevideo_LIBRARY}"
														Threads::Threads)
        set( GLFW_FOUND "YES" )
        set (GLFW_LIBRARY "${GLFW_LIBRARIES}")
        set (GLFW_INCLUDE_PATH "${GLFW_INCLUDE_DIR}")
    endif(GLFW_glfw_LIBRARY)


    # Tease the GLFW_VERSION numbers from the lib headers
    function(parseVersion FILENAME VARNAME)
            
        set(PATTERN "^#define ${VARNAME}.*$")
        
        file(STRINGS "${GLFW_INCLUDE_DIR}/${FILENAME}" TMP REGEX ${PATTERN})
        
        string(REGEX MATCHALL "[0-9]+" TMP ${TMP})
        
        set(${VARNAME} ${TMP} PARENT_SCOPE)
        
    endfunction()


    if(EXISTS "${GLFW_INCLUDE_DIR}/GL/glfw.h")

        parseVersion(GL/glfw.h GLFW_VERSION_MAJOR)
        parseVersion(GL/glfw.h GLFW_VERSION_MINOR)
        parseVersion(GL/glfw.h GLFW_VERSION_REVISION)

    elseif(EXISTS "${GLFW_INCLUDE_DIR}/GLFW/glfw3.h")

        parseVersion(GLFW/glfw3.h GLFW_VERSION_MAJOR)
        parseVersion(GLFW/glfw3.h GLFW_VERSION_MINOR)
        parseVersion(GLFW/glfw3.h GLFW_VERSION_REVISION)
 
    endif()

    if(${GLFW_VERSION_MAJOR} OR ${GLFW_VERSION_MINOR} OR ${GLFW_VERSION_REVISION})
        set(GLFW_VERSION "${GLFW_VERSION_MAJOR}.${GLFW_VERSION_MINOR}.${GLFW_VERSION_REVISION}")
        set(GLFW_VERSION_STRING "${GLFW_VERSION}")
        mark_as_advanced(GLFW_VERSION)
    endif()
    
endif(GLFW_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)

mark_as_advanced(
  GLFW_INCLUDE_DIR
  GLFW_LIBRARIES
  GLFW_glfw_LIBRARY
  GLFW_cocoa_LIBRARY
)

find_package_handle_standard_args(GLFW 
    REQUIRED_VARS
        GLFW_INCLUDE_DIR
        GLFW_LIBRARIES
    VERSION_VAR
        GLFW_VERSION
)

message("GLFW_INCLUDE_DIR: " ${GLFW_INCLUDE_DIR} )
message("GLFW_LIBRARIES: " ${GLFW_LIBRARIES} )

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.0")
	if(NOT TARGET glfw::glfw)
		if(${GLFW_FOUND} )
			add_library(glfw::glfw INTERFACE IMPORTED)
			set_property(TARGET glfw::glfw PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${GLFW_INCLUDE_DIR}")
			set_property(TARGET glfw::glfw PROPERTY INTERFACE_LINK_LIBRARIES
				${GLFW_LIBRARIES} Wayland::Client)
		elseif(TARGET glfw)
			set(GLFW_FOUND TRUE)
			add_library(glfw::glfw INTERFACE IMPORTED)
			target_link_libraries(glfw::glfw INTERFACE glfw)
		else()
		endif()
	endif()
endif()

