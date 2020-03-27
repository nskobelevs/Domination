//
// Created by nskobelevs on 25/03/2020.
//

#ifndef DOMINATION_GUI_H
#define DOMINATION_GUI_H

#include "components.h"

void drawBoard(Game *game, int selectedCellX, int selectedCellY);
void printTitle(void);

void namePrompt(Player *player, unsigned int playerIndex);
void colourPrompt(Player *player, Colour *player1Colour);

void drawCell(WINDOW *cellWindow, Cell *cell, int selectedCount);

Cell *getUserSelectedCell(Game *game, WINDOW *cellWindow);

#endif //DOMINATION_GUI_H
