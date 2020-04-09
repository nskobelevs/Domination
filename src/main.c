#include <locale.h>
#include "gameLogic.h"
#include "init.h"

int main() {
    setlocale(LC_ALL, "");
    Game *game = initialiseGame();

    runGame(game);

    freeBoard(game);
    return 0;
}
