//
// Created by nskobelevs on 26/03/2020.
//

#ifndef DOMINATION_GAMELOGIC_H
#define DOMINATION_GAMELOGIC_H

#include "components.h"

void movePieces(Cell *destination, Cell *source, unsigned int count);
void shortenCell(Cell *cell);
void runGame(Game *game);

#endif //DOMINATION_GAMELOGIC_H
