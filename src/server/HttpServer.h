#pragma once

#include "Epoll.h"
#include <string>
#include <netinet/in.h>

class HttpServer {
public:

    /**
     * @param address IP address to bind to the server.
     * @param port Port number to bind to the server.
     */
    void startLoop(const std::string &address, uint16_t port);

    /**
     * Client write callback
     * @param clientWriteHandler Called with a string argument (written by client), returns a response string (sent back to client)
     */
    void setClientWriteHandler(const std::function<std::string(std::string)> &clientWriteHandler);

    /**
     * Client disconnect callback
     * @param clientDisconnectedHandler Called with the file descriptor of the disconnected client
     */
    void setClientDisconnectedHandler(const std::function<void(int)> &clientDisconnectedHandler);

    /**
     * Client connected callback
     * @param clientConnectedHandler Called with the file descriptor of the connected client
     */
    void setClientConnectedHandler(const std::function<void(int)> &clientConnectedHandler);

    ~HttpServer();

private:
    static constexpr std::string_view NOT_FOUND_404 = "HTTP/1.1 404 Not Found\r\n"
                                                      "Content-Type: text/html\r\n"
                                                      "Content-Length: 80\r\n"
                                                      "\r\n"
                                                      "<html>\r\n"
                                                      "<head><title>404 Not Found</title></head>\r\n"
                                                      "<body>\r\n"
                                                      "<h1>404 Not Found</h1>\r\n"
                                                      "</body>\r\n"
                                                      "</html>\r\n";
    static constexpr unsigned int MAX_BUF_LENGTH = 4096;
    static constexpr unsigned int TCP_ACCEPT_BACKLOG = 5;

    int _serverSocketFd = -1;
    Epoll _epoll{true};

    std::function<std::string(std::string)> _clientWriteHandler = nullptr;
    std::function<void(int)> _clientDisconnectedHandler = nullptr;
    std::function<void(int)> _clientConnectedHandler = nullptr;

    /**
     * Called when data is written to the socket (client sent data)
     *
     * @param clientFd File descriptor for the client socket.
     */
    void _onClientWrite(int clientFd);

    /**
     * Called when client terminates the TCP connection.
     *
     * @param clientFd File descriptor for the client socket.
     */
    void _onClientDisconnect(int clientFd);

    /**
     * Accepts new TCP connections to the server.
     *
     * @param serverFd File descriptor for the server socket.
     */
    void _tcpAccept(int serverFd);

    /**
     * Initializes the server socket and starts listening for connections
     * on the provided port and IP address.
     *
     * @param address IP address to bind to the server.
     * @param port Port number to bind to the server.
     */
    void _startServer(const std::string &address, uint16_t port);

};