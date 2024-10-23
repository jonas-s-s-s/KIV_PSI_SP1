#pragma once
#include <string>
#include "HttpServer.h"

class RoutingTable {
public:
    /**
   * Main loop on which the Routing Table UI server runs
   *
   * @param address IP address to bind to the server.
   * @param port Port number to bind to the server.
   */
    void startLoop(const std::string &address, uint16_t port);

private:
    constexpr static const char *const NET_ROUTE_PATH = "/proc/net/route";

    HttpServer _server{};

    static std::vector<std::string> _readProcRoute();

    static std::string _parseRoutingTable(const std::vector<std::string>& table);
};