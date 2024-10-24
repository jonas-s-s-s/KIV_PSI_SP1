#pragma once

#include <string>
#include "HttpServer.h"

/**
 * Contains the flag values of the Linux routing table.
 */
namespace Flags {
    constexpr uint16_t RTF_UP        = 0x0001;  // route usable
    constexpr uint16_t RTF_GATEWAY   = 0x0002;  // destination is a gateway
    constexpr uint16_t RTF_HOST      = 0x0004;  // host entry (net otherwise)
    constexpr uint16_t RTF_REINSTATE = 0x0008;  // reinstate route after timeout
    constexpr uint16_t RTF_DYNAMIC   = 0x0010;  // created dynamically (by redirect)
    constexpr uint16_t RTF_MODIFIED  = 0x0020;  // modified dynamically (by redirect)
    constexpr uint16_t RTF_MTU       = 0x0040;  // specific MTU for this route
    constexpr uint16_t RTF_WINDOW    = 0x0080;  // per route window clamping
    constexpr uint16_t RTF_IRTT      = 0x0100;  // initial round trip time
    constexpr uint16_t RTF_REJECT    = 0x0200;  // reject route
}

/**
 * Contains parsed information about the particular routing table record
 */
class RoutingTableRecord {
public:
    const std::string Iface;
    const std::string Destination;
    const std::string Gateway;
    const std::vector<std::string> Flags;
    const int RefCnt;
    const int Use;
    const int Metric;
    const std::string Mask;
    const int MTU;
    const int Window;
    const int IRTT;

    RoutingTableRecord(const std::string &iface, const std::string &destination, const std::string &gateway,
                       const std::vector<std::string> &flags, const int refCnt, const int use, const int metric,
                       const std::string &mask, const int mtu, const int window, const int irtt) : Iface(iface),
                                                                                                     Window(window),
                                                                                                     MTU(mtu),
                                                                                                     Mask(mask),
                                                                                                     Use(use),
                                                                                                     Metric(metric),
                                                                                                     RefCnt(refCnt),
                                                                                                     Flags(flags),
                                                                                                     Gateway(gateway),
                                                                                                     Destination(
                                                                                                             destination),
                                                                                                     IRTT(irtt) {}
};

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

    static std::string _onClientWrite(const std::string &request);

    static std::vector<std::string> _readProcRoute();

    static std::string _parseRoutingTable(const std::vector<std::string> &table);

    static RoutingTableRecord _parseRoutingTableLine(const std::string &line);

    static std::string _parseIPv4HexString(const std::string &IPv4);

    static std::vector<std::string> _parseFlags(const std::string &flagsStr);
};