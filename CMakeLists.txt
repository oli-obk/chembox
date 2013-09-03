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
	src/rotatable_machine.hpp
	src/rotatable_machine.cpp
    src/rotatable_versioned_machine.hpp
    src/rotatable_versioned_machine.cpp
	src/machine_factory.hpp
	src/machine_factory.cpp

	src/machines/pipe.cpp
	src/machines/pipe.hpp
    src/machines/straight_pipe.cpp
    src/machines/straight_pipe.hpp
    src/machines/turn_pipe.cpp
    src/machines/turn_pipe.hpp
    src/machines/t_pipe.cpp
    src/machines/t_pipe.hpp
	src/machines/pump.cpp
	src/machines/pump.hpp
	src/machines/end_pipe.cpp
	src/machines/end_pipe.hpp

    src/Effects.hpp
    src/Effects.cpp

    src/imagestore.hpp

    lib/GosuParticleEngine/src/fast_math.cpp
    lib/GosuParticleEngine/src/fast_math.hpp

    lib/GosuParticleEngine/src/ParticleEmitter.cpp
    lib/GosuParticleEngine/src/ParticleEmitter.hpp
    lib/GosuParticleEngine/src/Particle.cpp
    lib/GosuParticleEngine/src/Particle.hpp
	)

#Projects headers files
SET(INC_FILES
    src/game_window.hpp
	include/optional.hpp
	src/ConnectorDir.hpp
	)
INCLUDE_DIRECTORIES(
include
src
lib/GosuParticleEngine/src/
)

#"Sources" and "Headers" are the group names in Visual Studio.
#They may have other uses too...
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})
SOURCE_GROUP("Build System" FILES CMakeLists.txt)

find_package(Gosu REQUIRED)

INCLUDE_DIRECTORIES(${Gosu_INCLUDE_DIRS})
LINK_DIRECTORIES(${Gosu_LIBRARY_DIRS})

set(CMAKE_CXX_COMPILER g++-4.8)
#set(CPPFLAGS ${CPPFLAGS} -D_GLIBCXX_DEBUG)

#Build
ADD_EXECUTABLE(ChemBox ${SRC_FILES})
set_target_properties(ChemBox PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

IF(MSVC)
	SET_TARGET_PROPERTIES(ChemBox PROPERTIES COMPILE_FLAGS "/W4 /wd4127")
ENDIF(MSVC)
SET_TARGET_PROPERTIES(ChemBox PROPERTIES COMPILE_FLAGS "-std=c++0x -g -pedantic -Wall -Wextra")
TARGET_LINK_LIBRARIES(ChemBox ${Gosu_LIBRARIES})
