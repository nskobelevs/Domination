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
static void printMessage(char *line1, char *line2, char *line3);

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
        printf("\t> ");
        fgets(player->name, sizeof(player->name), stdin);

        //Cut of \n at the end
        player->name[strlen(player->name) - 1] = '\0';

        //Checking names aren't the same
        if (otherPlayer != NULL && strcmp(player->name, otherPlayer->name) == 0)
            printCentered(printWidth, '-', "Both players can't have the same name");
        else
            break;
    }
}

void askPlayerForColour(Player *player, Player *otherPlayer) {
    printCentered(printWidth, '-', "Select your colour [index]:");
    for (int index = 1; index < 6; index++) {
        if (otherPlayer == NULL || index != (int)otherPlayer->colour)
            printCentered(printWidth, ' ', "%d) %s", index, colours[index]);
    }

    int colourValue;

    while (1) {
        printf("\t> ");
        colourValue = getchar() - '0';

        while ((getchar() != '\n'));

        if (otherPlayer != NULL && colourValue == otherPlayer->colour)
            printCentered(printWidth, ' ', "Both players can't have the same colour");
        else if (colourValue < 1 || colourValue > 6)
            printCentered(printWidth, ' ', "Colour index out of range");
        else
            break;

        printCentered(printWidth, '-', "Select your colour [index]:");
    }

    player->colour = (Colour)colourValue;
}

void printBoard(Game *game, Cell *selectedCell) {
    Cell *cell;
    char colourLetter;
    int cellY;
    bool printedFirst;

    Piece *tempPiece;

    char stackString[5];
    int strIndex;

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

                    strIndex = 0;

                    tempPiece = cell->head;
                    while (tempPiece != NULL) {
                        colourLetter = colours[(int)tempPiece->owner->colour][0];
                        stackString[strIndex++] = colourLetter;
                        tempPiece = tempPiece->next;
                    }

                    stackString[strIndex] = '\0';

                    printf("%s%c%5s%c│",
                            printedFirst ? "" : "│",
                           cell == selectedCell ? '[' : ' ',
                            stackString,
                           cell == selectedCell ? ']' : ' '
                            );
                }
                printedFirst = true;

            }
            printf("\n");
        } else {
            printf("%s\n", boardString[pixelY]);
        }
    }
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
}

static void printMessage(char *line1, char *line2, char *line3) {
    printCentered(printWidth, '-', "");
    printCentered(printWidth, ' ', line1);
    printCentered(printWidth, ' ', line2);
    printCentered(printWidth, ' ', line3);
}

Cell *selectCell(Game *game, bool selectingSource, bool *placeReservedPiece) {
    char temp[64];

    Player *currentPlayer = game->players[game->moveIndex % 2];

    printCentered(printWidth, '-', "");
    if (selectingSource) {
        printCentered(printWidth, ' ', "%s's Move", currentPlayer->name);
        printCentered(printWidth, ' ', "You have %d pieces reserved", currentPlayer->reservedCounter);
    } else {
        printCentered(printWidth, ' ', "Select where to move the pieces to");
        printf("\n");
    }

    unsigned cellX, cellY, scanfReturn;

    Cell *cell;
    while (true) {
        printCentered(printWidth, ' ', "Please pick a cell (x and y separated by a space)", "");
        printf("\t> ");
        fgets(temp, sizeof(temp), stdin);
        scanfReturn = sscanf(temp,"%d %d", &cellX, &cellY);

        if (scanfReturn != 2) {
            printCentered(printWidth, '-', "Not enough inputs. Please input x and y seperated by a space");
            continue;
        } else if (cellX < 0 || cellY < 0 || cellX > 7 || cellY > 7) {
            printBoard(game, NULL);
            printMessage("Position out of range. Input must be 0 <= x <= 7", "", "");
            continue;
        }


        cell = game->cells[cellY][cellX];
        if (cell == NULL) {
            printBoard(game, NULL);
            printf("\n");
            printMessage("That square ins't part of the game", "", "");
            printf("\n");
        }
        else if (cell->head == NULL) {
            printBoard(game, NULL);
            printMessage("Can't choose an empty cell", "", "");
            printf("\n");
        } else if (selectingSource) {
            printBoard(game, cell);
            printCentered(printWidth, '-', "");
            printCentered(printWidth, ' ', "Selected cell at (%d, %d)", cellX, cellY);
            printCentered(printWidth, ' ', "What would you like to do?");
            printCentered(printWidth, ' ', "Move pieces [move] / Place reserved piece [place]");

            printf("\t> ");
            fgets(temp, sizeof(temp), stdin);

            if (strncmp(temp, "place", 5) == 0) {
                *placeReservedPiece = true;
                break;
            } else if (strncmp(temp, "move", 4) == 0) {
                if (cell->head->owner == currentPlayer) {
                    *placeReservedPiece = false;
                    break;
                } else {
                    printCentered(printWidth, ' ', "You can't move your opponent's stack");
                }
            } else {
                printBoard(game, NULL);
                printCentered(printWidth, ' ', "Invalid input");
                printf("\n");
                printCentered(printWidth, '-', "");
            }
        } else {
            break;
        }
    }

    return cell;
}
