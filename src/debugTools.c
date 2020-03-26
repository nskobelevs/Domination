//
// Created by nskobelevs on 26/03/2020.
//
#include <stdio.h>
#include "debugTools.h"

//Prints a cell in the format:
//Cell at <address>: [owner -> owner -> owner -> owner] <length>
void printCell(Cell * const cell) {
    Piece *piece = cell->head;
    printf("Cell at %p: [", cell);
    while (piece != NULL) {
        printf("%s", piece->owner->name);
        piece = piece->next;
        if (piece != NULL) {
            printf(" -> ");
        }
    }
    printf("] (%d)\n", cell->length);
}