#ifndef DOMINATION_COMPONENTS_H
#define DOMINATION_COMPONENTS_H

#include <stdint.h>

/**
 * An enum to store the colour of a player<br>
 * 1-based indexing due to having to use it in <i>color_pair</i> which can't use 0 as index
 */
typedef enum {
    RED = 1, GREEN, YELLOW, BLUE, MAGENTA, CYAN, BLANK
} Colour;


/**
 * Represents a player in the game
 */
typedef struct {
    char name[24]; //!< The player name
    Colour colour; //!< The player's chosen colour representation
    unsigned int reservedCounter; //!< The number of pieces a player has reserved
} Player;

/**
 * Represents a single game piece
 */
typedef struct _piece {
    Player *owner; //!< A pointer to the player that own's the piece
    struct _piece *next; //!< A pointer to the piece below it. NULL if this is the bottom-most piece
} Piece;

/**
 * Represents a cell on the 8x8 game board
 */
typedef struct {
    Piece *head; //!< Pointer to the top-most piece in that cell
    Piece *tail; //!< Pointer to the bottom most piece in that cell
    uint8_t length; //!< The number of Pieces on the cell
    uint8_t rowIndex; //!< The row index of the cell
    uint8_t columnIndex; //!< The column index of the cell
} Cell;

/**
 * Represents an instance of the game
 */
typedef struct {
    Player *players[2]; //!< Pointers to player1 and player2
    Cell *cells[8][8]; //!< an 8x8 2D array of cells. If NULL, cell is not a valid position.
    unsigned short moveIndex; //!< The current move index
} Game;


#endif //DOMINATION_COMPONENTS_H
