#include <cstdlib>
#include <ctime>
#include <iostream>

#include "server.h"

int main() {
    // Init a new seed
    std::srand(std::time(0));

    start_server();
    return 0;
}