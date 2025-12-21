#ifndef PARSETABLE_H
#define PARSETABLE_H

#define MAX_NONTERMINALS 54
#define MAX_TERMINALS 52
#define TERMINALS_START 100
#define TERMINALS_END 152

#define EMPTY_ENTRY -1


extern int parseTable[MAX_NONTERMINALS][MAX_TERMINALS];

void initParseTable(void);
void populateParseTable(void);

#endif