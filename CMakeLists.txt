CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(ChemBox)

#Projects source files
SET(SRC_FILES
	src/main.cpp
    src/game_window.cpp
	src/game_window.hpp
	src/grid.hpp
	src/Particle.hpp
	src/machine.hpp
	src/machine.cpp
	src/particle_engine.cpp
	src/particle_engine.hpp

	src/machines/fourway_pipe.cpp
	src/machines/fourway_pipe.hpp
	src/machines/end_pipe.cpp
	src/machines/end_pipe.hpp
	)

#Projects headers files
SET(INC_FILES
    src/game_window.hpp
	include/optional.hpp
	)
INCLUDE_DIRECTORIES(include src)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})
SOURCE_GROUP("Build System" FILES CMakeLists.txt)

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
