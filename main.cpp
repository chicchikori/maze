#include <string>
#include "src/Game.h"

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        std::string filePaht = argv[1]; // NOLINT
    }
    Game game;
    game.Run();
    return 0;
}
