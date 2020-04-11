//
// Created by Nikita Skobelevs on 09/04/2020.
//

//This header file is shared by gui_posix and gui_windows

#ifndef DOMINATION_GUI_H
#define DOMINATION_GUI_H

#include <stdbool.h>
#include "components.h"

void askPlayerForName(Player *player, Player *otherPlayer);

void askPlayerForColour(Player *player, Player *otherPlayer);

void printBoard(Game *game, Cell *selectedCell);

Cell *selectCell(Game *game, Cell *sourceCell, bool *placeReservedPiece, unsigned int maxDist);

unsigned int askCount(Game *game, Cell *source);

void printWinner(Game *game, Player *player);



#endif //DOMINATION_GUI_H
