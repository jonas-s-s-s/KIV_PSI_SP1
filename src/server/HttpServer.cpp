#include "Epoll.h"
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <iostream>
#include <csignal>
#include "HttpServer.h"
#include <functional>

constexpr unsigned int MAX_BUF_LENGTH = 4096;
constexpr unsigned int TCP_ACCEPT_BACKLOG = 5;

//##########################################
//# CONSTRUCTORS + DESTRUCTORS
//##########################################

HttpServer::~HttpServer() {
    close(_serverSocketFd);
}

//##########################################
//# PUBLIC MEMBERS
//##########################################

void HttpServer::startLoop(const std::string &address, uint16_t port) {
    _startServer(address, port);

    for (;;) {
        _epoll.waitForEvents();
    }

    close(_serverSocketFd);
}

void HttpServer::setClientWriteHandler(const std::function<std::string(std::string)> &clientWriteHandler) {
    _clientWriteHandler = clientWriteHandler;
}

void HttpServer::setClientDisconnectedHandler(const std::function<void(int)> &clientDisconnectedHandler) {
    _clientDisconnectedHandler = clientDisconnectedHandler;
}

void HttpServer::setClientConnectedHandler(const std::function<void(int)> &clientConnectedHandler) {
    _clientConnectedHandler = clientConnectedHandler;
}


//##########################################
//# PRIVATE MEMBERS
//##########################################

/**
 * Called when data is written to the socket (client sent data)
 */
void HttpServer::_onClientWrite(int clientFd) {
    int bytesReceived = 0;
    std::string rcv;
    std::vector<char> buffer(MAX_BUF_LENGTH);

    bytesReceived = recv(clientFd, &buffer[0], buffer.size(), 0);
    if (bytesReceived == -1) {
        throw std::runtime_error("Failed to receive data on this socket. (FD" + std::to_string(clientFd) + ")");
    }
    rcv.append(buffer.cbegin(), buffer.cbegin() + bytesReceived);
    std::cout << "[HttpServer] " << "Received " << bytesReceived << " bytes of data from FD" << clientFd << std::endl;

    if (_clientWriteHandler == nullptr) {
        // There is no write handler
        send(clientFd, std::string(NOT_FOUND_404).c_str(), NOT_FOUND_404.size(), 0);
    } else {
        // Get response via the write handler
        const std::string &responseString = _clientWriteHandler(rcv);
        // Add an HTTP header
        const std::string &httpResponse =
                "HTTP/1.1 200 OK\r\nContent-Length: " + std::to_string(responseString.size()) +
                "\r\nContent-Type: text/html\r\n\r\n" + responseString;
        // Send response to client
        send(clientFd, httpResponse.c_str(), httpResponse.size(), 0);
    }

}

/**
 * Called when client terminates the TCP connection
 */
void HttpServer::_onClientDisconnect(int clientFd) {
    std::cout << "[HttpServer] " << "TCP client FD" << clientFd << " has disconnected." << std::endl;

    // Invoke user defined callback
    if (_clientDisconnectedHandler != nullptr)
        _clientDisconnectedHandler(clientFd);
}

/**
 * Accepts new TCP connections to the server
 */
void HttpServer::_tcpAccept(int serverFd) {
    int clientFd; // Client's socket - client sends requests via this socket
    struct sockaddr_in remoteAddr{}; // Client's address
    socklen_t remoteAddrLen{}; // Length of client's address

    clientFd = accept(serverFd, (struct sockaddr *) &remoteAddr, &remoteAddrLen);

    if (clientFd > 0) {
        std::cout << "[HttpServer] " << "A new TCP client FD" << clientFd << " connected to server FD" << serverFd
                  << std::endl;

        // Invoke user defined callback
        if (_clientConnectedHandler != nullptr)
            _clientConnectedHandler(clientFd);

        // Add this client socket to the Epoll
        _epoll.addDescriptor(clientFd);

        // The Epoll instance will call our handler functions once user writes something to the socket or disconnects
        _epoll.addEventHandler(clientFd, EPOLLIN, [this](int fd) { _onClientWrite(fd); });
        _epoll.addEventHandler(clientFd, EPOLLRDHUP | EPOLLHUP, [this](int fd) { _onClientDisconnect(fd); });
    } else {
        throw std::runtime_error("Fatal error in _tcpAccept of server socket FD" + std::to_string(serverFd)
                                 + " TCP accept failed. " + std::to_string(clientFd));
    }
}

/**
 * Initializes the server socket and starts listening for connections on provided port + ip
 */
void HttpServer::_startServer(const std::string &address, uint16_t port) {
    struct sockaddr_in localAddr{};
    localAddr.sin_family = AF_INET; // IPv4
    localAddr.sin_port = htons(port); // Server listens on this port
    localAddr.sin_addr.s_addr = inet_addr(address.c_str()); // Server listens on this address

    // Create the socket
    if ((_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        throw std::runtime_error("Failed to create a server socket (system resource error)");
    }

    // Bind socket to port and ip
    if (bind(_serverSocketFd, (struct sockaddr *) &localAddr, sizeof(struct sockaddr_in)) != 0) {
        throw std::runtime_error(
                "Failed bind server socket. (FD" + std::to_string(_serverSocketFd) + ") (Port: " + std::to_string(port)
                + ")");
    }

    // Listen on bound port and ip
    if (listen(_serverSocketFd, TCP_ACCEPT_BACKLOG) != 0) {
        throw std::runtime_error("Failed listen on server socket. (FD" + std::to_string(_serverSocketFd) + ")");
    }

    std::cout << "[HttpServer] " << "A new server socket FD" << _serverSocketFd << " is now listening on port " << port
              << std::endl;

    // Register this server socket with the _epoll
    _epoll.addDescriptor(_serverSocketFd);
    _epoll.addEventHandler(_serverSocketFd, EPOLLIN | EPOLLOUT, [this](int serverFd) { _tcpAccept(serverFd); });
}
