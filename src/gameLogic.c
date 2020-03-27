//
// Created by nskobelevs on 26/03/2020.
//

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "gameLogic.h"


//Appends a number of pieces from the top of source Cell to the top destination Cell
void movePieces(Cell *destination, Cell *source, unsigned int count) {

    //Basic assumptions
    //TODO: Remove for final version
    assert(count <= source->length);
    assert(count != 0);
    assert(source->length != 0);
    assert(destination != source);

    if (count == source->length) {
        source->tail->next = destination->head;
        destination->head = source->head;
        destination->length += count;

        source->head = NULL;
        source->tail = NULL;
        source->length = 0;

        if (destination->length > 5)
            shortenCell(destination);

        return;
    }

    Piece *newSourceHead = source->head;    //This will become the new head of source;
    Piece *piece = source->head;  //This is the piece above the newSourceHead;
    for (unsigned int index = 0; index < count; index++) {
        piece = newSourceHead;
        newSourceHead = newSourceHead->next;
    }

    /* BEFORE
     * Source -> p1 -> p2 -> p3 -> p4
     * Destination -> p5 -> p6 -> p7
     *
     * e.g. count == 2
     *
     *         --------------|
     * AFTER  /              \/
     * Source    p1 -> p2    p3 -> p4
     *           /\     |
     *           |     \/
     * Destination     p5 -> p6 -> p7
     *
     *
     * Source -> p3 -> p4
     * Destination -> p1 -> p2 -> p5 -> p6 -> p7
     */

    piece->next = destination->head;
    destination->head = source->head;
    source->head = newSourceHead;

    source->length -= count;
    destination->length += count;

    if (destination->length > 5)
        shortenCell(destination);
}

//Removes excess pieces when the Cell is larger than 5
void shortenCell(Cell *cell) {
    //Assumptions
    //TODO: Remove for final version
    assert(cell->length > 5);

    Player *player = cell->head->owner;

    //newTail is the 5th piece;
    Piece *newTail = cell->head->next->next->next->next;

    //Increases the amount of pieces a player has reserved
    //Free's pieces that aren't currently in game
    Piece *piece = newTail->next;
    Piece *temp = piece;
    while (piece != NULL) {
        if (piece->owner == player) {
            player->reservedCounter++;
        }
        temp = piece;
        piece = piece->next;

        free(temp);
    }

    //Setting the new tail and stopping tail from pointing to free'd memory
    cell->tail = newTail;
    cell->tail->next = NULL;
}

void runGame(Game *game) {

}