cmake_minimum_required(VERSION 3.16)

project(SpearmanUCP VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_subdirectory(Dependencies/WinBGIm)

add_library(miniaudio INTERFACE)
target_include_directories(miniaudio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/miniaudio)

file(GLOB_RECURSE SOURCE_FILES 
    "src/*.cpp"
    "src/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE WinBGIm miniaudio)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/Resources
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
)