//
// Created by Nikita Skobelevs on 09/04/2020.
//

//This header file is shared by gui_posix and gui_windows

#ifndef DOMINATION_GUI_H
#define DOMINATION_GUI_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "gameLogic.h"
#include "components.h"

extern bool usingCurses;

void printTitle(void);

void askPlayerForName(Player *player, Player *otherPlayer);

void askPlayerForColour(Player *player, Player *otherPlayer);

void printBoard(Game *game, Cell *selectedCell);

Cell *selectCell(Game *game, Cell *sourceCell, bool *placeReservedPiece, unsigned int maxDist);

unsigned int askCount(Game *game, Cell *source);

void printWinner(Game *game, Player *player);

//Curses only functions
void initCurses(void);
void endCurses(void);

static const char *colours[] = {
        "",
        "Red",
        "Green",
        "Yellow",
        "Blue",
        "Magenta",
};

static const char *boardString[] = {
        "    0       1       2       3       4       5       6       7    ",
        "                ┌───────┬───────┬───────┬───────┐                ",
        "                │       │       │       │       │                ",
        "                │       │       │       │       │                ",
        "                │       │       │       │       │                ",
        "        ┌───────┼───────┼───────┼───────┼───────┼───────┐        ",
        "        │       │       │       │       │       │       │        ",
        "        │       │       │       │       │       │       │        ",
        "        │       │       │       │       │       │       │        ",
        "┌───────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┐",
        "│       │       │       │       │       │       │       │       │",
        "│       │       │       │       │       │       │       │       │",
        "│       │       │       │       │       │       │       │       │",
        "├───────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┤",
        "│       │       │       │       │       │       │       │       │",
        "│       │       │       │       │       │       │       │       │",
        "│       │       │       │       │       │       │       │       │",
        "├───────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┤",
        "│       │       │       │       │       │       │       │       │",
        "│       │       │       │       │       │       │       │       │",
        "│       │       │       │       │       │       │       │       │",
        "├───────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┤",
        "│       │       │       │       │       │       │       │       │",
        "│       │       │       │       │       │       │       │       │",
        "│       │       │       │       │       │       │       │       │",
        "└───────┼───────┼───────┼───────┼───────┼───────┼───────┼───────┘",
        "        │       │       │       │       │       │       │        ",
        "        │       │       │       │       │       │       │        ",
        "        │       │       │       │       │       │       │        ",
        "        └───────┼───────┼───────┼───────┼───────┼───────┘        ",
        "                │       │       │       │       │                ",
        "                │       │       │       │       │                ",
        "                │       │       │       │       │                ",
        "                └───────┴───────┴───────┴───────┘                "};

static const char *titleString[] = {
        "  _____                  _             _   _             ",
        " |  __ \\                (_)           | | (_)            ",
        " | |  | | ___  _ __ ___  _ _ __   __ _| |_ _  ___  _ __  ",
        " | |  | |/ _ \\| '_ ` _ \\| | '_ \\ / _` | __| |/ _ \\| '_ \\ ",
        " | |__| | (_) | | | | | | | | | | (_| | |_| | (_) | | | |",
        " |_____/ \\___/|_| |_| |_|_|_| |_|\\__,_|\\__|_|\\___/|_| |_|"};

#endif //DOMINATION_GUI_H
