#ifndef DOMINATION_GAMELOGIC_H
#define DOMINATION_GAMELOGIC_H

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "gui.h"
#include "components.h"

void runGame(Game *game);

unsigned int getDistance(Cell *cell1, Cell *cell2);

void movePieces(Cell *source, Cell *destination, unsigned int count);

#endif //DOMINATION_GAMELOGIC_H
