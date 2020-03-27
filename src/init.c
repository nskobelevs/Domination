//
// Created by nskobelevs on 25/03/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "init.h"
#include "gui.h"

static Player* initialisePlayer(unsigned int playerIndex, Colour *player1Colour);

//Asks the user to input their name and pick a colour
static Player* initialisePlayer(unsigned int playerIndex, Colour *player1Colour) {
    Player *player = (Player *)malloc(sizeof(Player));

    namePrompt(player, playerIndex);
    colourPrompt(player, player1Colour);


    return player;
}

//Initialises a Game objects. Add the players, cells and correct starting
//arrangement of pieces.
Game initialiseGame(void) {
    stdscr = initscr();
    noecho();
    curs_set(0);
    start_color();

    printTitle();
    //COLOR_BLACK is the default terminal background
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(BLANK, COLOR_WHITE, COLOR_BLACK);

    Game game = {};

    Player *player1 = initialisePlayer(1, NULL);
    Player *player2 = initialisePlayer(2, &player1->colour);


    game.players[0] = player1;
    game.players[1] = player2;
    game.moveIndex = 0;

    int boardWindowOffset = (COLS - 67) / 2;

    game.boardWindow = newwin(35, 67, 7, boardWindowOffset);

    //Calculating distance from the centre to determine whether cell is used or not
    //i.e. to remove the corners
    double squaredDistance;

    Cell *cell;

    //Fill the board with empty stacks
    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {

            squaredDistance = pow(rowIndex - 3.5, 2) + pow(columnIndex - 3.5, 2);

            if (squaredDistance < 18) {
                cell = (Cell *)malloc(sizeof(Cell));
                cell->head = NULL;
                cell->tail = NULL;
                cell->length = 0;

                game.cells[rowIndex][columnIndex] = cell;
            } else {
                //Corner
                game.cells[rowIndex][columnIndex] = NULL;
            }
        }
    }

    //Add player's pieces to the inner 6x6 square;
    Piece *piece;
    int index;
    for (int rowIndex = 0; rowIndex < 6; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 6; columnIndex++) {
            piece = (Piece *)malloc(sizeof(Piece));
            piece->next = NULL;

            //Logic for figuring out the owner for every co-ordinate
            index = rowIndex * 6 + columnIndex;
            if (index % 4 == 0 || index % 4 == 1) {
                piece->owner = game.players[0];
            } else {
                piece->owner = game.players[1];
            }

            cell = game.cells[rowIndex + 1][columnIndex + 1];

            cell->head = piece;
            cell->tail = piece;
            cell->length = 1;
        }
    }

    return game;
}

//frees all allocated memory
void freeBoard(Game game) {

    Cell *stack;
    Piece *piece;
    Piece *temp;

    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {
            stack = game.cells[rowIndex][columnIndex];

            if (stack != NULL) {
                piece = stack->head;
                //Free all pieces in a stack
                while (piece != NULL) {
                    temp = piece;
                    piece = piece->next;
                    free(temp);
                }
                free(game.cells[rowIndex][columnIndex]); //Free the stack itself
            }
        }
    }

    //Free both players
    free(game.players[0]);
    free(game.players[1]);
}