cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "lib/*.cpp")
file(GLOB_RECURSE HEADERS "lib/*.h")

add_library(WinBGIm STATIC ${SOURCES} ${HEADERS})

target_include_directories(WinBGIm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(WIN32)
	target_link_libraries(WinBGIm PUBLIC
		user32 
		gdi32 
		ole32 
		comdlg32 
		oleaut32
		)
endif()