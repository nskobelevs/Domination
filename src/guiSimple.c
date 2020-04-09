//
// Created by Nikita Skobelevs on 09/04/2020.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include "gui.h"

#define printWidth 68

static void printCentered(unsigned int width, char paddingChar, const char *format, ...);

static const char *colours[] = {
        "",
        "Red",
        "Green",
        "Yellow",
        "Blue",
        "Magenta",
        };

const char *boardString[] = {
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

/**
 * \relates Player
 * @param player The player which is being asked for their name
 * @param otherPlayer If not NULL, will stop player from having same name as otherPlayer
 */
void askPlayerForName(Player *player, Player *otherPlayer) {

    while (1) {
        //Asks player for their name
        printCentered(printWidth, ' ', "Please enter your name: ");
        fgets(player->name, sizeof(player->name), stdin);

        //Cut of \n at the end
        player->name[strlen(player->name) - 1] = '\0';

        //Checking names aren't the same
        if (otherPlayer != NULL && strcmp(player->name, otherPlayer->name) == 0)
            printCentered(printWidth, '-', "Both players can't have the same name\n");
        else
            break;
    }
}

void askPlayerForColour(Player *player, Player *otherPlayer) {
    printCentered(printWidth, '-', "Select your colour [index]:");
    for (int index = 1; index < 6; index++) {
        printCentered(printWidth, ' ', "%d) %s", index, colours[index]);
    }

    int colourValue;

    while (1) {
        colourValue = getchar() - '0';

        while ((getchar() != '\n'));

        if (otherPlayer != NULL && colourValue == otherPlayer->colour)
            printf("Both players can't have same colour\n");
        else if (colourValue < 1 || colourValue > 6)
            printf("Colour index out of range\n");
        else
            break;

        printCentered(printWidth, '-', "Select your colour [index]:");
    }

    player->colour = (Colour)colourValue;
}

void printBoard(Game *game) {
    Cell *cell;
    char colour;
    int cellY;
    bool printedFirst;

    for (int pixelY = 0; pixelY < 33; pixelY++) {


        //Printing cell details
        if (pixelY % 4 == 2) {

            // signifies whether the first piece in a row has been printed
            // The first cell prints "|   |"
            // The rest print "   |"
            // When combined, you get "|   |   |   |"
            printedFirst = false;

            //Converting pixel position to row index
            cellY = pixelY / 4;

            //Printing the row of cells
            for (int cellX = 0; cellX < 8; cellX++) {

                cell = game->cells[cellY][cellX];

                //Not valid cell. Empty
                if (cell == NULL) {
                    printf("        ");
                    continue;
                }

                //Cell contains no pieces
                if (cell->head == NULL) {
                    printf("%s       │", printedFirst ? "" : "│");
                } else {
                    colour = colours[(int)cell->head->owner->colour][0];
                    printf("%s %c   %d │", printedFirst ? "" : "│", colour, cell->length);
                }
                printedFirst = true;

            }
            printf("\n");
        } else {
            printf("%s\n", boardString[pixelY]);
        }
    }

//    for (int rowIndex = 0; rowIndex < 8; rowIndex++) {
//        for (int columnIndex = 0; columnIndex < 8; columnIndex++) {
//            cell = game->cells[rowIndex][columnIndex];
//            if (cell == NULL) {
//                printf("       ");
//                continue;
//            } else if (cell->head == NULL) {
//                printf("       ");
//                continue;
//            }
//
//            colour = colourLetters[(int)cell->head->owner->colour];
//            printf(" %c %d ", colour, cell->length);
//        }
//        printf("\n");
//    }
}

static void printCentered(unsigned int width, char paddingChar, const char *format, ...) {
    char temp[128];

    va_list arg;
    va_start(arg, format);
    vsprintf(temp, format, arg);
    va_end(arg);

    unsigned int padding = (width - strlen(temp)) / 2;

    unsigned length = 0;

    for (int i = 0; i < padding && length < width; i++, length++)
        printf("%c", paddingChar);
    printf("%s", temp);

    length += strlen(temp);

    for (int i = 0; i < padding || length < width; i++, length++)
        printf("%c", paddingChar);

    printf("\n");

    fflush(stdout);
}
Cell *selectSource(Game *game) {
    char temp[64];

    Player *currentPlayer = game->players[game->moveIndex % 2];
    printCentered(printWidth, '-', "");
    printCentered(printWidth, ' ', "%s's Move", currentPlayer->name);
    printCentered(printWidth, ' ', "You have %d pieces reserved", currentPlayer->reservedCounter);
    printCentered(printWidth, ' ', "Please pick a cell (x and y separated by a space)", "");

    unsigned cellX, cellY, scanfReturn;

    Cell *cell;
    while (true) {
        scanfReturn = scanf("%d %d", &cellX, &cellY);

        printf("%d %d\n", cellX, cellY);

        if (scanfReturn != 2) {
            printCentered(printWidth, '-', "Not enough inputs. Please input x and y seperated by a space");
            while ((getchar() != '\n'));
            continue;
        } else if (cellX < 0 || cellY < 0 || cellX > 7 || cellY > 7) {
            printCentered(printWidth, '-', "Position out of index. Input must be 0 <= x <= 7");
            continue;
        }


        cell = game->cells[cellY][cellX];
        if (cell == NULL)
            printCentered(printWidth, '-', "That square isn't part of the game");
        else if (cell->head == NULL)
            printCentered(printWidth, '-', "Can't pick an empty square");
        else if (cell->head->owner != currentPlayer)
            printCentered(printWidth, '-', "You can't pick your opponent's square");
        else
            printCentered(printWidth, '-', "Selected cell at (%d, %d)", cellX, cellY);
            return cell;
    }

}
