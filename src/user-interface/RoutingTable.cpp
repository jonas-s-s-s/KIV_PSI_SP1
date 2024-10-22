#include "RoutingTable.h"
#include "HttpServer.h"
#include <filesystem>
#include <fstream>

void RoutingTable::startLoop(const std::string &address, uint16_t port) {
    _server.setClientWriteHandler([](const std::string &request) {
        return _readProcRoute();
    });

    _server.startLoop(address, port);
}

std::string RoutingTable::_readProcRoute() {
    if (std::filesystem::exists(NET_ROUTE_PATH)) {
        std::ifstream t(NET_ROUTE_PATH);
        std::stringstream buffer;
        buffer << t.rdbuf();
        return buffer.str();
    } else {
        throw std::runtime_error("Error: cannot read file " + std::string(NET_ROUTE_PATH));
    }
}
