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
    initscr();
    Board board = initialiseBoard();
    board.window = newwin(35, 69, 0, 0);
    drawBoard(&board);
    curs_set(0);

    wgetch(board.window);


    endwin();
    delwin(board.window);
    freeBoard(board);
    return 0;
}
