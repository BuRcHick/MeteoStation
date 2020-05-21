add_library(database database_manager.cpp database.cpp)
target_link_libraries(database sqlite3)