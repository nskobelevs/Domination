//
// Created by nskobelevs on 25/03/2020.
//

#ifndef DOMINATION_COMPONENTS_H
#define DOMINATION_COMPONENTS_H

#include <stdint.h>

typedef uint8_t byte;

//Represents the colour of a Player's pieces
typedef enum _colour {
    RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
} Colour;

//Represents a player
typedef struct _player {
    char name[32];
    Colour colour;
} Player;

//A game piece. Links to the next piece
typedef struct _piece {
    Player *owner;
    struct _piece *next;
} Piece;

//A stack of game pieces. Links to the first piece and to the last.
//Also stores the length
//If head and tail are NULL, stack is empty.
typedef struct _stack {
    Piece *head;
    Piece *tail;
    byte length;
} Stack;

//Represents a game board.
typedef struct _board {
    Player players[2];
    Stack *cells[8][8];
    unsigned short moveIndex;
} Board;


#endif //DOMINATION_COMPONENTS_H
