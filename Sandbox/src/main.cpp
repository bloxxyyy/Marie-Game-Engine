#include "Engine.h"
#include <iostream>

int main() {
    try {
        Engine engine(800, 600, "MarieEngine");
        engine.Run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
