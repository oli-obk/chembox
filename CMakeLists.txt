CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(ChemBox)

#Projects source files
SET(SRC_FILES
	src/main.cpp
    src/game_window.cpp
	)

#Projects headers files
SET(INC_FILES
    src/game_window.hpp
	include/optional.hpp
	)
INCLUDE_DIRECTORIES(include)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})

find_package(Gosu REQUIRED)

INCLUDE_DIRECTORIES(${Gosu_INCLUDE_DIRS})
LINK_DIRECTORIES(${Gosu_LIBRARY_DIRS})

set(CPPFLAGS ${CPPFLAGS} -D_GLIBCXX_DEBUG)

#Build
ADD_EXECUTABLE(ChemBox ${SRC_FILES})
set_target_properties(ChemBox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

IF(MSVC)
	SET_TARGET_PROPERTIES(ChemBox PROPERTIES COMPILE_FLAGS "/W4 /wd4127")
ENDIF(MSVC)
SET_TARGET_PROPERTIES(ChemBox PROPERTIES COMPILE_FLAGS "-std=c++0x -g -O2 -pedantic -Wall -Wextra")
TARGET_LINK_LIBRARIES(ChemBox ${Gosu_LIBRARIES})
