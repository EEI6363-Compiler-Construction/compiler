#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "symbol_table_printer.h"
#include "../syntax_analyzer/ast/ast.h"
#include "../utils/logger.h"

#define TABLE_STACK_SIZE 10

SymbolTable *globalTable;
SymbolTable *tableStack[TABLE_STACK_SIZE];
int tableTop = -1;


//! Utils

char* symbolTypes[] = {
    "CLASS_SYMBOL_TYPE",
    "IMPLEMENTATION_SYMBOL_TYPE",
    "FUNCTION_SYMBOL_TYPE",
    "VARIABLE_SYMBOL_TYPE",
    "RETURN_SYMBOL_TYPE",
    "FPARAM_SYMBOL_TYPE",
    "VISIBILITY_SYMBOL_TYPE",
    "TYPE_SYMBOL_TYPE",
    "IMPL_SYMBOL_TYPE",
    "IF_SYMBOL_TYPE",
    "THEN_BLOCK_TYPE",
    "ELSE_BLOCK_TYPE",
};

char* dataTypes[] = {
    "INT_TYPE",
    "FLOAT_TYPE",
    "VOID_TYPE",
    "STRING_TYPE",
    "VARIABLE_TYPE",
    "FUNCTION_TYPE",
    "INT_ARRAY_TYPE",
    "FLOAT_ARRAY_TYPE",
    "VARIABLE_ARRAY_TYPE",
};

char* getSymbolTypeString(int index) {
    return symbolTypes[index];
}

char* getDataTypeString(int index) {
    return dataTypes[index];
}


//! Creator Functions

Symbol *createSymbol() {

    Symbol *symbol = malloc(sizeof(Symbol));
    if (!symbol) return NULL;  

    symbol->identifier = NULL;
    symbol->symbolType = -1;     
    symbol->typeInfo = NULL;     

    symbol->numParams = 0;
    symbol->paramList = NULL;   

    symbol->numIsa = 0;
    symbol->isaList = NULL;
    symbol->link = NULL;     

    symbol->visibility = 0;

    symbol->line = -1;
    symbol->col = -1;

    return symbol;
}

SymbolTable *createSymbolTable(Symbol *parent) {

    SymbolTable *table = malloc(sizeof(SymbolTable));

    if (!table) {
        perror("Failed to allocate memory for SymbolTable");
        exit(EXIT_FAILURE);
    }

    table->symbolCount = 0;
    table->parent = parent;

    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        table->symbols[i] = NULL;
    }

    return table;
}

TypeInfo* createTypeInfo(char* type) {

    TypeInfo *typeInfo = malloc(sizeof(TypeInfo));

    if (strcmp(type, "integer") == 0){
        typeInfo->dataType = INT_TYPE;       
    } else if (strcmp(type, "float") == 0) {
        typeInfo->dataType = FLOAT_TYPE; 
    } else if (strcmp(type, "void") == 0) {
        typeInfo->dataType = VOID_TYPE; 
    }
    
    typeInfo->isArray = 0;       
    typeInfo->numDimensions = 0;  
    typeInfo->dimensions = NULL;  
    typeInfo->size = 0;
    typeInfo->offset = 0;         

    return typeInfo;
}


//! Setting Size

void setSize(TypeInfo* typeInfo){
    
    int size = 0;

    if (typeInfo->dataType == INT_TYPE){

        size = INT_SIZE;
        
    } else if (typeInfo->dataType == FLOAT_TYPE) {
        
        size = FLOAT_SIZE;
        
    }

    if (typeInfo->isArray == 1){
        
        int totalSize = 0;

        for (int i = 0; i < typeInfo->numDimensions; i++){
            totalSize += typeInfo->dimensions[i] * size;
        }
        typeInfo->size = totalSize;       


    } else {
        typeInfo->size = size;       
    }
    
}


//! Setting Offsets

Symbol *lookupOffsetSymbolAt(int index) {

    if (index < 0)
        return NULL;

    SymbolTable *table = tableStack[index];

    for (int i = table->symbolCount - 1; i >= 0; i--) {
        Symbol *sym = table->symbols[i];
        if (sym && sym->typeInfo && sym->typeInfo->size > 0) {
            return sym;
        }
    }

    return lookupOffsetSymbolAt(index - 1);
}

Symbol *reccuriveLookupOffsetSymbol() {
    return lookupOffsetSymbolAt(tableTop);
}

void setOffset(TypeInfo* typeInfo){

    Symbol *offsetSymbol = reccuriveLookupOffsetSymbol();

    if (!offsetSymbol || !offsetSymbol->typeInfo) {
        typeInfo->offset = 0;
        return;
    }

    typeInfo->offset = offsetSymbol->typeInfo->offset + offsetSymbol->typeInfo->size;

}



//! Symbol Table Helpers

int insertSymbol(SymbolTable *table, Symbol *symbol) {
    if(table->symbolCount >= SYMBOL_TABLE_SIZE) return 0;
    table->symbols[table->symbolCount++] = symbol;
    return 1;
}

void enterTableScope(SymbolTable *table) {
    if(tableTop < TABLE_STACK_SIZE - 1)
        tableStack[++tableTop] = table;
}

void leaveCurrentTableScope() {
    if(tableTop >= 0) {
        tableStack[tableTop] = NULL;
        tableTop--;
    }
}

SymbolTable *getCurrentTable() {
    return tableStack[tableTop];
}

SymbolTable *getParentTable() {
    return tableStack[tableTop - 1];
}

Symbol *getCurrentSymbol() {

    SymbolTable *currentTable = getCurrentTable();

    if (!currentTable) return NULL;
    if (currentTable->symbolCount == 0) return NULL;

    return currentTable->symbols[currentTable->symbolCount - 1];
}


//! Lookup Functions

Symbol* lookupSymbol(SymbolTable *table, const char *identifier) {

    if (!table || !identifier) return NULL;

    for (int i = 0; i < table->symbolCount; i++) {
        Symbol *sym = table->symbols[i];
        if (sym && sym->identifier && strcmp(sym->identifier, identifier) == 0) {
            return sym;
        }
    }
    return NULL;
}

Symbol* lookupSymbolAt(int index, const char *identifier) {
    if (index < 0 || !identifier) return NULL;

    SymbolTable *table = tableStack[index];

    for (int i = 0; i < table->symbolCount; i++) {
        Symbol *sym = table->symbols[i];
        if (sym && sym->identifier && strcmp(sym->identifier, identifier) == 0) {
            return sym;
        }
    }
    return lookupSymbolAt(index - 1, identifier);
}

Symbol* reccurlivelookupSymbol(const char *identifier) {
    return lookupSymbolAt(tableTop, identifier);
}

ParamInfo* lookupFparam(Symbol *symbol, const char *identifier) {

    if (!symbol || !identifier) return NULL;

    for (int i = 0; i < symbol->numParams; i++) {
        ParamInfo *info = symbol->paramList[i];
        if (info && info->identifier && strcmp(info->identifier, identifier) == 0) {
            return info;
        }
    }
    return NULL;
}

char* lookupIsa(Symbol *symbol, const char *name) {
    
    if (!symbol || !name) return NULL;

    for (int i = 0; i < symbol->numIsa; i++) {
        if (symbol->isaList[i] && strcmp(symbol->isaList[i], name) == 0) {
            return symbol->isaList[i];
        }
    }
    return NULL;
}


//! NOde Handlers 

void handleClassNode(node * treeNode) {

    Symbol *existingClass = lookupSymbol(globalTable, treeNode->value);

    if (existingClass) {
        printf("Semantic Error: Redeclaration of Class '%s'\n\n", treeNode->value);
        printf("%d | class %s\n", existingClass->line, treeNode->value);
        printf("%d | class %s\n\n", treeNode->line_no, treeNode->value);
        printf("Checking semantics");
        fail();
        exit(1);
    }
    
    Symbol *classSymbol = createSymbol();
    SymbolTable *classScopeTable = NULL;

    classSymbol->identifier = treeNode->value;
    classSymbol->symbolType = CLASS_SYMBOL_TYPE;
    classSymbol->line = treeNode->line_no;
    classSymbol->col = treeNode->col_no;
    
    if ((treeNode->nops >= 1 && treeNode->op[0]->type == CLASSBODY_NODE) ||
    (treeNode->nops >= 2 && treeNode->op[1]->type == CLASSBODY_NODE)) {
        classScopeTable = createSymbolTable(classSymbol);
        
    }

    classSymbol->link = classScopeTable;
    
    if (classScopeTable) enterTableScope(classScopeTable);
    
    for (int i = 0; i < treeNode->nops; i++) traverseAST(treeNode->op[i]);
    
    if (classScopeTable) leaveCurrentTableScope();
    
    insertSymbol(globalTable, classSymbol);
}

void handleInheritanceNode(node* treeNode) {

    for (int i = 0; i < treeNode->nops; i++) {

        Symbol *parentSymbol = getCurrentTable()->parent;
        char *childNodeValue = treeNode->op[i]->value;

        Symbol *existingClass = lookupSymbol(globalTable, childNodeValue);

        if (!existingClass) {
            printf("Semantic Error: Undeclared Inheritance used in Class '%s'\n\n", parentSymbol->identifier);
            printf("%d | isa %s\n\n", treeNode->line_no, childNodeValue);
            printf("Checking semantics");
            fail();
            exit(1);
        }


        char *existingInheritance = lookupIsa(parentSymbol, childNodeValue);

        if (existingInheritance) {
            printf("Semantic Error: Duplicate inheritance used in Class '%s'\n\n", globalTable->symbols[globalTable->symbolCount - 1]->identifier);
            printf("%d | isa %s\n\n", treeNode->line_no, childNodeValue);
            printf("Checking semantics");
            fail();
            exit(1);
        }

        parentSymbol->isaList = realloc(parentSymbol->isaList, sizeof(char*) * (parentSymbol->numIsa + 1));;
        parentSymbol->isaList[parentSymbol->numIsa] = childNodeValue;
        parentSymbol->numIsa++;


    }
}

void handleFuncDefNode(node * treeNode) {


    Symbol *existingFunc = lookupSymbol(getCurrentTable(), treeNode->value);

    if (existingFunc) { 
        printf("Semantic Error: Redeclaration of Function '%s'\n\n", treeNode->value);
        printf("%d | func %s\n", existingFunc->line, treeNode->value);
        printf("%d | func %s\n\n", treeNode->line_no, treeNode->value);
        printf("Checking semantics");
        fail();
        exit(1);
    }

    Symbol *funcSymbol = createSymbol();
    funcSymbol->identifier = treeNode->value;
    funcSymbol->symbolType = FUNCTION_SYMBOL_TYPE;
    funcSymbol->link = NULL;
    funcSymbol->line = treeNode->line_no;
    funcSymbol->col = treeNode->col_no;
    funcSymbol->visibility = (strcmp(treeNode->op[0]->value, "private") == 0) ? PRIVATE_VIS : PUBLIC_VIS;

    funcSymbol->typeInfo = createTypeInfo(treeNode->op[1]->value);
    /* printf("%s\n", getDataTypeString(funcSymbol->typeInfo->dataType) ); */

    insertSymbol(getCurrentTable(), funcSymbol);
    
    if (treeNode->nops >= 3){
        for (int i = 0; i < treeNode->op[2]->nops; i++) {
            traverseAST(treeNode->op[2]->op[i]);
        }
    }
    

}

void handleFparamNode(node *treeNode) {

    SymbolTable *currentTable = getCurrentTable();
    Symbol *currentSymbol = getCurrentSymbol();

    char *nodeValue = treeNode->value;
    ParamInfo *existingFparam = lookupFparam(currentSymbol, nodeValue);
    
    if (existingFparam) {
        printf("Semantic Error: Duplicate parameter(s) used in Function '%s'\n\n", currentSymbol->identifier);
        printf("%d | (%s) \n\n", treeNode->line_no, nodeValue);
        printf("Checking semantics");
        fail();
        exit(1);
    }

    currentSymbol->paramList = realloc(currentSymbol->paramList, sizeof(ParamInfo*) * (currentSymbol->numParams + 1));

    currentSymbol->paramList[currentSymbol->numParams] = malloc(sizeof(ParamInfo));
    currentSymbol->paramList[currentSymbol->numParams]->identifier = strdup(nodeValue);

    TypeInfo *typeInfo = createTypeInfo(treeNode->op[0]->value);
    

    if(treeNode->nops >= 2) {
        
        typeInfo->isArray = 1;
        typeInfo->numDimensions = treeNode->op[1]->nops;
        typeInfo->dimensions = malloc(sizeof(int) * typeInfo->numDimensions);
        
        for (int i = 0; i < typeInfo->numDimensions; i++) {
            typeInfo->dimensions[i] = atoi(treeNode->op[1]->op[i]->value);
        }
    }
    
    currentSymbol->paramList[currentSymbol->numParams]->typeInfo = typeInfo;
    setSize(currentSymbol->paramList[currentSymbol->numParams]->typeInfo);
    
    currentSymbol->numParams++;

}

void handleAttributeNode(node *treeNode){

    Symbol *existingAttribute = lookupSymbol(getCurrentTable(), treeNode->value);

    if (existingAttribute) {
        printf("Semantic Error: Redeclaration of Attribute '%s'\n\n", treeNode->value);
        printf("%d | attribute %s\n", existingAttribute->line, treeNode->value);
        printf("%d | attribute %s\n\n", treeNode->line_no, treeNode->value);
        printf("Checking semantics");
        fail();
        exit(1);
    }
    
    Symbol *attributeSymbol = createSymbol();

    attributeSymbol->identifier = treeNode->value;
    attributeSymbol->symbolType = VARIABLE_SYMBOL_TYPE;

    attributeSymbol->visibility = (strcmp(treeNode->op[0]->value, "private") == 0) ? PRIVATE_VIS : PUBLIC_VIS;
    attributeSymbol->typeInfo = createTypeInfo(treeNode->op[1]->value);
    
    
    if(treeNode->nops > 2) {
        
        attributeSymbol->typeInfo->isArray = 1;
        attributeSymbol->typeInfo->numDimensions = treeNode->op[2]->nops;
        attributeSymbol->typeInfo->dimensions = malloc(sizeof(int) * attributeSymbol->typeInfo->numDimensions);
        
        for (int i = 0; i < attributeSymbol->typeInfo->numDimensions; i++) {
            attributeSymbol->typeInfo->dimensions[i] = atoi(treeNode->op[2]->op[i]->value);
        }
        
    }
    
    setSize(attributeSymbol->typeInfo);

    attributeSymbol->line = treeNode->line_no;
    attributeSymbol->col = treeNode->col_no;

    insertSymbol(getCurrentTable(), attributeSymbol);

}

void handleLocalVarNode(node *treeNode){

    Symbol *existingLocalVar = lookupSymbol(getCurrentTable(), treeNode->value);

    ParamInfo *paramInfo = lookupFparam(getCurrentTable()->parent, treeNode->value);

    if (existingLocalVar) {
        printf("Semantic Error: Redeclaration of Variable '%s'\n\n", treeNode->value);
        printf("%d | local %s \n", existingLocalVar->line, treeNode->value);
        printf("%d | local %s\n\n", treeNode->line_no, treeNode->value);
        printf("Checking semantics");
        fail();
        exit(1);
    }
    
    if (paramInfo) {
        printf("Semantic Error: Redeclaration of Variable '%s'\n\n", treeNode->value);
        printf("func param (%s) \n", treeNode->value);
        printf("local %s\n\n", treeNode->line_no, treeNode->value);
        printf("Checking semantics");
        fail();
        exit(1);
    }
    
    Symbol *localVarSymbol = createSymbol();

    localVarSymbol->identifier = treeNode->value;
    localVarSymbol->symbolType = VARIABLE_SYMBOL_TYPE;

    localVarSymbol->typeInfo = createTypeInfo(treeNode->op[0]->value);
    
    
    if(treeNode->nops > 1) {
        
        localVarSymbol->typeInfo->isArray = 1;
        localVarSymbol->typeInfo->numDimensions = treeNode->op[1]->nops;
        localVarSymbol->typeInfo->dimensions = malloc(sizeof(int) * localVarSymbol->typeInfo->numDimensions);
        
        for (int i = 0; i < localVarSymbol->typeInfo->numDimensions; i++) {
            localVarSymbol->typeInfo->dimensions[i] = atoi(treeNode->op[1]->op[i]->value);
        }
        
    }
    
    setSize(localVarSymbol->typeInfo);
    setOffset(localVarSymbol->typeInfo);
    //printf("( Size: %d, Offset: %d )", localVarSymbol->typeInfo->size, localVarSymbol->typeInfo->offset);

    localVarSymbol->line = treeNode->line_no;
    localVarSymbol->col = treeNode->col_no;

    insertSymbol(getCurrentTable(), localVarSymbol);

}

void handleImplNode(node * treeNode) {

    Symbol *existingImpl = lookupSymbol(globalTable, treeNode->value);

    if (existingImpl) {
        printf("Semantic Error: Redeclaration of Implementation '%s'\n\n", treeNode->value);
        printf("%d | Implementation %s\n", existingImpl->line, treeNode->value);
        printf("%d | Implementation %s\n\n", treeNode->line_no, treeNode->value);
        printf("Checking semantics");
        fail();
        exit(1);
    }
    
    Symbol *implSymbol = createSymbol();
    
    SymbolTable *implScopeTable = NULL;

    implSymbol->identifier = treeNode->value;
    implSymbol->symbolType = IMPL_SYMBOL_TYPE;
    implSymbol->line = treeNode->line_no;
    implSymbol->col = treeNode->col_no;
    
    if (treeNode->nops >= 1) implScopeTable = createSymbolTable(implSymbol);

    implSymbol->link = implScopeTable;
    
    if (implScopeTable) enterTableScope(implScopeTable);
    
    for (int i = 0; i < treeNode->nops; i++) traverseAST(treeNode->op[i]);
    
    if (implScopeTable) leaveCurrentTableScope();
    
    insertSymbol(globalTable, implSymbol);
}

void handleFuncNode(node* treeNode) {

     Symbol *existingFunc = lookupSymbol(getCurrentTable(), treeNode->value);

    if (existingFunc) { 
        printf("Semantic Error: Redeclaration of Function '%s'\n\n", treeNode->value);
        printf("%d | func %s\n", existingFunc->line, treeNode->value);
        printf("%d | func %s\n\n", treeNode->line_no, treeNode->value);
        printf("Checking semantics");
        fail();
        exit(1);
    }

    Symbol *funcSymbol = createSymbol();
    funcSymbol->identifier = treeNode->value;
    funcSymbol->symbolType = FUNCTION_SYMBOL_TYPE;
    funcSymbol->link = NULL;
    funcSymbol->line = treeNode->line_no;
    funcSymbol->col = treeNode->col_no;
    
    insertSymbol(getCurrentTable(), funcSymbol);

    if (treeNode->op[0]->type == TYPE_NODE){
        funcSymbol->typeInfo = createTypeInfo(treeNode->op[0]->value);
    } 
    
    else {
        funcSymbol->typeInfo = createTypeInfo(treeNode->op[1]->value);
        for (int i = 0; i < treeNode->op[0]->nops; i++) traverseAST(treeNode->op[0]->op[i]);
    }

    if ( !treeNode->op[1]->value){
        SymbolTable *funcScopeTable = createSymbolTable(funcSymbol);
        funcSymbol->link = funcScopeTable;
        enterTableScope(funcScopeTable);
        for (int i = 0; i < treeNode->op[1]->nops; i++) traverseAST(treeNode->op[1]->op[i]);
        leaveCurrentTableScope();
    }

    if ( treeNode->nops > 2 && !treeNode->op[2]->value){
        SymbolTable *funcScopeTable = createSymbolTable(funcSymbol);
        funcSymbol->link = funcScopeTable;
        enterTableScope(funcScopeTable);
        for (int i = 0; i < treeNode->op[2]->nops; i++) traverseAST(treeNode->op[2]->op[i]);
        leaveCurrentTableScope();
    }
}

void handleIfNode(node * treeNode) {

    Symbol *ifSymbol = createSymbol();
    
    SymbolTable *ifScopeTable = NULL;

    ifSymbol->identifier = "if_statement";
    ifSymbol->symbolType = IF_SYMBOL_TYPE;
    ifSymbol->line = treeNode->line_no;
    ifSymbol->col = treeNode->col_no;
    
    ifScopeTable = createSymbolTable(ifSymbol);
    ifSymbol->link = ifScopeTable;
    
    if (ifScopeTable) enterTableScope(ifScopeTable);
    for (int i = 0; i < treeNode->nops; i++) traverseAST(treeNode->op[i]);
    if (ifScopeTable) leaveCurrentTableScope();
    
}

void handleRelOpNode(node * treeNode) {

    for (int i = 0; i < treeNode->nops; i++){
        
        if (treeNode->op[i]->type == VARIABLECALL_NODE){

            Symbol *existingFunc = lookupSymbol(getParentTable(), treeNode->op[i]->value);

            if (!existingFunc) { 
                printf("Semantic Error: Undeclared variable (%s) in expression '%s'\n\n", treeNode->op[i]->value, treeNode->value);
                printf("Checking semantics");
                fail();
                exit(1);
            }
        }
    }  
}

void handleBlockNode(node * treeNode) {

    SymbolTable *blockTable = NULL;
    Symbol *blockSymbol = createSymbol();

    if (treeNode->type == THEN_NODE){
        blockSymbol->identifier = "then_block";
        blockSymbol->symbolType = THEN_BLOCK_TYPE;      
    }
    
    else {
        blockSymbol->identifier = "else_block";
        blockSymbol->symbolType = ELSE_BLOCK_TYPE;
    }
    

    if (treeNode->nops > 0) blockTable = createSymbolTable(blockSymbol);
    blockSymbol->link = blockTable;

    if (blockTable) enterTableScope(blockTable);
    for (int i = 0; i < treeNode->nops; i++) traverseAST(treeNode->op[i]);
    if (blockTable) leaveCurrentTableScope();

    //insertSymbol(getCurrentTable(), blockSymbol);
}

void handleArithExprNode(node * treeNode) {


    for (int i = 0; i < treeNode->nops; i++){
        
        if (treeNode->op[i]->type == VARIABLECALL_NODE){

            Symbol *variable = reccurlivelookupSymbol(treeNode->op[i]->value);

            if (!variable) { 
                printf("Semantic Error: Undeclared variable (%s) in expression '%s'\n\n", treeNode->op[i]->value, treeNode->value);
                printf("Checking semantics");
                fail();
                exit(1);
            }

        } 
        
        else if (treeNode->op[i]->type == ARITHEXPR_NODE){
            for (int i = 0; i < treeNode->op[i]->nops; i++) traverseAST(treeNode->op[i]);
        }
    }  
}

void handleAssignNode(node * treeNode) {

    for (int i = 0; i < treeNode->nops; i++){
        
        if (treeNode->op[i]->type == VARIABLECALL_NODE){

            Symbol *existingFunc = reccurlivelookupSymbol(treeNode->op[i]->value);

            if (!existingFunc) { 
                printf("Semantic Error: Undeclared variable (%s) in expression '%s'\n\n", treeNode->op[i]->value, treeNode->value);
                printf("Checking semantics");
                fail();
                exit(1);
            }
        } 
    }
}

void traverseAST(node *treeNode) {

    if (!treeNode) {
        fail();
        return;
    }

    switch(treeNode->type) {

        case CLASS_NODE: {
            handleClassNode(treeNode);
            break;
        }

        case INHERITANCE_NODE: {
            handleInheritanceNode(treeNode);
            break;
        }

        case FUNCDEF_NODE: {
            handleFuncDefNode(treeNode);
            break;
        }

        case FUNC_NODE: {
            handleFuncNode(treeNode);
            break;
        }

        case FPARAM_NODE: {
            handleFparamNode(treeNode);
            break;
        }

        case ATTRIBUTE_NODE: {
            handleAttributeNode(treeNode);
            break;
        }

        case IMPL_NODE: {
            handleImplNode(treeNode);
            break;
        }

        case LOCALVAR_NODE: {
            handleLocalVarNode(treeNode);
            break;
        }

        case IF_NODE: {
            handleIfNode(treeNode);
            break;
        }

        case WHILE_NODE: {
            handleIfNode(treeNode);
            break;
        }

        case RELOP_NODE: {
            handleRelOpNode(treeNode);
            break;
        }

        case THEN_NODE: {
            handleBlockNode(treeNode);
            break;
        }

        case ELSE_NODE: {
            handleBlockNode(treeNode);
            break;
        }

        case ASSIGN_NODE: {
            handleAssignNode(treeNode);
            break;
        }

        default: {
            for (int i = 0; i < treeNode->nops; i++) traverseAST(treeNode->op[i]);
            break;
        }
    }
}


//! Creation of Symbol tables

void generateSymbolTables() {

    globalTable = createSymbolTable(NULL);

    enterTableScope(globalTable);

    hbr();
    printf("Checking semantics");
    hbr();
    sp(2);

    traverseAST(semanticStack[0]);

    leaveCurrentTableScope();

    printf("\nSymbol Tables:\n\n");

   
    printTableTitle(0, 8, "Global Scope", "PROGRAMME");
    printHeader(0, 8, "Identifier", "symbol type", "data type", "is array", "dimensions", "size", "offset", "extra info");

    printSymbolTable(globalTable, 0);
    printSymbolTableToFile(globalTable, 0);
    
    hbr();
    printf("Checking semantics");
    done_nobr();
    hbr();
    sp(2);
}
