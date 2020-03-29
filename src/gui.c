//
// Created by nskobelevs on 25/03/2020.
//

#include <string.h>
#include <math.h>
#include <unistd.h>
#include "gui.h"

void printTitle(void) {
    char *titlesString[] = {"  _____                  _             _   _             ",
                            " |  __ \\                (_)           | | (_)            ",
                            " | |  | | ___  _ __ ___  _ _ __   __ _| |_ _  ___  _ __  ",
                            " | |  | |/ _ \\| '_ ` _ \\| | '_ \\ / _` | __| |/ _ \\| '_ \\ ",
                            " | |__| | (_) | | | | | | | | | | (_| | |_| | (_) | | | |",
                            " |_____/ \\___/|_| |_| |_|_|_| |_|\\__,_|\\__|_|\\___/|_| |_|"
    };
    const int lineCount = sizeof(titlesString) / sizeof(*titlesString);
    const int length = (int) strlen(titlesString[0]);
    const int offset = (COLS - length) / 2;

    for (int i = 0; i < lineCount; i++) {
        mvprintw(i, offset, titlesString[i]);
    }
    refresh();
}

void drawBoard(Game *game, int selectedCellX, int selectedCellY) {
    WINDOW *boardWindow = game->boardWindow;

    const char *boxString[] = {
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
    const int lineCount = sizeof(boxString) / sizeof(boxString[0]);
    box(boardWindow, 0, 0);
    for (int i = 0; i < lineCount; i++) {
        mvwprintw(boardWindow, i + 1, 1, boxString[i]);
    }

    Cell *cell;
    Piece *piece = NULL;

    int windowX;
    int windowY;

    bool isSelected;

    unsigned int character;

    //Drawing the individual pieces
    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {
            cell = game->cells[rowIndex][columnIndex];
            isSelected = (rowIndex == selectedCellY && columnIndex == selectedCellX);

            if (cell == NULL)
                continue;



            if (cell->head == NULL) {
                if (isSelected) {
                    wattron(boardWindow, COLOR_PAIR(BLANK));
                } else {
                    continue;
                }
            } else {
                piece = cell->head;
                wattron(boardWindow, COLOR_PAIR(piece->owner->colour));
                if (!isSelected) {
                    wattron(boardWindow, A_DIM);
                }
            }

            windowX = (columnIndex * 8) + 5;
            windowY = (rowIndex * 4) + 3;

            for (int xOffset = -3; xOffset <= 3; xOffset++) {
                for (int yOffset = -1; yOffset <= 1; yOffset++) {
                    if (xOffset == -3) {
                        mvwprintw(boardWindow, windowY + yOffset, windowX + xOffset, "▐"); //Left half block
                    } else if (xOffset == 3) {
                        mvwprintw(boardWindow, windowY + yOffset, windowX + xOffset, "▌"); //Right half block
                    } else {
                        mvwprintw(boardWindow, windowY + yOffset, windowX + xOffset, "█"); //Full block
                    }
                }
            }

            if (piece != NULL) {
                wattroff(boardWindow, COLOR_PAIR(piece->owner->colour));
            } else {
                wattroff(boardWindow, COLOR_PAIR(BLANK));
            }
            wattroff(boardWindow, A_DIM);
        }
    }
    wrefresh(boardWindow);
}

void namePrompt(Player *player, unsigned int playerIndex) {

    //width and height of the name prompt window
    int namePromptWidth = 55;
    int namePromptHeight = 9;

    //Initialising the window

    char promptText[40];
    sprintf(promptText, "Player %d, please enter your name:", playerIndex);

    //Making the prompt text and text box centered
    int offset = (int) (namePromptWidth - strlen(promptText)) / 2;
    int nameBoxOffset = (int) (namePromptWidth - sizeof(player->name)) / 2;
    int windowOffset = (COLS - namePromptWidth) / 2;

    WINDOW *promptWindow = newwin(namePromptHeight, namePromptWidth, 7, windowOffset);

    //Drawing the box, the text, and asking for user input
    box(promptWindow, 0, 0);
    mvwprintw(promptWindow, 2, offset, promptText);
    mvwchgat(promptWindow, 5, nameBoxOffset, sizeof(player->name) + 1, A_STANDOUT, 0, NULL);
    wattron(promptWindow, A_STANDOUT);

    echo();                                                                 //Allow typing
    curs_set(1);                                                            //Cursor set to blink
    wgetnstr(promptWindow, player->name, -1);                               //Get the string
    noecho();                                                               //Disallow typing
    curs_set(0);                                                            //Remove blinking cursor

    werase(promptWindow);                                                   //Erasing window
    wrefresh(promptWindow);

    delwin(promptWindow);                                                   //Delete the window

}

void colourPrompt(Player *player, Colour *player1Colour) {

    //Colours and their corresponding enum values
    const char *colourOptions[] = {
            "RED",
            "GREEN",
            "YELLOW",
            "BLUE",
            "MAGENTA",
            "CYAN"
    };

    const Colour colours[] = {
            RED,
            GREEN,
            YELLOW,
            BLUE,
            MAGENTA,
            CYAN,
    };

    //Number of colours
    const int colourCount = sizeof(colourOptions) / sizeof(*colourOptions);

    int colourMenuWidth = 55;
    int colourMenuHeight = 9;

    //Offset to centre the box
    int colourWindowOffset = (COLS - colourMenuWidth) / 2;

    //Menu for colour selection
    WINDOW *colourMenu = newwin(colourMenuHeight, colourMenuWidth, 7, colourWindowOffset);
    keypad(colourMenu, true); //Allow arrow keys
    box(colourMenu, 0, 0); //Draw box border


    char colourPromptString[100];
    sprintf(colourPromptString, "%s, please choose your colour: ", player->name);

    //Offset to center text inside box
    int colourPromptOffset = (int)(colourMenuWidth - strlen(colourPromptString)) / 2;
    mvwprintw(colourMenu, 2, colourPromptOffset, colourPromptString);


    //Key being pressed
    int key = 0;

    //Index of the currently selected colour
    int selectedIndex = 0;

    do {

        if (key == KEY_RIGHT) {
            selectedIndex += 1;
        } else if (key == KEY_LEFT) {
            selectedIndex -= 1;
        }

        //Loop around either side
        //+ colourCount because C calculated -1 % x as -1. We want -1 % x to be x-1
        selectedIndex = (selectedIndex + colourCount) % colourCount;

        wmove(colourMenu, 5, 3);
        for (int i = 0; i < colourCount; i++) {
            //If player1 colour was specified, and that's the colour we're currently drawing
            //Highlight it differently showing that it can#t be chosen again
            if (player1Colour != NULL && colours[i] == *player1Colour) {
                wattron(colourMenu, A_REVERSE);
            } else if (i == selectedIndex) {
                //Underline and blink currently selected colour
                wattron(colourMenu, A_UNDERLINE | A_BOLD);
            }

            //Enable currently drawn colour
            wattron(colourMenu, COLOR_PAIR(colours[i]));
            wprintw(colourMenu, "[%s]", colourOptions[i]);
            wattroff(colourMenu, COLOR_PAIR(colours[i]));
            wattroff(colourMenu, A_UNDERLINE | A_REVERSE | A_BOLD); //Turn off all attributes

            //Draw a space between colours
            if (i != colourCount - 1)
                waddstr(colourMenu, " ");
        }

        wrefresh(colourMenu);
    } while ((key = wgetch(colourMenu)) != 10 || (player1Colour != NULL && colours[selectedIndex] == *player1Colour));
    //This is looped until the user presses ENTER. However, the loop continues if the chosen colour is taken by player 1

    player->colour = colours[selectedIndex];

    //Clear window
    werase(colourMenu);
    wrefresh(colourMenu);

    //Delete window
    delwin(colourMenu);

    //END COLOUR PROMPT
}

void drawCell(WINDOW *cellWindow, Cell *cell, int selectedCount) {

    werase(cellWindow);
    box(cellWindow, 0, 0);

    int cellWindowWidth = getmaxx(cellWindow);

    Piece *piece = cell->head;

    int x = 0;
    int y = 0;

    int pieceHeight = 4;

    int startY = (5 - cell->length) * (pieceHeight + 1) + 1;
    wmove(cellWindow, startY, 1);

    int depth = 0;
    while (piece != NULL) {
        if (depth == selectedCount)
            wattron(cellWindow, A_DIM);

        getyx(cellWindow, y, x);

        wattron(cellWindow, COLOR_PAIR(piece->owner->colour));
        for (int xOffset = 0; xOffset < cellWindowWidth-2; xOffset++) {
            for (int yOffset = 0; yOffset < pieceHeight; yOffset++) {
                mvwprintw(cellWindow, y + yOffset, x + xOffset, "█");
            }
        }
        wattroff(cellWindow, COLOR_PAIR(piece->owner->colour));
        wmove(cellWindow, y + pieceHeight + 1, 1);

        piece = piece->next;
        depth += 1;
    }

    wattroff(cellWindow, A_DIM);


    wrefresh(cellWindow);
}

Coord getUserSelectedCell(Game *game, WINDOW *cellWindow) {
    static int selectedCellX = 1;
    static int selectedCellY = 1;

    Cell *cell;
    Piece *piece;
    int key = -1;
    while ((key = wgetch(game->boardWindow)) != 10) {
        //Moving around to select a cell
        //Checks out of bounds. NULL checks for invalid corners.
        if (key == KEY_RIGHT && selectedCellX != 7 && game->cells[selectedCellY][selectedCellX + 1] != NULL) {
            selectedCellX += 1;
        } else if (key == KEY_LEFT && selectedCellX != 0 && game->cells[selectedCellY][selectedCellX - 1] != NULL) {
            selectedCellX -= 1;
        } else if (key == KEY_UP && selectedCellY != 0 && game->cells[selectedCellY - 1][selectedCellX] != NULL) {
            selectedCellY -= 1;
        } else if (key == KEY_DOWN && selectedCellY != 7 && game->cells[selectedCellY + 1][selectedCellX] != NULL) {
            selectedCellY += 1;
        }
        drawCell(cellWindow, game->cells[selectedCellY][selectedCellX], 0);
        drawBoard(game, selectedCellX, selectedCellY);
    }

    Coord coord = {selectedCellX, selectedCellY};
    return coord;
}

void printMessage(int seconds, const char * const message) {
    int offset = (int)(COLS - strlen(message)) / 2;
    mvprintw(43, offset, message);
    refresh();
    sleep(seconds);
    move(42, offset);
    clrtobot();
    refresh();
}