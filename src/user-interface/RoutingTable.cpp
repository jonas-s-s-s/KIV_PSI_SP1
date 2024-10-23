#include "RoutingTable.h"
#include "HttpServer.h"
#include <filesystem>
#include <fstream>
#include "HtmlUtils.h"

void RoutingTable::startLoop(const std::string &address, uint16_t port) {
    _server.setClientWriteHandler([](const std::string &request) {
            return _parseRoutingTable(_readProcRoute());
    });

    _server.startLoop(address, port);
}

std::vector<std::string> RoutingTable::_readProcRoute() {
    if (std::filesystem::exists(NET_ROUTE_PATH)) {
        std::ifstream file(NET_ROUTE_PATH);
        std::string str;
        std::vector<std::string> output = {};
        while (std::getline(file, str))
        {
            output.emplace_back(str);
        }
        return output;
    } else {
        throw std::runtime_error("Error: cannot read file " + std::string(NET_ROUTE_PATH));
    }
}

std::string RoutingTable::_parseRoutingTable(const std::vector<std::string>& rawTable)
{
    std::string output{};
    for (const auto &tableLine : rawTable)
    {
        output += TR(TD(tableLine));
    }
    return TABLE(output);
}
