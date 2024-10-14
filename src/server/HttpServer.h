#pragma once

#include "Epoll.h"
#include <string>
#include <netinet/in.h>

class HttpServer {
    private:

    static constexpr unsigned int MAX_BUF_LENGTH = 4096;
    static constexpr unsigned int TCP_ACCEPT_BACKLOG = 5;

    int serverSocketFd = -1;
    Epoll epoll{true};

    /**
     * Called when data is written to the socket (client sent data)
     *
     * @param clientFd File descriptor for the client socket.
     */
    void onClientWrite(int clientFd);

    /**
     * Called when client terminates the TCP connection.
     *
     * @param clientFd File descriptor for the client socket.
     */
    void onClientDisconnect(int clientFd);

    /**
     * Accepts new TCP connections to the server.
     *
     * @param serverFd File descriptor for the server socket.
     */
    void tcpAccept(int serverFd);

    /**
     * Initializes the server socket and starts listening for connections
     * on the provided port and IP address.
     *
     * @param address IP address to bind to the server.
     * @param port Port number to bind to the server.
     */
    void startServer(const std::string& address, uint16_t port);


    public:

    /**
     * @param address IP address to bind to the server.
     * @param port Port number to bind to the server.
     */
    void startLoop(const std::string& address, uint16_t port);

    ~HttpServer();
};