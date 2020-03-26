#include <locale.h>
#include <stdio.h>
#include "components.h"
#include "init.h"
#include "gameLogic.h"
#include "debugTools.h"

int main() {
    setlocale(LC_ALL, "");
    Board board = initialiseBoard();

    freeBoard(board);
    return 0;
}
