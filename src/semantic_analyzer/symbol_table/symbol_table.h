#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../../syntax_analyzer/ast/ast.h"

#define SYMBOL_TABLE_SIZE 50
#define INT_SIZE 3
#define FLOAT_SIZE 8

typedef enum {
    CLASS_SYMBOL_TYPE = 0,
    IMPLEMENTATION_SYMBOL_TYPE,
    FUNCTION_SYMBOL_TYPE,
    VARIABLE_SYMBOL_TYPE,
    RETURN_SYMBOL_TYPE,
    FPARAM_SYMBOL_TYPE,
    VISIBILITY_SYMBOL_TYPE,
    TYPE_SYMBOL_TYPE,
    IMPL_SYMBOL_TYPE,
    IF_SYMBOL_TYPE,
    THEN_BLOCK_TYPE,
    ELSE_BLOCK_TYPE,

} SymbolType;

typedef enum {
    INT_TYPE,
    FLOAT_TYPE,
    VOID_TYPE,
    STRING_TYPE,
    VARIABLE_TYPE,
    FUNCTION_TYPE,
    INT_ARRAY_TYPE,
    FLOAT_ARRAY_TYPE,
    VARIABLE_ARRAY_TYPE,

} DataType;

typedef enum Visibility {
    PUBLIC_VIS,
    PRIVATE_VIS,
    LOCAL_VIS,

} Visibility;

typedef struct TypeInfo {

    DataType dataType;        
    int isArray;          
    int numDimensions;
    int *dimensions;      
    int size;             
    int offset;           
     
} TypeInfo;

typedef struct ParamInfo {

    char *identifier;        
    TypeInfo *typeInfo;           
     
} ParamInfo;

typedef struct Symbol {

    char *identifier;          
    SymbolType symbolType;    
    TypeInfo *typeInfo;        

    int numParams;     
    ParamInfo **paramList;     

    int numIsa;            
    char **isaList;    
    struct SymbolTable *link; 

    Visibility visibility;

    int line;                  
    int col;         

} Symbol;


typedef struct SymbolTable {
    Symbol *symbols[SYMBOL_TABLE_SIZE]; 
    int symbolCount;
    int scopeSize;                     
    struct Symbol *parent;          
} SymbolTable;



Symbol *createSymbol();
SymbolTable *createSymbolTable(Symbol *parent);
void generateSymbolTables();
void traverseAST(node *n);


Symbol *getSymbol(char *identifier);
char* getSymbolTypeString(int index);
char* getDataTypeString(int index);

#endif
