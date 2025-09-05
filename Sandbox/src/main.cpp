#include "Engine.h"

int main() {
    try {
        Engine engine;
        engine.Run();
    }
    catch (const std::exception& e) {
        printf("Error: %s\n", e.what());
        return -1;
    }

    return 0;
}
