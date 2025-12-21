#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "../lexical_analyzer/tokens.h"
#include "parse_table/parse_table.h"
#include "parse_table/rules.h"
#include "../utils/error_handle.h"
#include "../utils/token_strings.h"
#include "../utils/logger.h"
#include "parsing_stack/parsing_stack.h"
#include "ast/ast.h"

extern int yylex(void);
extern FILE *yyin;

TOKEN token;


void LookAhead() {
    yylex();
    token = getCurrentToken();
};

int readFile(char *argv) {

    s("Reading File");

    yyin = fopen(argv, "r");

    if (!yyin) {
        fail();
        e("Error opening file");
        return 0;
    } else {
        done();
        return 1;
    }
}

void handleSyntaxError(char* argv, char* previousLexeme) {

    fail();
    printf("No production(s) found." );
    for (int i = 0; token.lexeme[i] != '\0'; i++) col_no--;
    printf("\n\n");
    for (int i = 0; i < 200; i++) printf("=");
    printf("\n\nSyntax Error near \n\n%s %s\n", previousLexeme, token.lexeme);
    for (int i = 0; previousLexeme[i] != '\0'; i++) printf(" ");
    printf("^");
    for (int i = 0; token.lexeme[i] != '\0'; i++) printf("`");
    printf("\n\n");
    printf("%s line: %d, column: %d\n\n", argv, line_no, col_no);
    for (int i = 0; i < 200; i++) printf("=");
    printf("\n\n\n\n");
    printf("Parsing");
    fail();
    exit(1);
}

void printStack() {

    printf("\n\n");
    for (int i = 0; i < 200; i++) printf("=");
    printf("\n\nstack\n++++++++++++++++++++");
    
    for (int i = top - 1; i >= 0; i--) {
        if (parsingStack[i] >= 100) {
            printf("\n%s", getTerminalString(parsingStack[i]));
        } else {
            printf("\n%s", getNonTerminalString(parsingStack[i]));
        }

        if (i > 0) printf(", ");
    }

    printf(" \n++++++++++++++++++++\n");
}

void printLookAhead() {
    printf("\nlookahead: %s (%s)", token.lexeme, getTerminalString(token.terminal));
}

void printNodeToFile(FILE *file, node* n, int depth, const char* prefix, int isLast) {

    if (!n) return;

    fprintf(file, "%s", prefix);
    fprintf(file, "%s", (depth == 0) ? "" : (isLast ? "└──> " : "├──> "));

    if (n->type == LITERAL_NODE) {
        if (n->floatValue != 0.0f)
            fprintf(file, "%s (%.2f)\n", getNodeTypeString(n->type), n->floatValue);
        else
            fprintf(file, "%s (%d)\n", getNodeTypeString(n->type), n->intValue);
    } else {
        fprintf(file, "%s (%s)\n", getNodeTypeString(n->type),
                n->value ? n->value : "");
    }

    char newPrefix[256];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s",
             prefix,
             (depth == 0 ? "" : (isLast ? "\t\t" : "│\t\t")));
    fprintf(file, "%s\n", newPrefix);

    for (int i = 0; i < n->nops; i++) {
        printNodeToFile(file, n->op[i], depth + 1, newPrefix, i == n->nops - 1);
    }
}

void printASTToFile() {

    if (semTop <= 0) return;

    struct stat st = {0};
    if (stat("output", &st) == -1) {
        mkdir("output", 0700);
    }

    FILE *file = fopen("output/ast.txt", "w");
    if (!file) {
        perror("Failed to open output/ast.txt");
        return;
    }

    fprintf(file, "Abstract Syntax Tree\n\n");

    for (int i = semTop - 1; i >= 0; i--) {
        fprintf(file, "[%d]\n", i);
        printNodeToFile(file, semanticStack[i], 1, " ", 1);
        fprintf(file, "\n");
    }

    fclose(file);

    printf("\nAST written to output/ast.txt\n\n");
}

void printNode(node* n, int depth, const char* prefix, int isLast) {

    if (!n) return;

    printf("%s", prefix);
    printf("%s", (depth == 0) ? "" : (isLast ? "└──> " : "├──> "));

    if (n->type == LITERAL_NODE) {
        if (n->floatValue != 0.0f)
            printf("%s (%.2f)\n", getNodeTypeString(n->type), n->floatValue);
        else
            printf("%s (%d)\n", getNodeTypeString(n->type), n->intValue);
    } else {
        printf("%s (%s)\n", getNodeTypeString(n->type),
               n->value ? n->value : "");
               
            }
    char newPrefix[256];
    snprintf(newPrefix, sizeof(newPrefix), "%s%s",
             prefix,
             (depth == 0 ? "" : (isLast ? "\t\t" : "│\t\t")));
    printf("%s", newPrefix);
    printf("\n");

    for (int i = 0; i < n->nops; i++) {
        printNode(n->op[i], depth + 1, newPrefix, i == n->nops - 1);
    }
}

void printAST() {
    if (semTop <= 0) return;

    printf("\n\nAbstract Syntax Tree:(%d)\n\n", semTop);

    for (int i = semTop - 1; i >= 0; i--) {
        printf("[%d]\n", i);
        printNode(semanticStack[i], 1, " ", 1);
        printf("\n");
    }
}

void printPendingStack() {
    
    if (penTop <= 0) {
        return;
    }
    
    printf("\n\nPending stack (%d):\n\n", penTop);

    for (int i = penTop - 1; i >= 0; i--) {
        if (pendingStack[i] == NULL) {
            printf("[%d] -> <NULL pointer>\n", i);
        } else {
            printf("\n[%d] -> %s (symbolCount=%d) (currentSymbol=%d)", i, getNonTerminalString(pendingStack[i]->nonTerminal), pendingStack[i]->symbolCount, pendingStack[i]->currentSymbol);
            printf("=> [");
            for (int j = 0; j < pendingStack[i]->currentSymbol; j++){
                int currentSymbol = pendingStack[i]->symbols[j].symbol;
                if (currentSymbol == EMPTY) {
                     printf(" EMPTY ");
                } else if (currentSymbol >= 100) {
                    printf(" %s ", getTerminalString(currentSymbol));
                } else {
                    printf(" %s ", getNonTerminalString(currentSymbol));
                }
            }
            printf("]");
        }
    }
    printf("\n");
}

void reducePendingStack(int symbol, char* lexeme) {

    if (penTop <= 0) {
        printf("\n\nAST generation finished (Reduse semantic stack complete)");
        return;
    }
    
    pendingNonTerminal *topPending = pendingStack[penTop - 1];
    
    if (symbol != 999 ) {
        pendingNonTerminalSymbol newSymbol = { symbol, lexeme } ;
        topPending->symbols[topPending->currentSymbol] = newSymbol;
    }
    
    
    printf("\n\nincrease the %s.currentSymbol value\n", getNonTerminalString(topPending->nonTerminal));
     printf("\n+-----------------------------------------+");
    printf("\n| pre. value | curr. value | symbol count |\n");
    printf("+-----------------------------------------+\n");
    printf("|%7d     |", topPending->currentSymbol);
    topPending->currentSymbol++;
    printf("%7d      |", topPending->currentSymbol);
    printf("%7d       |", topPending->symbolCount);
    printf("\n+-----------------------------------------+\n\n");

    if (topPending->currentSymbol ==  topPending->symbolCount){
        printf("curr. value = symbol count =>  Create a New Node for: \n%s ", getNonTerminalString(topPending->nonTerminal));

        printf("=> [");
            for (int j = 0; j < topPending->currentSymbol; j++){

                int currentSymbol = topPending->symbols[j].symbol;

                if (currentSymbol == EMPTY) {
                     printf(" EMPTY ");

                } else if (currentSymbol >= 100) {
                    printf(" %s ", getTerminalString(currentSymbol));
                } else {
                    printf(" %s ", getNonTerminalString(currentSymbol));
                }
            }
        printf("]");

        createASTNode(topPending);
        //printAST();

        popPendingStack();
        reducePendingStack(topPending->nonTerminal, NULL);
    }
}

int runParser(char *argv) {

    p("Parser Started...");

    if (!readFile(argv)) { return 0;}

    initParseTable();
    populateParseTable();

    push(END_OF_INPUT);
    push(PROG);

    LookAhead();

    char* previousLexeme = "";

    while (top > 0){

        printStack();
        printLookAhead();

        int production;
        int topSymbol = parsingStack[top - 1];

        //! Parse finished
        if (topSymbol ==  END_OF_INPUT) {

            if (token.terminal ==  END_OF_INPUT) {
                printf("\n\n\n\n");
                for (int i = 0; i < 200; i++) printf("=");
                printf("\n");
                printf("Parsing complete...");
                done();
                for (int i = 0; i < 200; i++) printf("=");
                printAST();
                printASTToFile();
            } else {
                handleSyntaxError(argv, previousLexeme);
            }

            break;
        }

        //! Terminal match
        else if (topSymbol >= 100) {

            if (topSymbol == token.terminal) {

                printf("\n\nTerminals match");
                printf("\nRemove %s from the stack \n", getTerminalString(token.terminal));
                pop();

                previousLexeme = token.lexeme;

                switch (topSymbol) {

                    case INT_LIT:
                        pushSemanticStack(literalIntNode(atoi(token.lexeme), line_no, col_no));
                        break;
                    case FLOAT_LIT:
                        pushSemanticStack(literalFloatNode(atof(token.lexeme), line_no, col_no));
                        break;
                    case ID:
                        pushSemanticStack(idNode(token.lexeme, line_no, col_no));
                        break;
                    case PRIVATE:
                        pushSemanticStack(createLeafNode(token.lexeme, VISIBILITY_NODE, line_no, col_no));
                        break;
                    case PUBLIC:
                        pushSemanticStack(createLeafNode(token.lexeme, VISIBILITY_NODE, line_no, col_no));
                        break;
                    case LOCAL:
                        pushSemanticStack(createLeafNode(token.lexeme, ATTRIBUTE_OR_LOCAL_NODE, line_no, col_no));
                        break;
                    case ATTRIBUTE:
                        pushSemanticStack(createLeafNode(token.lexeme, ATTRIBUTE_OR_LOCAL_NODE, line_no, col_no));
                        break;
                    case INTEGER:
                        pushSemanticStack(createLeafNode(token.lexeme, TYPE_NODE, line_no, col_no));
                        break;
                    case FLOAT:
                        pushSemanticStack(createLeafNode(token.lexeme, TYPE_NODE, line_no, col_no));
                        break;
                    case VOID:
                        pushSemanticStack(createLeafNode(token.lexeme, TYPE_NODE, line_no, col_no));
                        break;
                    case ADD:
                        pushSemanticStack(createLeafNode(token.lexeme, ARITHEXPR_NODE, line_no, col_no));
                        break;
                    case SUBTRACT:
                        pushSemanticStack(createLeafNode(token.lexeme, ARITHEXPR_NODE, line_no, col_no));
                        break;
                    case OR:
                        pushSemanticStack(createLeafNode(token.lexeme, ARITHEXPR_NODE, line_no, col_no));
                        break;
                    case MULTIPLY:
                        pushSemanticStack(createLeafNode(token.lexeme, ARITHEXPR_NODE, line_no, col_no));
                        break;
                    case DIVISION:
                        pushSemanticStack(createLeafNode(token.lexeme, ARITHEXPR_NODE, line_no, col_no));
                        break;
                    case AND:
                        pushSemanticStack(createLeafNode(token.lexeme, ARITHEXPR_NODE, line_no, col_no));
                        break;
                    case DOUBLE_EQ:
                        pushSemanticStack(createLeafNode(token.lexeme, RELOP_NODE, line_no, col_no));
                        break;
                    case NOT_EQ:
                        pushSemanticStack(createLeafNode(token.lexeme, RELOP_NODE, line_no, col_no));
                        break;
                    case L_THAN:
                        pushSemanticStack(createLeafNode(token.lexeme, RELOP_NODE, line_no, col_no));
                        break;
                    case G_THAN:
                        pushSemanticStack(createLeafNode(token.lexeme, RELOP_NODE, line_no, col_no));
                        break;
                    case L_THAN_EQ:
                        pushSemanticStack(createLeafNode(token.lexeme, RELOP_NODE, line_no, col_no));
                        break;
                    case G_THAN_EQ:
                        pushSemanticStack(createLeafNode(token.lexeme, RELOP_NODE, line_no, col_no));
                        break;
                    default:
                        break;
                } 

                reducePendingStack(topSymbol, token.lexeme);
                LookAhead();

            } else {

                handleSyntaxError(argv, previousLexeme);
                break;
            }

        } 
        
        //! Non-terminal match
        else if (topSymbol < 100) {

            production = parseTable[topSymbol][token.terminal - 100];
            printf("\n\nRetrieving production for : [ %s, %s ]",getNonTerminalString(topSymbol), getTerminalString(token.terminal) );

            if (production ==  -1){
                handleSyntaxError(argv, previousLexeme);
                break;
            }
    
            done();

            Production currentProduction = rules[topSymbol].rhs.productions[production];
            printf("deriving %s with : %s", getNonTerminalString(topSymbol) ,currentProduction.grammar );
            pop();

            if(currentProduction.productionSymbols[0] != EMPTY) {
                pushPendingStack(topSymbol, currentProduction.productionSymbolCount);
            }
           

            for (int i = currentProduction.productionSymbolCount - 1; i >= 0; i--) {
                int productionSymbol = currentProduction.productionSymbols[i];

                if (productionSymbol != EMPTY) {
                    push(productionSymbol);

                }else {
                    reducePendingStack(EMPTY, "empty");
    
                }
            }
        }
    
        /* printAST(); */
        printPendingStack();
    
    }
}

