#ifndef DOMINATION_COMPONENTS_H
#define DOMINATION_COMPONENTS_H

#include <stdint.h>

/**
 * \brief
 * An enum to store the colour of a player<br>
 */
typedef enum {
    BLANK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN
} Colour;


/**
 * \brief Represents a player in a game
 */
typedef struct {
    char name[24]; //!< The player name
    Colour colour; //!< The player's chosen colour representation
    unsigned int reservedCounter; //!< The number of pieces a player has reserved
} Player;

/**
 * \brief Represents a single game piece
 */
typedef struct Piece {
    Player *owner; //!< A pointer to the player that owns the piece
    struct Piece *next; //!< A pointer to the piece below it. NULL if this is the bottom-most piece
} Piece;

/**
 * \brief Represents a cell on the 8x8 game board
 */
typedef struct {
    Piece *head; //!< Pointer to the top-most piece in that cell
    Piece *tail; //!< Pointer to the bottom most piece in that cell
    uint8_t length; //!< The number of Pieces on the cell
    uint8_t rowIndex; //!< The row index of the cell
    uint8_t columnIndex; //!< The column index of the cell
} Cell;

/**
 * \brief Represents an instance of the game
 */
typedef struct {
    Player *players[2]; //!< Pointers to player1 and player2
    Cell *cells[8][8]; //!< an 8x8 2D array of cells. If NULL, cell is not a valid position.
    unsigned short moveIndex; //!< The current move index
} Game;


#endif //DOMINATION_COMPONENTS_H
