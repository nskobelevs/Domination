#include "gameLogic.h"

static bool playerCanMakeMove(Game *game, Player *player);
static void shortenCell(Cell *cell);

/**
 * \relates Cell
 * \relates Game
 * Creates a new piece owned by player and places it on top of cell
 * @param cell The cell where to place the new cell
 * @param player The player which will own the cell
 * \note
 * Assume's player's reservedCounter > 0
 */
static void pushPiece(Cell *cell, Player *player) {

    Piece *piece = (Piece *)malloc(sizeof(*piece));

    if (piece == NULL) {
        fprintf(stderr, "Unable to allocate memory for a piece\n");
        exit(EXIT_FAILURE);
    }

    piece->owner = player;
    piece->next = cell->head;
    cell->head = piece;
    cell->length += 1;
    player->reservedCounter--;

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

    Piece *newSourceHead = source->head;
    Piece *piece = source->head;  //This is the piece above the newSourceHead;
    for (unsigned int index = 0; index < count; index++) {
        piece = newSourceHead;
        newSourceHead = newSourceHead->next;
    }

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

    Player *stackOwner = cell->head->owner;

    Piece *newTail = cell->head->next->next->next->next;

    Piece *piece = newTail->next;
    Piece *current = NULL;

    //Looping until the bottom of the stack
    while (piece != NULL) {
        if (piece->owner == stackOwner) {
            stackOwner->reservedCounter++;
        }
        current = piece;
        piece = piece->next;
        free(current);
    }

    newTail->next = NULL;
    cell->tail = newTail;
    cell->length = 5;
}

/**
 * \relates Cell
 * Gets the taxicab distance between two cells
 * @param cell1 Cell1
 * @param cell2 Cell2
 * @return The taxicab distance between the two cells
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
    Cell *sourceCell, *destinationCell;
    bool placeReservedPiece = false;
    Player *currentPlayer, *otherPlayer;
    unsigned int count;

    while (true) {
        currentPlayer = game->players[game->moveIndex % 2];
        otherPlayer = game->players[(game->moveIndex + 1) % 2];

        sourceCell = askUserForCell(game, NULL, &placeReservedPiece, -1);

        if (placeReservedPiece) {
            pushPiece(sourceCell, currentPlayer);
        } else {
            count = askCount(game, sourceCell);
            destinationCell = askUserForCell(game, sourceCell, NULL, count);
            movePieces(sourceCell, destinationCell, count);
        }

        if (playerCanMakeMove(game, otherPlayer) == false) {
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
    if (player->reservedCounter > 0)
        return true;

    Cell *cell;

    //Looping through cell grid
    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {
            cell = game->cells[rowIndex][columnIndex];

            //Skipping invalid and empty cells
            if (cell == NULL || cell->head == NULL)
                continue;

            //If stack is owned by player, they can make a valid move
            if (cell->head->owner == player) {
                return true;
            }
        }
    }

    //If the whole board has been looped, no move for player exists
    return false;
}