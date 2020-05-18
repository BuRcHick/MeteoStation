add_library(sources logger.cpp)
add_subdirectory(database)
add_subdirectory(mqtt)
add_subdirectory(qp)

target_link_libraries(sources mqtt qp database)