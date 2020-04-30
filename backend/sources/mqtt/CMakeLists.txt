add_library(mqtt mqtt_client.cpp)
target_link_libraries(mqtt mosquittopp)