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

Cell *selectCell(Game *game, bool selectingSource, bool *placeReservedPiece);

#endif //DOMINATION_GUI_H
