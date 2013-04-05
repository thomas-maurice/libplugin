CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(libplugin)

INCLUDE_DIRECTORIES("include" "include/libplugin")
LINK_DIRECTORIES(${PROJECT_BINARY_DIR})

INCLUDE(cmake/FindGZIP.cmake)

SET(CMAKE_CXX_FLAGS "-Wall -O3 -fPIC -s")
SET(BINARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
# Man
SET(MAN_PAGES "man/libplugin.3")

INCLUDE(cmake/BuildMan.cmake)

FILE(
	GLOB_RECURSE
	LIB_SOURCES
	src/*.cpp
	include/*.h
)

# Common files
ADD_LIBRARY(
	libplugin_common
	OBJECT
	${LIB_SOURCES}
)

# Shared lib
ADD_LIBRARY(        
	libplugin
	SHARED
	$<TARGET_OBJECTS:libplugin_common>
)
SET_TARGET_PROPERTIES(
	libplugin
	PROPERTIES
	PREFIX ""
)

# Static lib
ADD_LIBRARY(        
	libplugin_static
	STATIC
	$<TARGET_OBJECTS:libplugin_common>
)
SET_TARGET_PROPERTIES(
	libplugin_static
	PROPERTIES
	OUTPUT_NAME "plugin"
	PREFIX ""
)

# Exemple module
ADD_LIBRARY(        
	mod
	SHARED
	exemples/mod.cpp
)

# Configure the mod
SET_TARGET_PROPERTIES(
	mod
	PROPERTIES
	PREFIX ""
)

# Executable loader
ADD_EXECUTABLE(
	loader
	exemples/loader.cpp
)

# Dependencies
ADD_DEPENDENCIES(loader libplugin)
ADD_DEPENDENCIES(mod loader libplugin)
ADD_DEPENDENCIES(man mod)

TARGET_LINK_LIBRARIES(
	loader
	plugin
	dl
)

# Linking the loader to the lib
TARGET_LINK_LIBRARIES(
	mod
	plugin
	dl
)
