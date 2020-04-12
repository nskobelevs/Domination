//
// Created by nskobelevs on 25/03/2020.
//

#include "init.h"

static Player *initialisePlayer(unsigned int playerIndex, Player *otherPlayer);

/**
 * \relates Player
 * Initialises a player
 * @param playerIndex The index of the player. 0 for player1, 1 for player2
 * @param otherPlayer The player1Colour to avoid both players having same colour
 * @return A pointer to the initialised player
 */
static Player *initialisePlayer(unsigned int playerIndex, Player *otherPlayer) {
    Player *player = (Player *) malloc(sizeof(Player));

    askPlayerForName(player, otherPlayer);
    askPlayerForColour(player, otherPlayer);

    return player;
}

/**
 * \relates Game
 * Initialised the game and it' players
 * @return The game variables
 */
Game *initialiseGame(void) {

    //If building curses version
    if (usingCurses) {
        initCurses();
    }

    Game *game = (Game *) malloc(sizeof(*game));

    printTitle();


    //Initialising both players
    Player *player1 = initialisePlayer(1, NULL);
    Player *player2 = initialisePlayer(2, player1);
    exit(1);
    game->players[0] = player1;
    game->players[1] = player2;
    game->moveIndex = 0;

    //Squared distance from the centre of the board to the cell being created
    double squaredDistance;

    Cell *cell;

    //Fill the board with empty stacks
    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {

            squaredDistance = pow(rowIndex - 3.5, 2) + pow(columnIndex - 3.5, 2);

            //If cell is too far away, it's one of the invalid corner pieces
            if (squaredDistance > 18) {
                game->cells[rowIndex][columnIndex] = NULL;
                continue;
            }
            //Allocating memory
            cell = (Cell *) malloc(sizeof(Cell));
            //Initialising cell to null
            cell->head = NULL;
            cell->tail = NULL;
            cell->length = 0;
            cell->rowIndex = rowIndex;
            cell->columnIndex = columnIndex;
            game->cells[rowIndex][columnIndex] = cell;
        }
    }

    int cellIndex;
    Piece *piece;
    //Setting the inner 6x6 grid with pieces
    for (int rowIndex = 0; rowIndex < 6; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 6; columnIndex++) {
            piece = (Piece *) malloc(sizeof(Piece));
            piece->next = NULL;

            //Logic for figuring out the owner for every co-ordinate
            cellIndex = rowIndex * 6 + columnIndex;
            if (cellIndex % 4 == 0 || cellIndex % 4 == 1)
                piece->owner = game->players[0];
            else
                piece->owner = game->players[1];

            cell = game->cells[rowIndex + 1][columnIndex + 1];

            cell->head = piece;
            cell->tail = piece;
            cell->length = 1;
        }
    }

    return game;
}

/**
 * \relates Game
 * Free's all allocated memory
 * @param game Game instance
 */
void freeBoard(Game *game) {

    Cell *stack;
    Piece *piece;
    Piece *temp;

    //Looping through the 8x8 board
    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {
            stack = game->cells[rowIndex][columnIndex];

            if (stack != NULL) {
                piece = stack->head;
                //Free all pieces in a stack
                while (piece != NULL) {
                    temp = piece;
                    piece = piece->next;
                    //Freeing the pieces
                    free(temp);
                }
                free(game->cells[rowIndex][columnIndex]); //Free the stack itself
            }
        }
    }

    //Free both players
    free(game->players[0]);
    free(game->players[1]);

    //Free game itself
    free(game);

    //If building curses version
    if (usingCurses) {
        endCurses();
    }
}