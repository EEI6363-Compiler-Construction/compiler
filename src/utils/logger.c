#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h> 

#define CELL_WIDTH 30
#define BUFFER_LENGTH 256


//! Print Utils

void e(char* msg) {
    fprintf(stderr, "\nError: %s\n", msg);
}

void p(char* msg) {
    printf("\n%s\n", msg);
}

void s(char* msg) {
    printf("\n%s", msg);
}

void i(int intValue) {
    printf("\n%d", intValue);
}

void done(void) {
    printf("   [done]\n");
}

void done_nobr(void) {
    printf("   [done]");
}

void fail(void) {

    printf("   [failed]\n");
}

void d(char* msg) {
    printf("\nDebug: %s\n", msg);
}

void br(){
    printf("\n\n\n");
    for (int i = 0; i < 200; i++) {
        printf("=");
    }
    printf("\n\n");
}

void hbr(){
    printf("\n");
    for (int i = 0; i < 200; i++) {
        printf("=");
    }
    printf("\n");
}

void sp(int amount){
    for (int i = 0; i < amount; i++) printf("\n");
}


//! Print Symbol Tables (Terminal)

void printIndent(int indent){
    for (int i = 0; i <= indent * CELL_WIDTH; i++) printf(" ");
}

void printPointer(int indent, int length){

    printIndent(indent);
    printf("\u21E7\n");
    printIndent(indent);
    printf("└");
    for (int j = 0; j < CELL_WIDTH * length; j++) printf("=");
    
    printf("┐\n");
    for (int j = 0; j < CELL_WIDTH * length; j++) printf(" ");
    
    printIndent(indent);
    printf(" \u21E9\n");
}

void printTableTitle(int indent, int numCols, const char *title, const char *type) {

    int tableWidth = numCols * CELL_WIDTH;

    printIndent(indent);
    printf("┌");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) printf("─");
        if (i < numCols - 1) printf("─");
    }
    printf("┐\n");


    printIndent(indent);

    char buffer[BUFFER_LENGTH];
    sprintf(buffer, "%s [%s]", title, type);

    int titleLen = (int)strlen(buffer);
    int leftPad = (tableWidth - titleLen) / 2;
    int rightPad = tableWidth - titleLen - leftPad;

    for (int i = 0; i < leftPad; i++) printf(" ");
    printf("%s", buffer);
    for (int i = 0; i < rightPad; i++) printf(" ");
    printf("\n");


    printIndent(indent);
    printf("└");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) printf("─");
        if (i < numCols - 1) printf("─");
    }
    printf("┘\n");
}

char **printHeader(int indent, int numCols, ...) {

    va_list args;
    va_start(args, numCols);
    
    char **cols = malloc(sizeof(char*) * numCols);
    if (!cols) {
        va_end(args);
        return NULL;
    }

    printIndent(indent);
    printf("┌");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) printf("─");
        if (i < numCols - 1) printf("┬");
    }
    printf("┐\n");

    printIndent(indent);
    printf("│");
    for (int i = 0; i < numCols; i++) {
        const char *text = va_arg(args, const char*);
        cols[i] = strdup(text);

        int padding = CELL_WIDTH - (int)strlen(text);
        int leftPad = 1; // one space on left
        int rightPad = padding - leftPad;
        printf("%*s%s%*s│", leftPad, "", text, rightPad, "");
    }
    printf("\n");

    printIndent(indent);
    printf("└");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) printf("─");
        if (i < numCols - 1) printf("┴");
    }
    printf("┘\n");

    va_end(args);
    return cols;
}

char **printRow(int indent, int numCols, ...) {

    va_list args;
    va_start(args, numCols);
    
    char **cols = malloc(sizeof(char*) * numCols);
    if (!cols) {
        va_end(args);
        return NULL;
    }

    printIndent(indent);
    printf("┌");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) printf("─");
        if (i < numCols - 1)  printf("┬");
    }
    printf("┐\n");

    printIndent(indent);
    printf("│");
    for (int i = 0; i < numCols; i++) {
        const char *text = va_arg(args, const char*);
        cols[i] = strdup(text);

        int padding = CELL_WIDTH - (int)strlen(text);
        int leftPad = 1; // one space on left
        int rightPad = padding - leftPad;
        printf("%*s%s%*s│", leftPad, "", text, rightPad, "");
    }
    printf("\n");

    printIndent(indent);
    printf("└");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) printf("─");
        if (i < numCols - 1)  printf("┴");
    }
    printf("┘\n");

    va_end(args);
    return cols;
}


//! Print Symbol Tables (File)

void printTableTitleToFile(FILE *out, int indent, int numCols, const char *title, const char *type) {

    int tableWidth = numCols * CELL_WIDTH;

    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");

    fprintf(out, "┌");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) fprintf(out, "─");
        if (i < numCols - 1) fprintf(out, "─");
    }
    fprintf(out, "┐\n");


    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");

    char buffer[BUFFER_LENGTH];
    sprintf(buffer, "%s [%s]", title, type);

    int titleLen = (int)strlen(buffer);
    int leftPad  = (tableWidth - titleLen) / 2;
    int rightPad =  tableWidth - titleLen - leftPad;

    for (int i = 0; i < leftPad; i++) fprintf(out, " ");
    fprintf(out, "%s", buffer);
    for (int i = 0; i < rightPad; i++) fprintf(out, " ");
    fprintf(out, "\n");


    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");

    fprintf(out, "└");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) fprintf(out, "─");
        if (i < numCols - 1) fprintf(out, "─");
    }
    fprintf(out, "┘\n");
}

void printPointerToFile(FILE *out, int indent, int length){

    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");
    fprintf(out, "\u21E7\n");

    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");
    fprintf(out, "└");
    for (int j = 0; j < CELL_WIDTH * length; j++) fprintf(out, "=");
    fprintf(out, "┐\n");

    for (int j = 0; j < CELL_WIDTH * length; j++) fprintf(out, " ");

    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");
    fprintf(out, " \u21E9\n");
}

char **printHeaderToFile(FILE *out, int indent, int numCols, ...) {

    va_list args;
    va_start(args, numCols);

    char **cols = malloc(sizeof(char*) * numCols);

    if (!cols) {
        va_end(args);
        return NULL;
    }

    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");
    fprintf(out, "┌");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) fprintf(out, "─");
        if (i < numCols - 1) fprintf(out, "┬");
    }
    fprintf(out, "┐\n");


    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");
    fprintf(out, "│");

    for (int i = 0; i < numCols; i++) {

        const char *text = va_arg(args, const char*);
        cols[i] = strdup(text);

        int padding = CELL_WIDTH - (int)strlen(text);
        int leftPad = 1;
        int rightPad = padding - leftPad;

        fprintf(out, "%*s%s%*s│",
                leftPad, "",
                text,
                rightPad, "");
    }

    fprintf(out, "\n");


    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");
    fprintf(out, "└");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) fprintf(out, "─");
        if (i < numCols - 1) fprintf(out, "┴");
    }
    fprintf(out, "┘\n");


    va_end(args);
    return cols;
}

char **printRowToFile(FILE *out, int indent, int numCols, ...) {

    va_list args;
    va_start(args, numCols);

    char **cols = malloc(sizeof(char*) * numCols);

    if (!cols) {
        va_end(args);
        return NULL;
    }

    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");
    fprintf(out, "┌");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) fprintf(out, "─");
        if (i < numCols - 1) fprintf(out, "┬");
    }
    fprintf(out, "┐\n");


    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");
    fprintf(out, "│");

    for (int i = 0; i < numCols; i++) {

        const char *text = va_arg(args, const char*);
        cols[i] = strdup(text);

        int padding = CELL_WIDTH - (int)strlen(text);
        int leftPad = 1;
        int rightPad = padding - leftPad;

        fprintf(out, "%*s%s%*s│",
            leftPad, "",
            text,
            rightPad, "");
    }

    fprintf(out, "\n");


    for (int i = 0; i <= indent * CELL_WIDTH; i++) fprintf(out, " ");

    fprintf(out, "└");
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) fprintf(out, "─");
        if (i < numCols - 1) fprintf(out, "┴");
    }
    fprintf(out, "┘\n");


    va_end(args);
    return cols;
}
