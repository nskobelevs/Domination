#include "init.h"

static Player *initialisePlayer(Player *otherPlayer);

/**
 * \relates Player
 * Initialises a player
 * @param otherPlayer The player1Colour to avoid both players having same colour
 * @return A pointer to the initialised player
 */
static Player *initialisePlayer(Player *otherPlayer) {
    Player *player = (Player *) malloc(sizeof(Player));

    //If malloc fails
    if (player == NULL) {
        fprintf(stderr, "Unable to allocate memory for a player");
        exit(1);
    }

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
    //Allocating memory for board
    Game *game = (Game *) malloc(sizeof(*game));

    //In the rare case that malloc fails
    if (game == NULL) {
        fprintf(stderr, "Unable to allocate memory for the game\n");
        exit(EXIT_FAILURE);
    }

    //Initialising both players
    Player *player1 = initialisePlayer(NULL);
    Player *player2 = initialisePlayer(player1);

    game->players[0] = player1;
    game->players[1] = player2;
    game->moveIndex = 0;

    //Squared distance from the centre of the board to the cell being created
    double squaredDistance = 0;

    Cell *cell = NULL;

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
            cell = (Cell *)malloc(sizeof(Cell));
            if (cell == NULL) {
                fprintf(stderr, "Error allocating memory for a cell\n");
                exit(EXIT_FAILURE);
            }

            //Initialising cell to null
            cell->head = NULL;
            cell->tail = NULL;
            cell->length = 0;
            cell->rowIndex = rowIndex;
            cell->columnIndex = columnIndex;
            game->cells[rowIndex][columnIndex] = cell;
        }
    }

    Piece *piece = NULL;
    int cellIndex = 0;
    //Setting the inner 6x6 grid with pieces
    for (int rowIndex = 0; rowIndex < 6; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 6; columnIndex++) {
            piece = (Piece *) malloc(sizeof(Piece));
            if (piece == NULL) {
                fprintf(stderr, "Error allocating memory for game piece\n");
                exit(1);
            }
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
    Cell *stack = NULL;
    Piece *piece = NULL;
    Piece *temp = NULL;

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
                free(stack); //Free the stack itself
            }
        }
    }
    //Free both players
    free(game->players[0]);
    free(game->players[1]);
    //Free game itself
    free(game);

}