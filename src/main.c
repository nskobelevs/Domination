#include <locale.h>
#include <stdbool.h>
#include <unistd.h>
#include "components.h"
#include "init.h"
#include "gameLogic.h"
#include "debugTools.h"
#include "gui.h"

int main() {
    setlocale(LC_ALL, "");
    Game game = initialiseGame();

    runGame(&game);



    delwin(game.boardWindow);
    endwin();
    freeBoard(game);
    return 0;
}
