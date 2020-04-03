cmake_minimum_required(VERSION 3.15)
project(JustLayers)

set(CMAKE_CXX_STANDARD 17)

add_library(JustLayers Taskmaster.cpp include/JustLayers/Taskmaster.h ECS.cpp include/JustLayers/ECS.h include/JustLayers/System.h include/JustLayers/ResourceManager.h)
target_include_directories(JustLayers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
