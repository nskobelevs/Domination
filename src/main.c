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

    movePieces(game.cells[1][2], game.cells[1][1], 1);
    movePieces(game.cells[1][3], game.cells[1][2], 2);
    movePieces(game.cells[1][4], game.cells[1][3], 3);
    movePieces(game.cells[1][5], game.cells[1][4], 4);
//    movePieces(game.cells[1][6], game.cells[1][5], 1);

    runGame(&game);



    delwin(game.boardWindow);
    endwin();
    freeBoard(game);
    return 0;
}
