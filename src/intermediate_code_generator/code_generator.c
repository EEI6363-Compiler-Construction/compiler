#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../syntax_analyzer/ast/ast.h"
#include "../symbol_table/symbol_table.h"
#include "../utils/logger.h"


void generateCode(node* astNode);
void generateExpression(node* expressionNode);
void generateBinaryArithmeticExpr(const node* exprNode);
void generateAssignment(node* assignNode);
void generateWriteStatement(node* writeNode);
Symbol* findSymbolInTableRecursive(SymbolTable* table, const char* identifier);
void generateCode(node* abstractSyntaxTree);


#define HEX_START 0x80
#define WRITE_LOCATION "0x95"
#define TEMP_LOCATION "0x90"

char* intToHexString(int offset)
{
    unsigned int value = HEX_START + offset;

    char* result = malloc(6);

    if (!result) return NULL;

    sprintf(result, "0x%02X", value);

    return result;
}


Symbol* findSymbolInTableRecursive(SymbolTable* table, const char* identifier) {

    if (!table || !identifier) {
        return NULL;
    }

    for (int index = 0; index < table->symbolCount; index++) {

        Symbol* symbol = table->symbols[index];
        if (!symbol) {
            continue;
        }

        if (strcmp(symbol->identifier, identifier) == 0) {
            return symbol;
        }

        if (symbol->link) {
            Symbol* result = findSymbolInTableRecursive(symbol->link, identifier);
            if (result) {
                return result;
            }
        }
    }

    return NULL;
}

Symbol* findSymbol(const char* identifier) {
    return findSymbolInTableRecursive(globalTable, identifier);
}



void generateLiteral(const node* literalNode) {
    printf("loadacc #%d\n", literalNode->intValue);
}

void generateVariable(const node* variableNode) {
    Symbol *symbol = findSymbol(variableNode->value);
    printf("loadacc %s\n", intToHexString(symbol->typeInfo->offset));
}

void generateBinaryArithmeticExpr(const node* exprNode) {

    node* leftOperand  = exprNode->op[0];
    node* rightOperand = exprNode->op[1];


    if (leftOperand->type == ARITHEXPR_NODE) {
        generateBinaryArithmeticExpr(leftOperand);
    }
    else if (leftOperand->type == LITERAL_NODE) {
        printf("loadacc #%s\n", leftOperand->value);
    }
    else if (leftOperand->type == VARIABLECALL_NODE || leftOperand->type == ID_NODE){

        Symbol *symbol = findSymbol(leftOperand->value);
        printf("loadacc %s\n", intToHexString(symbol->typeInfo->offset));
    }


    if (rightOperand->type == ARITHEXPR_NODE) {

        printf("storeacc %s\n", TEMP_LOCATION);

        generateBinaryArithmeticExpr(rightOperand);

        if (strcmp(exprNode->value, "+") == 0)
            printf("add %s\n", TEMP_LOCATION);
        else if (strcmp(exprNode->value, "-") == 0)
            printf("sub %s\n", TEMP_LOCATION);
        else if (strcmp(exprNode->value, "*") == 0)
            printf("mul %s\n", TEMP_LOCATION);
        else if (strcmp(exprNode->value, "/") == 0)
            printf("div %s\n", TEMP_LOCATION);

        return;
    }


    if (rightOperand->type == LITERAL_NODE) {

        if (strcmp(exprNode->value, "+") == 0)
            printf("add #%d\n", rightOperand->intValue);
        else if (strcmp(exprNode->value, "-") == 0)
            printf("sub #%d\n", rightOperand->intValue);
        else if (strcmp(exprNode->value, "*") == 0)
            printf("mul #%d\n", rightOperand->intValue);
        else if (strcmp(exprNode->value, "/") == 0)
            printf("div #%d\n", rightOperand->intValue);

        return;
    }

    if (rightOperand->type == VARIABLECALL_NODE || rightOperand->type == ID_NODE) {

        Symbol *symbol = findSymbol(rightOperand->value);

        if (strcmp(exprNode->value, "+") == 0) {
            printf("add %s\n", intToHexString(symbol->typeInfo->offset));
        }
        else if (strcmp(exprNode->value, "-") == 0) {
            printf("sub %s\n", intToHexString(symbol->typeInfo->offset));
        }
        else if (strcmp(exprNode->value, "*") == 0) {
            printf("mul %s\n", intToHexString(symbol->typeInfo->offset));
        }
        else if (strcmp(exprNode->value, "/") == 0) {
            printf("div %s\n", intToHexString(symbol->typeInfo->offset));
        }

        return;
    }


    if (strcmp(exprNode->value, "+") == 0)
        printf("add %s\n", rightOperand->value);
    else if (strcmp(exprNode->value, "-") == 0)
        printf("sub %s\n", rightOperand->value);
    else if (strcmp(exprNode->value, "*") == 0)
        printf("mul %s\n", rightOperand->value);
    else if (strcmp(exprNode->value, "/") == 0)
        printf("div %s\n", rightOperand->value);
}

void generateExpression(node* expressionNode) {

    if (!expressionNode) {
        return;
    }

    switch(expressionNode->type) {

        case LITERAL_NODE:
            generateLiteral(expressionNode);
            break;

        case VARIABLECALL_NODE:{break;}
        case ID_NODE:
            generateVariable(expressionNode);
            break;

        case ARITHEXPR_NODE:
            generateBinaryArithmeticExpr(expressionNode);
            break;

        default:
            for (int i = 0; i < expressionNode->nops; i++) {
                generateCode(expressionNode->op[i]);
            }
            break;
    }
}

void generateAssignment(node* assignNode) {

    node* leftNode  = assignNode->op[0];
    node* rightNode = assignNode->op[1];

    generateExpression(rightNode);

    Symbol *symbol = findSymbol(leftNode->value);

    printf("storeacc %s\n", intToHexString(symbol->typeInfo->offset));
}

void generateWriteStatement(node* writeNode) {

    node* argument = writeNode->op[0];
    printf("storeacc %s\n", WRITE_LOCATION);
}

void generateRelop(node* relopNode){

    node *leftNode = relopNode->op[0];
    node *rightNode = relopNode->op[1];

    Symbol *leftSymbol = findSymbol(leftNode->value);
    Symbol *rightSymbol = findSymbol(rightNode->value);

    
    if (strcmp(relopNode->value, ">") == 0){
        printf("loadacc %s\n", intToHexString(leftSymbol->typeInfo->offset));
        printf("sub %s\n", intToHexString(rightSymbol->typeInfo->offset));
        printf("loadacc #9\n");
        printf("js\n");
    } else if (strcmp(relopNode->value, "<>") == 0) {
        printf("loadacc %s\n", intToHexString(leftSymbol->typeInfo->offset));
        printf("sub %s\n", intToHexString(rightSymbol->typeInfo->offset));
        printf("loadacc #9\n");
        printf("jz\n");
    }
}



void generateCode(node* astNode) {

    if (!astNode) {
        return;
    }

    switch(astNode->type) {

        case ASSIGN_NODE: {
            generateAssignment(astNode);
            break;
        }

        case WRITE_NODE:{
            generateWriteStatement(astNode);
            break;
        }

        case ARITHEXPR_NODE:
        case VARIABLECALL_NODE:
        case ID_NODE:
        case LITERAL_NODE:{
            generateExpression(astNode);
            break;
        }

        case RELOP_NODE: {
            generateRelop(astNode);
            break;
        }
        case ELSE_NODE: {
            printf("jump #6\n");
        }

        case FUNCBODY_NODE:
        case BLOCK_NODE:
        case PROGRAM_NODE:
        default: {
            for (int childIndex = 0; childIndex < astNode->nops; childIndex++) {
                generateCode(astNode->op[childIndex]);
            }
            break;
        }
    }
}


void runCodeGenerator(void) {

    hbr();
    printf("Code Generation");
    hbr();
    sp(1);
    generateCode(astRoot);
    hbr();
    printf("Code Generation Complete [done]");
    hbr();
    sp(2);
}

