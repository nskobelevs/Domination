#include <locale.h>
#include <stdbool.h>
#include "gameLogic.h"
#include "init.h"
#include "gameLogic.h"

int main() {
    setlocale(LC_ALL, "");
    Game *game = initialiseGame();

    for (int j = 1; j < 7; j++)
        for (int i = 1; i < 6; i++)
            movePieces(game->cells[j][i], game->cells[j][i+1], i);

    runGame(game);
    freeBoard(game);
    return 0;
}
