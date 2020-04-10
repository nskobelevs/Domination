#include <locale.h>
#include "gameLogic.h"
#include "init.h"

int main() {
    setlocale(LC_ALL, "");
    Game *game = initialiseGame();

    game->players[0]->reservedCounter = 10;

    runGame(game);
    freeBoard(game);
    return 0;
}
