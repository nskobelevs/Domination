//
// Created by nskobelevs on 26/03/2020.
//

#ifndef DOMINATION_GAMELOGIC_H
#define DOMINATION_GAMELOGIC_H

#include "components.h"

void runGame(Game *game);

unsigned int getDistance(Cell *cell1, Cell *cell2);

void movePieces(Cell *source, Cell *destination, unsigned int count);

#endif //DOMINATION_GAMELOGIC_H
