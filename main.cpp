// main.cpp
#include "Engine.h"

int main(int argc, char** argv) {
    Engine engine(argc, argv);
    engine.SetClearColor(0.1f, 0.1f, 0.15f);
    engine.Init();
    engine.Run();
    engine.Cleanup();
    return 0;
}
