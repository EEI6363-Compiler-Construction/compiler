#ifndef AST_H
#define AST_H

#define MAX_SEMANTIC_STACK 100
#define MAX_NODE_TYPES 54

#include "../../lexical_analyzer/tokens.h"

extern int semTop;
extern int penTop;


typedef enum {
    CLASS_NODE = 0,
    IMPL_NODE,
    FUNC_NODE,
    FUNCBODY_NODE,
    FUNCDEF_NODE,
    ATTRIBUTE_NODE,
    ASSIGN_NODE,
    IF_NODE,
    WHILE_NODE,
    READ_NODE,
    WRITE_NODE,
    RETURN_NODE,
    BIN_OP_NODE,
    UN_OP_NODE,
    ID_NODE,
    LITERAL_NODE,
    TYPE_NODE,
    BLOCK_NODE,
    PARAM_NODE,
    MULTOP_NODE,
    FUNCTIONCALL_NODE,
    VARIABLECALL_NODE,
    INHERITANCE_NODE,
    EMPTY_NODE,
    PROGRAM_NODE,
    VISIBILITY_NODE,
    LEAF_NODE,
    CLASSBODY_NODE,
    FPARAMS_NODE,
    FPARAM_NODE,
    APARAMS_NODE,
    APARAM_NODE,
    ARRAY_NODE,
    ARRAYSIZE_NODE,
    CONSTRUCTORDEF_NODE,
    CONSTRUCTOR_NODE,
    CONSTRUCTORBODY_NODE,
    LOCALVAR_NODE,
    VARDECL_NODE,
    ATTRIBUTE_OR_LOCAL_NODE,
    VARIABLE_NODE,
    INDICELIST_NODE,
    ARITHEXPR_NODE,
    OPEREND_NODE,
    RIGHTOPR_NODE,
    RELOP_NODE,
    ASSIGNOP_NODE,
    STATBLOCK_NODE,
    THEN_NODE,
    ELSE_NODE,
    
} nodeType;

typedef struct node {
    nodeType type;
    char* value;                
    char* typeName;           
    int intValue;              
    float floatValue;         
    int nops;                  
    struct node** op;      
    int line_no;
    int col_no;
} node;

typedef struct {
    int symbol; 
    char* lexeme;  
} pendingNonTerminalSymbol;

typedef struct {
    NonTerminals nonTerminal;
    int symbolCount; 
    int currentSymbol;  
    pendingNonTerminalSymbol symbols[20];
} pendingNonTerminal;

extern char* nodeTypeStrings[];
extern node* semanticStack[];
extern pendingNonTerminal* pendingStack[];

void pushSemanticStack(node* node);
node* popSemanticStack();
void pushPendingStack(NonTerminals nonTerminal, int symbolCount);
pendingNonTerminal* popPendingStack();
node* operatorNode(nodeType type, int nops, ...);
node* literalIntNode(int value, int line_no, int col_no);
node* literalFloatNode(float value, int line_no, int col_no);
node* idNode(char* name, int line_no, int col_no);
node* visibilityNode(char* name);
node* createLeafNode(char* value, nodeType nodeType, int line_no, int col_no);
char* getNodeTypeString(int type);
void applySemanticActions(int nonterminal, int production);
void createASTNode(pendingNonTerminal *pendingNonTerminal);
void runSemanticAnalyzer();

#endif