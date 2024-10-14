add_library(server HttpServer.cpp HttpServer.h)
target_include_directories(server PUBLIC .)
target_link_libraries(server epoll_lib)