cmake_minimum_required(VERSION 3.27)
project(TerminalFileExplorer)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)

add_library(Directory lib/Directory.h src/Directory.cpp)
target_include_directories(Directory PUBLIC lib)

add_library(File lib/File.h src/File.cpp)
target_include_directories(File PUBLIC lib)

add_library(RasterLine lib/RasterLine.h src/RasterLine.cpp)
target_link_libraries(RasterLine PUBLIC Point)
target_include_directories(RasterLine PUBLIC lib)

add_library(Point lib/Point.h src/Point.cpp)
target_include_directories(Point PUBLIC lib)

add_library(Window lib/Window.h src/Window.cpp)
target_include_directories(Window PUBLIC lib)
target_link_libraries(Window PUBLIC Point)


add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC lib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CURSES_LIBRARIES} Directory File RasterLine Point Window)