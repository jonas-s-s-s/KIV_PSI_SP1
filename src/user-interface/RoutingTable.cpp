#include "RoutingTable.h"
#include "HttpServer.h"
#include <filesystem>
#include <fstream>
#include <regex>
#include "HtmlUtils.h"

void RoutingTable::startLoop(const std::string &address, uint16_t port) {
    _server.setClientWriteHandler(_onClientWrite);
    _server.startLoop(address, port);
}

std::string RoutingTable::_onClientWrite(const std::string &request) {
    const std::string &routingTable = _parseRoutingTable(_readProcRoute());
    return DEFAULT_HTML_TEMPLATE(H1("Routing Table") + routingTable + HR() + P("Routing table of a virtual server."));
}

std::vector<std::string> RoutingTable::_readProcRoute() {
    if (std::filesystem::exists(NET_ROUTE_PATH)) {
        std::ifstream file(NET_ROUTE_PATH);
        std::string str;
        std::vector<std::string> output = {};
        while (std::getline(file, str)) {
            output.emplace_back(str);
        }
        return output;
    } else {
        throw std::runtime_error("Error: cannot read file " + std::string(NET_ROUTE_PATH));
    }
}

//##########################################
//# PARSING
//##########################################

std::string RoutingTable::_parseRoutingTable(const std::vector<std::string> &rawTable) {
    std::vector<std::string> tableRows{};
    std::string output{};

    // Insert table header
    output += TR(TH("Interface") +
                 TH("Destination") +
                 TH("Mask") +
                 TH("Metric") +
                 TH("Gateway") +
                 TH("Flags"));

    bool isFirstLine = true;
    for (const auto &tableLine: rawTable) {
        // Skip first line (table header)
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        // Parse the current line
        RoutingTableRecord r = _parseRoutingTableLine(tableLine);

        // Skip "unusable" records
        if (r.Gateway == "0.0.0.0")
            continue;

        // Convert the flags string list to HTML
        std::string flagsFormatted{};
        for (auto const &flagStr: r.Flags) {
            flagsFormatted += flagStr + BR();
        }

        // Create the table row
        output += TR(
                TD(r.Iface) +
                TD(r.Destination) +
                TD(r.Mask) +
                TD(std::to_string(r.Metric)) +
                TD(r.Gateway) +
                TD(flagsFormatted)
        );
    }
    return TABLE(output);
}

RoutingTableRecord RoutingTable::_parseRoutingTableLine(const std::string &line) {
    std::regex re("\\S+");
    std::vector<std::string> tokens;
    std::sregex_token_iterator it(line.begin(), line.end(), re);
    std::sregex_token_iterator end;
    while (it != end) {
        tokens.push_back(*it++);
    }

    const RoutingTableRecord r{tokens[0],
                               _parseIPv4HexString(tokens[1]),
                               _parseIPv4HexString(tokens[2]),
                               _parseFlags(tokens[3]),
                               std::stoi(tokens[4]),
                               std::stoi(tokens[5]),
                               std::stoi(tokens[6]),
                               _parseIPv4HexString(tokens[7]),
                               std::stoi(tokens[8]),
                               std::stoi(tokens[9]),
                               std::stoi(tokens[10])};
    return r;
}

std::string RoutingTable::_parseIPv4HexString(const std::string &IPv4) {
    const uint32_t address = std::stoul(IPv4, nullptr, 16);
    std::ostringstream ipOutput;
    ipOutput << (address & 0xFF) << "."
             << ((address >> 8) & 0xFF) << "."
             << ((address >> 16) & 0xFF) << "."
             << ((address >> 24) & 0xFF);
    return ipOutput.str();
}

std::vector<std::string> RoutingTable::_parseFlags(const std::string &flagsStr) {
    auto flags = static_cast<uint16_t>(std::stoul(flagsStr));
    std::vector<std::string> flagsParsed{};

    if (flags & Flags::RTF_UP)
        flagsParsed.emplace_back("Route usable");
    if (flags & Flags::RTF_GATEWAY)
        flagsParsed.emplace_back("Destination is a gateway");
    if (flags & Flags::RTF_HOST)
        flagsParsed.emplace_back("Host entry");
    if (flags & Flags::RTF_REINSTATE)
        flagsParsed.emplace_back("Reinstate route after timeout");
    if (flags & Flags::RTF_DYNAMIC)
        flagsParsed.emplace_back("Created dynamically");
    if (flags & Flags::RTF_MODIFIED)
        flagsParsed.emplace_back("Modified dynamically");
    if (flags & Flags::RTF_MTU)
        flagsParsed.emplace_back("Specific MTU for this route");
    if (flags & Flags::RTF_WINDOW)
        flagsParsed.emplace_back("Per route window clamping");
    if (flags & Flags::RTF_IRTT)
        flagsParsed.emplace_back("Initial round trip time");
    if (flags & Flags::RTF_REJECT)
        flagsParsed.emplace_back("Reject route");
    if (flags == 0)
        flagsParsed.emplace_back("No flags");

    return flagsParsed;
}


