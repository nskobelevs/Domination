#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <locale.h>
#include "components.h"
#include "init.h"
#include "gui.h"

int main() {
    setlocale(LC_ALL, "");
    Board board = initialiseBoard();


    freeBoard(board);
    return 0;
}
