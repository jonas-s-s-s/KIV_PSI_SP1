add_subdirectory(epoll-cpp)
add_subdirectory(server)
#add_subdirectory(user-interface)

add_executable(http_server main.cpp)
target_link_libraries(http_server server)