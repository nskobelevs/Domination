//
// Created by nskobelevs on 25/03/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "utils.h"

static Player* initialisePlayer(void);

//Asks the user to input their name and pick a colour
//TODO: Convert from command-line input to ncurses menu
static Player* initialisePlayer(void) {
    Player *player = (Player *)malloc(sizeof(Player));

    printf("Please enter player name: ");

    //A format string is generated so that if the length of name inside player
    //struct is changed, this code will adapt.
    //If name has max length of 20, format Str will be %20s after the sprintf
    char formatStr[10];
    sprintf(formatStr, "%%%lus", sizeof(player->name));
    scanf(formatStr, player->name);

    printf("Please enter player colour: ");
    scanf("%d", &player->colour);

    return player;
}

//Initialises a Board objects. Add the players, cells and correct starting
//arrangement of pieces.
Board initialiseBoard(void) {
    Board board = {};

    Player *player1 = initialisePlayer();
    Player *player2 = initialisePlayer();

    board.players[0] = player1;
    board.players[1] = player2;
    board.moveIndex = 0;

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

                board.cells[rowIndex][columnIndex] = cell;
            } else {
                //Corner
                board.cells[rowIndex][columnIndex] = NULL;
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
                piece->owner = board.players[0];
            } else {
                piece->owner = board.players[1];
            }

            cell = board.cells[rowIndex + 1][columnIndex + 1];

            cell->head = piece;
            cell->tail = piece;
            cell->length = 1;
        }
    }

    return board;
}

//frees all allocated memory
void freeBoard(Board board) {

    Cell *stack;
    Piece *piece;
    Piece *temp;

    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {
            stack = board.cells[rowIndex][columnIndex];

            if (stack != NULL) {
                piece = stack->head;
                //Free all pieces in a stack
                while (piece != NULL) {
                    temp = piece;
                    piece = piece->next;
                    free(temp);
                }
                free(board.cells[rowIndex][columnIndex]); //Free the stack itself
            }
        }
    }

    //Free both players
    free(board.players[0]);
    free(board.players[1]);
}