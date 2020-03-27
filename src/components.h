//
// Created by nskobelevs on 25/03/2020.
//

#ifndef DOMINATION_COMPONENTS_H
#define DOMINATION_COMPONENTS_H

#include <stdint.h>
#include <ncurses.h>

typedef uint8_t byte;

//Represents the colour of a Player's pieces
//Starts at 1 because these values are used for colours and
//the index cant be 0
typedef enum _colour {
    RED = 1, GREEN, YELLOW, BLUE, MAGENTA, CYAN
} Colour;

//Represents a player
typedef struct _player {
    char name[24];                  //Name of the player
    Colour colour;                  //Their chosen colour
    unsigned int reservedCounter;   //How many pieces they have reserved
} Player;

//A game piece. Links to the next piece
typedef struct _piece {
    Player *owner;                  //Pointer to player who owns the piece
    struct _piece *next;            //Pointer to the piece below it. NULL if it is the bottom piece
} Piece;

//A stack of game pieces. Links to the first piece and to the last.
//Also stores the length
//If head and tail are NULL, stack is empty.
typedef struct _cell {
    Piece *head;                    //Pointer to the top piece
    Piece *tail;                    //Pointer to the bottom piece
    byte length;                    //Number of pieces
} Cell;

//Represents a game
typedef struct _game {
    Player *players[2];             //Pointers to the two players
    Cell *cells[8][8];              //8x8 grid of cells. 3 cells in each corner are unused
    unsigned short moveIndex;       //Current move index. Used to determine which player's turn it is
    WINDOW *boardWindow;
} Game;

#endif //DOMINATION_COMPONENTS_H
