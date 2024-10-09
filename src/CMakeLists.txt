add_subdirectory(epoll-cpp)

add_executable(http_server main.cpp)
target_link_libraries(http_server epoll_lib)