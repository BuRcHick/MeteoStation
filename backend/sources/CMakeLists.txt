add_library(sources logger.cpp)
add_subdirectory(database)
add_subdirectory(mqtt)

target_link_libraries(sources database mqtt)