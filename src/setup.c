//
// Created by nskobelevs on 25/03/2020.
//

#include <stdio.h>
#include "setup.h"

static Player initialisePlayer(void);

//Asks the user to input their name and pick a colour
//TODO: Convert from command-line input to ncurses menu
static Player initialisePlayer(void) {
    Player player = {};

    printf("Please enter player name: ");

    //A format string is generated so that if the length of name inside player
    //struct is changed, this code will adapt.
    //If name has max length of 20, format Str will be %20s after the sprintf
    char formatStr[10];
    sprintf(formatStr, "%%%lus", sizeof(player.name));
    scanf(formatStr, player.name);

    printf("Please enter player colour: ");
    scanf("%d", &player.colour);

    return player;
}

Board initialiseBoard(void) {
    Board board = {};

    Player player1 = initialisePlayer();
    Player player2 = initialisePlayer();

    board.players[0] = player1;
    board.players[1] = player2;

    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {

        }
    }

    return board;

}