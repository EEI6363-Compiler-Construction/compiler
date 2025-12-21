#ifndef SYMBOL_TABLE_PRINTER_H
#define SYMBOL_TABLE_PRINTER_H

#include "symbol_table.h"
#include <stdio.h> 

void printSymbolTable(SymbolTable *table, int indent);
void printSymbolTableToFile(SymbolTable *table, int indent);

#endif