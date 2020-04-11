//
// Created by nskobelevs on 26/03/2020.
//

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "gameLogic.h"
#include "gui.h"


static bool playerCanMakeMove(Game *game, Player *player);
static void shortenCell(Cell *cell);

static void placePiece(Cell *cell, Player *player) {
    Piece *piece = (Piece *)malloc(sizeof(*piece));

    player->reservedCounter--;

    piece->owner = player;
    piece->next = cell->head;
    cell->head = piece;
    cell->length += 1;

    if (cell->length > 4)
        shortenCell(cell);
}

/**
 * \relates Cell
 * Moves <b>count</b> number of pieces from <b>source</b> to <b>destination</b>
 * @param destination Where to move the pieces
 * @param source Where to move the pieces from
 * @param count How many pieces to move<br>
 */
void movePieces(Cell *source, Cell *destination, unsigned int count) {

    //Basic assumptions
    assert(count <= source->length);
    assert(count != 0);
    assert(source->length != 0);
    assert(destination != source);

    if (count == source->length) {
        source->tail->next = destination->head;
        destination->head = source->head;
        destination->length += count;

        if (destination->tail == NULL)
            destination->tail = source->tail;

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

    if (destination->tail == NULL)
        destination->tail = piece;

    source->length -= count;
    destination->length += count;

    if (destination->length > 5)
        shortenCell(destination);
}

/**
 * \relates Cell
 * Performs extra logic when a stack is greater than 5 pieces.<br>
 * Cell must be > 5 when function is called<br>
 * All extra pieces are free'd<br>
 * If the removed pieces are the player's, the player's reservedCounter is increased appropriately
 * @param cell The cell which to shorten
 * @attention
 * an assertion is made that <b>cell->length > 5</b>
 */
static void shortenCell(Cell *cell) {

    assert(cell->length > 5);

    //The player who own's the stack
    Player *player = cell->head->owner;
    //newTail is the 5th piece;
    Piece *newTail = cell->head->next->next->next->next;

    Piece *piece = newTail->next;
    Piece *temp = piece;

    //Looping until the bottom of the stack
    while (piece != NULL) {
        if (piece->owner == player) {
            //Increases the amount of pieces a player has reserved
            player->reservedCounter++;
        }
        temp = piece;
        piece = piece->next;

        //Free's pieces that aren't currently in game
        free(temp);
    }

    //Setting the new tail and stopping tail from pointing to free'd memory
    cell->tail = newTail;
    cell->tail->next = NULL;
    cell->length = 5;
}

/**
 * \relates Cell
 * Gets the taxicab distance between two cells
 * @param cell1 Cell1
 * @param cell2 Cell2
 * @return |cell1.rowIndex - cell2.rowIndex| + |cell1.columnIndex - cell2.columnIndex|
 */
unsigned  getDistance(Cell *cell1, Cell *cell2) {
    return abs(cell1->rowIndex - cell2->rowIndex) + abs(cell1->columnIndex - cell2->columnIndex);
}

/**
 * \relates Game
 * Runs the main game loop
 * @param game A game instance
 */
void runGame(Game *game) {

    Cell *source, *destination;
    bool placeReservedPiece;
    Player *currentPlayer, *otherPlayer;
    unsigned int count;

    while (true) {
        currentPlayer = game->players[game->moveIndex % 2];
        otherPlayer = game->players[(game->moveIndex + 1) % 2];
        source = selectCell(game, NULL, &placeReservedPiece, 0);

        if (placeReservedPiece) {
            placePiece(source, currentPlayer);
        } else {
            count = askCount(game, source);
            destination = selectCell(game, source, NULL, count);
            movePieces(source, destination, count);
        }

        if (otherPlayer->reservedCounter == 0 && !playerCanMakeMove(game, otherPlayer)) {
            printWinner(game, currentPlayer);
            break;
        }

        game->moveIndex++;
    }
}

/**
 * \relates Game
 * Returns true/false whether a player can move <b>any</b> piece on the board
 * @param game Game instance
 * @param player The player being checked
 * @return bool signifying whether player can move
 */
static bool playerCanMakeMove(Game *game, Player *player) {
    Cell *cell;

    //Looping through cell grid
    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {
            cell = game->cells[rowIndex][columnIndex];

            //Skipping invalid and empty cells
            if (cell == NULL || cell->head == NULL)
                continue;

            //If stack is owned by player, they can make a valid move
            if (cell->head->owner == player)
                return true;
        }
    }

    //If the whole board has been looped, no move for palyer exists
    return false;
}