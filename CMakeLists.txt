# cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
cmake_minimum_required(VERSION 3.8) # Ensure a modern CMake version

set(CMAKE_CXX_STANDARD 14) # Force C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Ensure it is enforced

project(PS)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB MACRO_FILES
	"*.mac"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(sim sim.cc ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})

add_custom_target(PS DEPENDS halld)

find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIRS})

target_link_libraries(sim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
