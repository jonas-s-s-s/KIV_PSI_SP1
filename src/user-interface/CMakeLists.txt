add_library(user-interface HtmlUtils.cpp RoutingTable.cpp)
target_link_libraries(user-interface server)

target_include_directories(user-interface PUBLIC .)
