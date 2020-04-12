#include "gui.h"

#define printWidth 68

static void printCentered(char paddingChar, const char *format, ...);
static void printSeparator(void);
static bool askAction(Game *game, Cell *cell, char *message, Player *currentPlayer, bool *placeReservedPiece);

//Uses clrscr() in conio to clear screen in Windows. Else uses ANSI escape characters
#ifdef _WIN32
    #include <conio.h>
#else
    #define clrscr() printf("\033[H\033[J");
#endif

/**
 * Clears the screen and print the title
 */
void printTitle(void) {
    unsigned int titleLines = sizeof(titleString) / sizeof(*titleString);
    clrscr();
    for (int i = 0; i < titleLines; i++) {
        printCentered(' ', titleString[i]);
    }
    printf("\n");
}

/**
 * \relates Player
 * @param player The player which is being asked for their name
 * @param otherPlayer If not NULL, will stop player from having same name as otherPlayer
 */
void askPlayerForName(Player *player, Player *otherPlayer) {
    clrscr();
    printTitle();
    while (1) {
        //Asks player for their name
        printCentered('-', "Please enter your name: ");
        printf("\t> ");
        fgets(player->name, sizeof(player->name), stdin);

        //Cut of \n at the end
        player->name[strlen(player->name) - 1] = '\0';

        //Checking names aren't the same
        if (otherPlayer != NULL && strcmp(player->name, otherPlayer->name) == 0)
            printCentered('-', "Both players can't have the same name");
        else
            break;
    }
}

/**
 * \relates Player
 * Asks a player what colour they want.<br>
 * If otherPlayer is not NULL, player won't be allowed to choose the same colour
 * @param player The player being asked for colour
 * @param otherPlayer Other player. Both players can't have the same colour
 */
void askPlayerForColour(Player *player, Player *otherPlayer) {
    clrscr();
    printTitle();
    printCentered('-', "%s, please select your colour [index]:", player->name);
    for (int index = 1; index < 6; index++) {
        if (otherPlayer == NULL || index != (int)otherPlayer->colour)
            printf("\t\t\t%d) %s\n", index, colourStrings[index]);
    }

    int colourValue;

    while (1) {
        printf("\t> ");
        colourValue = getchar() - '0';

        while ((getchar() != '\n')); //Consuming extra input

        //Error handling
        if (otherPlayer != NULL && colourValue == otherPlayer->colour)
            printCentered(' ', "Both players can't have the same colour");
        else if (colourValue < 1 || colourValue > 6)
            printCentered(' ', "Colour index out of range");
        else
            break;

        printCentered('-', "Select your colour [index]:");
    }

    //Setting the colour value
    player->colour = (Colour)colourValue;
}

void printBoard(Game *game, Cell *selectedCell) {

    clrscr();

    Cell *cell;
    char colourLetter;
    int cellY;
    bool printedFirst;

    Piece *tempPiece;

    char stackString[5];
    int strIndex;

    //Looping through every line of the board
    for (int pixelY = 0; pixelY < 34; pixelY++) {
        //Printing cell details if middle of cell reached
        if (pixelY % 4 == 3) {
            // signifies whether the first piece in a row has been printed
            // The first cell prints "|   |"
            // The rest print "   |"
            // When combined, you get "|   |   |   |"
            printedFirst = false;

            //Converting pixel position to row index
            cellY = pixelY / 4;
            printf(" %d ", cellY);
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
                    //Converting stack into a string
                    while (tempPiece != NULL) {
                        colourLetter = colourStrings[(int)tempPiece->owner->colour][0];
                        stackString[strIndex++] = colourLetter;
                        tempPiece = tempPiece->next;
                    }
                    stackString[strIndex] = '\0';

                    //Printing stack info
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

            //Print default board line if not middle of cell
            printf("   %s\n", boardString[pixelY]);

        }
    }
}

/**
 * Print's a formatted string, centered, using the paddingChar either side
 * @param paddingChar The character to use for padding either side
 * @param format Format string
 * @param ... Format string arguments
 */
static void printCentered(char paddingChar, const char *format, ...) {
    char temp[128];

    va_list arg;
    va_start(arg, format);
    vsprintf(temp, format, arg);
    va_end(arg);

    unsigned int padding = (printWidth - strlen(temp)) / 2;

    unsigned length = 0;

    for (int i = 0; i < padding && length < printWidth; i++, length++)
        printf("%c", paddingChar);

    printf("%s", temp);

    length += strlen(temp);

    for (int i = 0; i < padding || length < printWidth; i++, length++)
        printf("%c", paddingChar);

    printf("\n");
}

/**
 * Prints a seperator using '-'
 */
static void printSeparator(void) {
    printCentered('-', "");
}

/**
 * \relates Game
 * \relates Cell
 * Allows players to select a cell on the game board.<br>
 * If source is NULL. Player will be asked whether to move the stack or place a piece<br>
 * In that case, placeReservedPiece will be set to true is player wants to place a piece.
 * Must not be NULL in that case<br>
 * maxDist only used if sourceCell is not NULL. maxDist specifies the maximum
 * (taxicab) distance sourceCell can be from selected cell
 * \section
 * selectCell(game, NULL, bool*, 0) means asking for source<br>
 * selectCell(game, Cell*, NULL, int) means asking for destination<br>
 * @param game Game instance
 * @param sourceCell If asking for destination, the source cell asked previously.
 * @param placeReservedPiece If asking for source, will be set to true if player decided to place deserved piece
 * @param maxDist IF asking for destination. The max distance allowed from source cell
 * @return
 */
Cell *selectCell(Game *game, Cell *sourceCell, bool *placeReservedPiece, unsigned int maxDist) {
    char temp[32];
    unsigned int cellX, cellY, sscanfreturn, distance;
    Cell *cell;

    char message[64] = "";

    Player *currentPlayer = game->players[game->moveIndex % 2];
    Player *opponentPlayer = game->players[(game->moveIndex + 1) % 2];

    printBoard(game, sourceCell);
    if (sourceCell == NULL) {
        printCentered('-', "%s's Move (%s)", currentPlayer->name, colourStrings[(int)currentPlayer->colour]);
        printCentered(' ', "You have %d pieces reserved.", currentPlayer->reservedCounter);
        printCentered(' ', "%s has %d pieces reserved", opponentPlayer->name, opponentPlayer->reservedCounter);
    } else {
        printSeparator();
        printCentered(' ', "Select where you want to move your stack to");
        printCentered(' ', "You can move at most %d cells", maxDist);
    }

    while (true) {

        //Error messages
        if (strlen(message) != 0) {
            printBoard(game, sourceCell);
            printSeparator();
            printCentered(' ', message);
            printf("\n");
        }

        printCentered(' ', "Please select cell (x & y separated by space");
        printf("\t> ");
        fgets(temp, sizeof(temp), stdin);
        sscanfreturn = sscanf(temp, "%d %d", &cellX, &cellY);

        //Cells outside the board
        if (sscanfreturn != 2 || cellX < 0 || cellY < 0 || cellX > 7 || cellY > 7) {
            strcpy(message, "Invalid choice");
            continue;
        }

        cell = game->cells[cellY][cellX];

        //Corner cells which are invalid
        if (cell == NULL) {
            strcpy(message, "Invalid choice");
        } else if (cell == sourceCell) {
            strcpy(message, "Can't move to the cell you're starting from");
        } else if (sourceCell == NULL) {
            if( askAction(game, cell, message, currentPlayer, placeReservedPiece) ) {
                return cell;
            }
        } else {
            distance = getDistance(sourceCell, cell);
            if (distance > maxDist) {
                strcpy(message, "That cell is to far away");
                continue;
            }
            return cell;
        }
    }

}

/**
 * \realtes Game
 * Asks user whether to move, place, or go back
 * @param game Game instance
 * @param cell The cell currently selected
 * @param message Message to print to the screen
 * @param currentPlayer The player being asked the action
 * @param placeReservedPiece Will be set to true if player decided to place reserved pieces
 * @return boolean of whether player has chosen an action (true) or to go back (false)
 */
static bool askAction(Game *game, Cell *cell, char *message, Player *currentPlayer, bool *placeReservedPiece) {
    char temp[64];

    printBoard(game, cell);
    printSeparator();
    printCentered(' ', "Selected cell at (%d, %d)", cell->columnIndex, cell->rowIndex);
    printCentered(' ', "What would you like to do?");
    printCentered(' ', "[move] pieces / [place] reserved piece / go [back]");
    printf("\t> ");
    fgets(temp, sizeof(temp), stdin);

    if (strncmp(temp, "move", 4) == 0) {
        if (cell->head->owner != currentPlayer) {
            strcpy(message, "You can't move your opponent's stack");
        } else {
            *placeReservedPiece = false;
            return true;
        }
    } else if (strncmp(temp, "place", 5) == 0) {
        if (currentPlayer->reservedCounter == 0) {
            strcpy(message, "You don't have any reserved piece");
        } else {
            *placeReservedPiece = true;
            return true;
        }
    } else if (strncmp(temp, "back", 4) == 0) {
        strcpy(message, "Going back..");
    } else {
        strcpy(message, "Invalid choice");
    }

    return false;
}

/**
 * Asks the player how many pieces to move
 * @param game Game instance
 * @param source The cell being moved
 * @return The number of pieces. 1 <= return <= source->length
 */
unsigned int askCount(Game *game, Cell *source) {
    if (source->length == 1) {
        return 1;
    }
    char temp[64];
    char message[64] = "";

    unsigned int count, sscanfReturn;

    while (true) {
        printBoard(game, source);
        printSeparator();
        printCentered(' ', "This cell has %d pieces.", source->length);
        printCentered(' ', "How many do you want to move?");
        printf("\n\t> ");

        fgets(temp, sizeof(temp), stdin);
        sscanfReturn = sscanf(temp, "%d", &count);

        if (sscanfReturn != 1 || count < 1 || count > source->length) {
            strcpy(message, "Not a valid amount");
            continue;
        }

        break;
    }
}

/**
 * Prints the game winner
 * @param game Game instance
 * @param player The player who won
 */
void printWinner(Game *game, Player *player) {
    printBoard(game, NULL);
    printSeparator();
    printCentered(' ', "Congratulations!");
    printf("\n");
    printCentered(' ', "%s Wins!", player->name);
}