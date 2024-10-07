# CMakeList.txt : CMake project for StockDiffusionModel, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("StockDiffusionModel")
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
	sfml
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)


# Add source to this project's executable.
add_executable (StockDiffusionModel "../StockDiffusionModel.cpp" "../lib/PriceSimulation.h" "../lib/PriceSimulation.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET StockDiffusionModel PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(StockDiffusionModel sfml-window sfml-graphics)