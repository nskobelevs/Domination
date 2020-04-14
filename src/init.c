#include "init.h"

static Player *initialisePlayer(Player *otherPlayer);

/**
 * \relates Player
 * Initialises a player
 * @param otherPlayer The player1Colour to avoid both players having same colour
 * @return A pointer to the initialised player
 */
static Player *initialisePlayer(Player *otherPlayer) {
#ifdef DEBUG
    LOG("INFO: %s(%p) called\n", __func__, otherPlayer);
#endif

    Player *player = (Player *) malloc(sizeof(Player));

    //If malloc fails
    if (player == NULL) {

#ifdef DEBUG
        LOG("%s:%d ERROR: malloc() returned NULL. Exiting...\n", __FILE__, __LINE__);
        endLog();
#endif

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
#ifdef DEBUG
    LOG("INFO: %s() called\n", __func__);
#endif

    //Allocating memory for board
    Game *game = (Game *) malloc(sizeof(*game));

    //In the rare case that malloc fails
    if (game == NULL) {

#ifdef DEBUG
      LOG("%s:%d ERROR: malloc() returned NULL. Exiting...\n", __FILE__, __LINE__);
      endLog();
#endif
        fprintf(stderr, "Unable to allocate memory for the game\n");
        exit(EXIT_FAILURE);
    }

    //Initialising both players
    Player *player1 = initialisePlayer(NULL);
    Player *player2 = initialisePlayer(player1);

    game->players[0] = player1;
    game->players[1] = player2;
    game->moveIndex = 0;

#ifdef DEBUG
    LOG("INFO: game @ %p, game->player1 @ %p, game->player2 @ %p, game->?moveIndex = %d\n", game, game->players[0], game->players[1], game->moveIndex);
#endif

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

#ifdef DEBUG
                LOG("%s:%d ERROR: malloc() returned NULL. Exiting...\n", __FILE__, __LINE__);
#endif
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
#ifdef DEBUG
            LOG("INFO: cell (%d,%d) @ %p, cell->head @ %p, cell->tail @ %p, cell->length = %u\n",
                    columnIndex, rowIndex, cell, cell->head, cell->tail, cell->length);
#endif
        }
    }

    Piece *piece = NULL;
    int cellIndex = 0;
    //Setting the inner 6x6 grid with pieces
    for (int rowIndex = 0; rowIndex < 6; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 6; columnIndex++) {
            piece = (Piece *) malloc(sizeof(Piece));
            if (piece == NULL) {

#ifdef DEBUG
            LOG("%s:%d ERROR: malloc() returned NULL. Exiting...\n", __FILE__, __LINE__);
#endif

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

#ifdef DEBUG
            LOG("INFO: piece @ %p, piece->next @ %p, piece->owner @ %p\n", piece, piece->next, piece->owner);
#endif

            cell = game->cells[rowIndex + 1][columnIndex + 1];

            cell->head = piece;
            cell->tail = piece;
            cell->length = 1;

#ifdef DEBUG
            LOG("INFO: cell (%d,%d) @ %p, cell->head @ %p, cell->tail @ %p, cell->length = %u\n",
                columnIndex, rowIndex, cell, cell->head, cell->tail, cell->length);
#endif
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
#ifdef DEBUG
    LOG("INFO: %s(%p) called\n", __func__, game);
#endif

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
#ifdef DEBUG
                    LOG("INFO: piece @ %p free'd\n", piece);
#endif
                    //Freeing the pieces
                    free(temp);
                }

#ifdef DEBUG
                LOG("INFO: stack @ %p free'd\n", stack);
#endif


                free(stack); //Free the stack itself
            }
        }
    }

#ifdef DEBUG
    LOG("INFO: player @ %p free'd\n", game->players[0]);
    LOG("INFO: player @ %p free'd\n", game->players[1]);
#endif

    //Free both players
    free(game->players[0]);
    free(game->players[1]);

#ifdef DEBUG
    LOG("INFO: game @ %p free'd\n", game);
#endif

    //Free game itself
    free(game);

}