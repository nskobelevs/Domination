#include "gameLogic.h"
#include "init.h"

int main() {
    Game *game = initialiseGame();

    runGame(game);
    freeBoard(game);
    return 0;
}
