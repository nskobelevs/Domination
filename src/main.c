#include <locale.h>
#include "log.h"
#include "gameLogic.h"
#include "init.h"

int main() {

#ifdef DEBUG
    initLog();
    LOG("Starting Domination...\n");
#endif

    setlocale(LC_ALL, "");
    Game *game = initialiseGame();

    runGame(game);
    freeBoard(game);

#ifdef DEBUG
    LOG("Returning from main...\n");
    endLog();
#endif

    return 0;
}
