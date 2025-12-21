#ifndef LOGGER_H
#define LOGGER_H
#include <stdarg.h>

void p(char* msg);
void d(char* msg);
void e(char* msg);
void s(char* msg);
void i(int intValue);
void done(void);
void done_nobr(void);
void fail(void);
void br();
void hbr();
void sp(int amount);


void printTableTitle(int indent, int numCols, const char *title, const char *type);
char **printHeader(int indent, int numCols, ...);
char **printRow(int indent, int numCols, ...);
void printPointer(int indent, int length);


void printTableTitleToFile(FILE *out, int indent, int numCols, const char *title, const char *type);
char **printHeaderToFile(FILE *out, int indent, int numCols, ...);
char **printRowToFile(FILE *out, int indent, int numCols, ...);
void printPointerToFile(FILE *out, int indent, int length);

#endif