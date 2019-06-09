#include "engine.hpp"

Engine *engine;

int main(int argc, char *argv[]) {
    engine = new Engine();

    if (engine->init(argc, argv)) {
        while (engine->main());
        engine->shutdown();
    }

    delete engine;

    return 0;
}
