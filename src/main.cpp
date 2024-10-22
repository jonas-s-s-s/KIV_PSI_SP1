#include "RoutingTable.h"

int main(int argc, char **argv) {
    auto rt = RoutingTable{};
    rt.startLoop("127.0.0.1", 3000);
}