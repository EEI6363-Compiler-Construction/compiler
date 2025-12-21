#ifndef RULES_H
#define RULES_H

#define RULES_COUNT 100


typedef struct {
    int productionSymbolCount;
    int productionSymbols[10];
    char* grammar;

} Production;


typedef struct {
    int productionsCount;
    Production productions[10];

} RHS;


typedef struct {
    NonTerminals lhs;
    RHS rhs;

} Rule;

extern Rule rules[RULES_COUNT];

#endif