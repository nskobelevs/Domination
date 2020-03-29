//
// Created by nskobelevs on 26/03/2020.
//

#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gameLogic.h"
#include "gui.h"

static int getDistance(Coord coord1, Coord coord2);


//Appends a number of pieces from the top of source Cell to the top destination Cell
void movePieces(Cell *destination, Cell *source, unsigned int count) {

    //Basic assumptions
    //TODO: Remove for final version
    assert(count <= source->length);
    assert(count != 0);
    assert(source->length != 0);
    assert(destination != source);

    FILE *outpitFile = fopen("domination.log", "a");

    fprintf(outpitFile, "Moving %d pieces.", count);
    fprintf(outpitFile, "Before:");
    fprintf(outpitFile, "\tSource: %p\n", source);
    fprintf(outpitFile, "\tSource Head: %p\n", source->head);
    fprintf(outpitFile, "\tSource Tail: %p\n", source->tail);
    fprintf(outpitFile, "\tSource Length: %d\n\n", source->length);

    fprintf(outpitFile, "\tDestination: %p\n", destination);
    fprintf(outpitFile, "\tDestination Head: %p\n", destination->head);
    fprintf(outpitFile, "\tDestination Tail: %p\n", destination->tail);
    fprintf(outpitFile, "\tDestination Length: %d\n\n", destination->length);

    fclose(outpitFile);


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
    cell->length = 5;
}

static int getDistance(Coord coord1, Coord coord2) {
    return abs(coord1.x - coord2.x) + abs(coord1.y - coord2.y);
}

//Runs the game loop
void runGame(Game *game) {
    WINDOW *window = game->boardWindow;
    keypad(window, true);

    int cellWindowHeight = 26;
    int cellWindowWidth = 17;

    int heightOffset = (LINES - cellWindowHeight) / 2;
    int widthOffset = ( (COLS/2 + 34) + COLS) / 2 - cellWindowWidth/2;

    WINDOW *cellWindow = newwin(cellWindowHeight, cellWindowWidth, heightOffset, widthOffset);
    keypad(cellWindow, true);
    box(cellWindow, 0, 0);
    wrefresh(cellWindow);

    int selectedCellX = 1;
    int selectedCellY = 1;
    int key = -1;

    Cell *cell;
    Piece *piece;
    Coord sourceCoord;
    Coord destinationCoord;

    char temp[100];

    Player *currentPlayer;
    Player *opponentPlayer;

    while (true) {

        drawCell(cellWindow, game->cells[selectedCellY][selectedCellX], 0);
        drawBoard(game, selectedCellX, selectedCellY);

        currentPlayer = game->players[game->moveIndex % 2];

        sprintf(temp, "%s's move...", currentPlayer->name);
        attron(COLOR_PAIR(currentPlayer->colour));
        printMessage(1, temp);
        attroff(COLOR_PAIR(currentPlayer->colour));

        Cell *source = NULL;
        Cell *destination;


        while (true) {
            sourceCoord = getUserSelectedCell(game, cellWindow);
            source = game->cells[sourceCoord.y][sourceCoord.x];
            if (source->length == 0)
                printMessage(1, "You can't move piece from an empty cell");
            else if (source->head->owner != currentPlayer)
                printMessage(1, "You can't move your opponents piece");
            else
                break;
        }

        Cell *selectedCell = source;
        int piecesCount = 1;
        key = -1;
        //Part 2. Selecting number of pieces to move
        drawCell(cellWindow, selectedCell, piecesCount);
        while (source->length != 1 && (key = wgetch(cellWindow)) != 10) {
            if (key == KEY_UP && piecesCount > 1)
                piecesCount -= 1;
            else if (key == KEY_DOWN && piecesCount < selectedCell->length)
                piecesCount += 1;

            drawCell(cellWindow, selectedCell, piecesCount);
        }
        //Part 3. Selecting where to places the pieces
        int taxicabDistance;
        while (true) {
            destinationCoord = getUserSelectedCell(game, cellWindow);
            destination = game->cells[destinationCoord.y][destinationCoord.x];
            assert(destination != NULL);
            taxicabDistance = getDistance(sourceCoord, destinationCoord);

            if (taxicabDistance > source->length) {
                sprintf(temp, "You can move at most %d pieces", source->length);
                printMessage(1, temp);
            } else if (taxicabDistance == 0)
                printMessage(1, "You can't move to the same space");
            else if (taxicabDistance <= source->length) {
                movePieces(destination, source, piecesCount);
                selectedCellX = destinationCoord.x;
                selectedCellY = destinationCoord.y;
                break;
            }
        }

        game->moveIndex++;

        if (game->moveIndex == 100)
            break;
    }

    delwin(cellWindow);
}

Player *getBoardWinner(Game *game) {
    Player *player = NULL;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            //Skip corners
            if (game->cells[y][x] == NULL)
                continue;

            //Skip empty
            if (game->cells[y][x]->length == 0)
                continue;

            //First non-corner and non-empty cell
            if (player == NULL)
                player = game->cells[y][x]->head->owner;

            if (player != game->cells[y][x]->head->owner)
                return NULL;
        }
    }

    return player;
}