#include "engine.hpp"

Engine *engine;

int main(int argc, char *argv[])
{
    Engine engine;

    if (engine.init(argc, argv)) {
        while (engine.main()) {
        }

        engine.halt();
    }

    return 0;
}
