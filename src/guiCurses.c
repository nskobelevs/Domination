//
// Created by Nikita Skobelevs on 09/04/2020.
//

#include "gui.h"
#include <ncurses.h>

bool usingCurses = true;

static WINDOW *gameWindow = NULL;
static WINDOW *cellWindow = NULL;

void initCurses(void) {
    stdscr = initscr();
    noecho();
    curs_set(0);
//    COLOR_BLACK is the default terminal background
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(BLANK, COLOR_WHITE, COLOR_BLACK);
}

void endCurses() {
    endwin();
}

void printTitle(void) {
    int xOffset = (COLS - 57) / 2;
    WINDOW *titleWindow = newwin(6, 57, 0, xOffset);
    for (int lineIndex = 0; lineIndex < 6; lineIndex++) {
        mvwprintw(titleWindow, lineIndex, 0, titleString[lineIndex]);
    }
    wrefresh(titleWindow);
    delwin(titleWindow);
}

void askPlayerForName(Player *player, Player *otherPlayer) {
    int windowWidth = 50;
    int windowHeight = 10;

    int xOffset = (COLS - windowWidth) / 2;
    int yOffset = 7;

    WINDOW *nameWindow = newwin(windowHeight, windowWidth, yOffset, xOffset);

    int textXOffset = (windowWidth - 23) / 2;
    while (true) {
         box(nameWindow, 0, 0);
         mvwprintw(nameWindow, 2, textXOffset, "Please enter your name:");
         mvwchgat(nameWindow, 6, textXOffset, sizeof(player->name) + 1, A_STANDOUT, 0, NULL);
         wattron(nameWindow, A_STANDOUT);
         echo();
         curs_set(1);
         wgetnstr(nameWindow, player->name, sizeof(player->name));
         curs_set(2);
         noecho();
         wattroff(nameWindow, A_STANDOUT);

         if (otherPlayer != NULL && strcmp(player->name, otherPlayer->name) == 0) {
             werase(nameWindow);
             mvwprintw(nameWindow, 4, (windowWidth - 37)/2, "Both players can't have the same name");
         } else {
             break;
         }
    }

    delwin(nameWindow);
    endwin();
}

//TODO: void askPlayerForColour(Player *player, Player *otherPlayer);
void askPlayerForColour(Player *player, Player *otherPlayer) {
    int windowWidth = 50;
    int windowHeight = 10;

    int xOffset = (COLS - windowWidth) / 2;
    int yOffset = 7;

    WINDOW *colourWindow = newwin(windowHeight, windowWidth, yOffset, xOffset);
    keypad(colourWindow, true);
    box(colourWindow, 0, 0);
    curs_set(0);

    char temp[64];

    sprintf(temp, "%s, pick your colour: ", player->name);

    int textOffset = (windowWidth - strlen(temp)) / 2;

    int key = 0;
    int selectedColourIndex = 0;

    do {

        if (key == 10) {
            break;
        } else if (key == KEY_RIGHT) {
            selectedColourIndex++;
        } else if (key == KEY_LEFT) {
            selectedColourIndex--;
        }

        selectedColourIndex = ((selectedColourIndex + 5) % 5);


        mvwprintw(colourWindow, 2, textOffset, temp);
        wmove(colourWindow, 6, 5);
        for (int colourIndex = 1; colourIndex < 6; colourIndex++) {
            if (selectedColourIndex + 1 == colourIndex)
                wattron(colourWindow, A_REVERSE);
            wprintw(colourWindow, "[%s]", colours[colourIndex]);
            wattroff(colourWindow, A_REVERSE);
            if (colourIndex != 5)
                wprintw(colourWindow, " ");
        }

    } while ((key = wgetch(colourWindow)));

    player->colour = selectedColourIndex + 1;

    delwin(colourWindow);
}

//TODO: void printBoard(Game *game, Cell *selectedCell);
void printBoard(Game *game, Cell *selectedCell) {

}

//TODO: Cell *selectCell(Game *game, Cell *sourceCell, bool *placeReservedPiece, unsigned int maxDist);
Cell *selectCell(Game *game, Cell *sourceCell, bool *placeReservedPiece, unsigned int maxDist) {

}

//TODO: unsigned int askCount(Game *game, Cell *source);
unsigned int askCount(Game *game, Cell *source) {

}

//TODO: void printWinner(Game *game, Player *player);
void printWinner(Game *game, Player *player) {

}