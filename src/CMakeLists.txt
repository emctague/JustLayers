add_library(JustLayers Taskmaster.cpp include/JustLayers/Taskmaster.h ECS.cpp include/JustLayers/ECS.h include/JustLayers/System.h include/JustLayers/ResourceManager.h include/JustLayers/Registry.h)
target_include_directories(JustLayers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
