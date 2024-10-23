#include "RoutingTable.h"

int main(int argc, char **argv) {
    auto rt = RoutingTable{};
    rt.startLoop("0.0.0.0", 3000);
}