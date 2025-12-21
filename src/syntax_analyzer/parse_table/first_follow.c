#include <stdio.h> 
#include "../../lexical_analyzer/tokens.h"
#include "first_follow.h"
#include "../../utils/token_strings.h"
#include "parse_table.h"



//! First sets
// FIRST Sets
int prog_FirstSet[] =                           { 5, CLASS, IMPLEMENT, FUNC, CONSTRUCT, EMPTY };
int classOrImplOrFuncList_FirstSet[] =          { 5, CLASS, IMPLEMENT, FUNC, CONSTRUCT, EMPTY };
int classOrImplOrFunc_FirstSet[] =              { 4, CLASS, IMPLEMENT, FUNC, CONSTRUCT };
int classDecl_FirstSet[] =                      { 1, CLASS };
int inheritanceOptional_FirstSet[] =            { 2, ISA, EMPTY };
int inheritanceList_FirstSet[] =                { 2, COMMA, EMPTY };
int visibilityMemberDeclList_FirstSet[] =       { 3, PRIVATE, PUBLIC, EMPTY };
int implDef_FirstSet[] =                        { 1, IMPLEMENT };
int funcDefList_FirstSet[] =                    { 3, FUNC, CONSTRUCT, EMPTY };
int funcDef_FirstSet[] =                        { 2, FUNC, CONSTRUCT };
int visibility_FirstSet[] =                     { 2, PRIVATE, PUBLIC };
int memberDecl_FirstSet[] =                     { 3, FUNC, CONSTRUCT, ATTRIBUTE };
int funcDecl_FirstSet[] =                       { 2, FUNC, CONSTRUCT };
int funcHead_FirstSet[] =                       { 2, FUNC, CONSTRUCT };
int funcBody_FirstSet[] =                       { 1, L_CURLY };
int varDeclOrStmtList_FirstSet[] =              { 8, LOCAL, ID, IF, WHILE, READ, WRITE, RETURN, EMPTY };
int varDeclOrStmt_FirstSet[] =                  { 7, LOCAL, ID, IF, WHILE, READ, WRITE, RETURN };
int attributeDecl_FirstSet[] =                  { 1, ATTRIBUTE };
int localVarDecl_FirstSet[] =                   { 1, LOCAL };
int varDecl_FirstSet[] =                        { 1, ID };
int statement_FirstSet[] =                      { 6, ID, IF, WHILE, READ, WRITE, RETURN };
int assignStatOrFunctionCall_FirstSet[] =       { 1, ID };
int assignStatOrFunctionCallCont_FirstSet[] =   { 3, L_PARAN, L_BRACKET, EMPTY };
int statBlock_FirstSet[] =                      { 8, L_CURLY, ID, IF, WHILE, READ, WRITE, RETURN, EMPTY };
int statementList_FirstSet[] =                  { 7, ID, IF, WHILE, READ, WRITE, RETURN, EMPTY };
int expr_FirstSet[] =                           { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int exprCont_FirstSet[] =                       { 7, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, EMPTY };
int relExpr_FirstSet[] =                        { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int arithExpr_FirstSet[] =                      { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int arithOperations_FirstSet[] =                { 4, ADD, SUBTRACT, OR, EMPTY };
int sign_FirstSet[] =                           { 2, ADD, SUBTRACT };
int term_FirstSet[] =                           { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int termOperations_FirstSet[] =                 { 4, MULTIPLY, DIVISION, AND, EMPTY };
int factor_FirstSet[] =                         { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int variableOrFunctionCall_FirstSet[] =         { 1, ID };
int variable_FirstSet[] =                       { 1, ID };
int variableOrFunctionCallCont_FirstSet[] =     { 3, L_BRACKET, L_PARAN, EMPTY };
int indiceList_FirstSet[] =                     { 2, L_BRACKET, EMPTY };
int indice_FirstSet[] =                         { 1, L_BRACKET };
int arraySizeCont_FirstSet[] =                  { 2, INT_LIT, R_BRACKET };
int arraySize_FirstSet[] =                      { 1, L_BRACKET };
int type_FirstSet[] =                           { 3, INTEGER, FLOAT, ID };
int returnType_FirstSet[] =                     { 4, INTEGER, FLOAT, ID, VOID };
int fParamsTailList_FirstSet[] =                { 2, COMMA, EMPTY };
int fParams_FirstSet[] =                        { 2, ID, EMPTY };
int aParamsTailList_FirstSet[] =                { 2, COMMA, EMPTY };
int aParams_FirstSet[] =                        { 8, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT, EMPTY };
int arraySizeList_FirstSet[] =                  { 2, L_BRACKET, EMPTY };
int fParamsTail_FirstSet[] =                    { 1, COMMA };
int aParamsTail_FirstSet[] =                    { 1, COMMA };
int assignOp_FirstSet[] =                       { 1, ASSIGN };
int relOp_FirstSet[] =                          { 6, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ };
int addOp_FirstSet[] =                          { 3, ADD, SUBTRACT, OR };
int multOp_FirstSet[] =                         { 3, MULTIPLY, DIVISION, AND };



// FOLLOW Sets
int prog_FollowSet[] =                          { 1, END_OF_INPUT };
int classOrImplOrFuncList_FollowSet[] =         { 1, END_OF_INPUT };
int classOrImplOrFunc_FollowSet[] =             { 5, CLASS, IMPLEMENT, FUNC, CONSTRUCT, END_OF_INPUT };
int classDecl_FollowSet[] =                     { 5, CLASS, IMPLEMENT, FUNC, CONSTRUCT, END_OF_INPUT };
int inheritanceOptional_FollowSet[] =           { 1, L_CURLY };
int inheritanceList_FollowSet[] =               { 1, L_CURLY };
int visibilityMemberDeclList_FollowSet[] =      { 1, R_CURLY };
int implDef_FollowSet[] =                       { 5, CLASS, IMPLEMENT, FUNC, CONSTRUCT, END_OF_INPUT };
int funcDefList_FollowSet[] =                   { 1, R_CURLY };
int funcDef_FollowSet[] =                       { 6, CLASS, IMPLEMENT, FUNC, CONSTRUCT, R_CURLY, END_OF_INPUT };
int visibility_FollowSet[] =                    { 3, FUNC, CONSTRUCT, ATTRIBUTE };
int memberDecl_FollowSet[] =                    { 3, PRIVATE, PUBLIC, R_CURLY };
int funcDecl_FollowSet[] =                      { 3, PRIVATE, PUBLIC, R_CURLY };
int funcHead_FollowSet[] =                      { 2, SEMI_COL, L_CURLY };
int funcBody_FollowSet[] =                      { 6, CLASS, IMPLEMENT, FUNC, CONSTRUCT, R_CURLY, END_OF_INPUT };
int varDeclOrStmtList_FollowSet[] =             { 1, R_CURLY };
int varDeclOrStmt_FollowSet[] =                 { 8, LOCAL, ID, IF, WHILE, READ, WRITE, RETURN, R_CURLY };
int attributeDecl_FollowSet[] =                 { 3, PRIVATE, PUBLIC, R_CURLY };
int localVarDecl_FollowSet[] =                  { 8, LOCAL, ID, IF, WHILE, READ, WRITE, RETURN, R_CURLY };
int varDecl_FollowSet[] =                       { 10, LOCAL, ID, IF, WHILE, READ, WRITE, RETURN, R_CURLY, PRIVATE, PUBLIC };
int statement_FollowSet[] =                     { 10, LOCAL, ID, IF, WHILE, READ, WRITE, RETURN, R_CURLY, ELSE, SEMI_COL };
int assignStatOrFunctionCall_FollowSet[] =      { 1, SEMI_COL };
int assignStatOrFunctionCallCont_FollowSet[] =  { 1, SEMI_COL };
int statBlock_FollowSet[] =                     { 2, ELSE, SEMI_COL};
int statementList_FollowSet[] =                 { 1, R_CURLY };
int expr_FollowSet[] =                          { 3, R_PARAN, SEMI_COL, COMMA };
int exprCont_FollowSet[] =                      { 3, R_PARAN, SEMI_COL, COMMA };
int relExpr_FollowSet[] =                       { 1, R_PARAN };
int arithExpr_FollowSet[] =                     { 10, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET };
int arithOperations_FollowSet[] =               { 10, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET };
int sign_FollowSet[] =                          { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int term_FollowSet[] =                          { 13, ADD, SUBTRACT, OR, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET };
int termOperations_FollowSet[] =                { 13, ADD, SUBTRACT, OR, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET };
int factor_FollowSet[] =                        { 16, MULTIPLY, DIVISION, AND, ADD, SUBTRACT, OR, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET };
int variableOrFunctionCall_FollowSet[] =        { 16, MULTIPLY, DIVISION, AND, ADD, SUBTRACT, OR, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET };
int variable_FollowSet[] =                      { 1, R_PARAN };
int variableOrFunctionCallCont_FollowSet[] =    { 16, MULTIPLY, DIVISION, AND, ADD, SUBTRACT, OR, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET };
int indiceList_FollowSet[] =                    { 17, MULTIPLY, DIVISION, AND, ADD, SUBTRACT, OR, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET, ASSIGN };
int indice_FollowSet[] =                        { 17, L_BRACKET, MULTIPLY, DIVISION, AND, ADD, SUBTRACT, OR, DOUBLE_EQ, NOT_EQ, L_THAN, G_THAN, L_THAN_EQ, G_THAN_EQ, R_PARAN, SEMI_COL, COMMA, R_BRACKET };
int arraySizeCont_FollowSet[] =                 { 4, L_BRACKET, COMMA, SEMI_COL, R_PARAN };
int arraySize_FollowSet[] =                     { 4, L_BRACKET, COMMA, SEMI_COL, R_PARAN };
int type_FollowSet[] =                          { 5, L_BRACKET, COMMA, SEMI_COL, L_CURLY, R_PARAN };
int returnType_FollowSet[] =                    { 2, SEMI_COL, L_CURLY };
int fParamsTailList_FollowSet[] =               { 1, R_PARAN };
int fParams_FollowSet[] =                       { 1, R_PARAN };
int aParamsTailList_FollowSet[] =               { 1, R_PARAN };
int aParams_FollowSet[] =                       { 1, R_PARAN };
int arraySizeList_FollowSet[] =                 { 3, COMMA, SEMI_COL, R_PARAN };
int fParamsTail_FollowSet[] =                   { 2, COMMA, R_PARAN };
int aParamsTail_FollowSet[] =                   { 2, COMMA, R_PARAN };
int assignOp_FollowSet[] =                      { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int relOp_FollowSet[] =                         { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int addOp_FollowSet[] =                         { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };
int multOp_FollowSet[] =                        { 7, ID, INT_LIT, FLOAT_LIT, L_PARAN, NOT, ADD, SUBTRACT };

int *firstSets[MAX_NONTERMINALS] = {
    [PROG] =                            prog_FirstSet,
    [CLASSORIMPORFUNC_LIST] =           classOrImplOrFuncList_FirstSet,
    [CLASSORIMPORFUNC] =                classOrImplOrFunc_FirstSet,
    [CLASSDECL] =                       classDecl_FirstSet,
    [IMPLDEF] =                         implDef_FirstSet,
    [FUNCDEF] =                         funcDef_FirstSet,
    [INHERITANCE_OPTIONAL] =            inheritanceOptional_FirstSet,
    [INHERITANCE_LIST] =                inheritanceList_FirstSet,
    [VISIBILITYMEMBERDECL_LIST] =       visibilityMemberDeclList_FirstSet,
    [IMPLDEF] =                         implDef_FirstSet,
    [FUNCDEF_LIST] =                    funcDefList_FirstSet,
    [FUNCDEF] =                         funcDef_FirstSet,
    [VISIBILITY] =                      visibility_FirstSet,
    [MEMBERDECL] =                      memberDecl_FirstSet,
    [FUNCHEAD] =                        funcHead_FirstSet,
    [FUNCDECL] =                        funcDecl_FirstSet,
    [FUNCBODY] =                        funcBody_FirstSet,
    [VARDECLORSTMT_LIST] =              varDeclOrStmtList_FirstSet,
    [VARDECLORSTMT] =                   varDeclOrStmt_FirstSet,
    [ATTRIBUTEDECL] =                   attributeDecl_FirstSet,
    [LOCALVARDECL] =                    localVarDecl_FirstSet,
    [VARDECL] =                         varDecl_FirstSet,
    [STATEMENT] =                       statement_FirstSet,
    [ASSIGNSTATORFUNCCALL] =            assignStatOrFunctionCall_FirstSet,
    [ASSIGNSTATORFUNCCALL_CONT] =       assignStatOrFunctionCallCont_FirstSet,
    [STATBLOCK] =                       statBlock_FirstSet,
    [STATEMENT_LIST] =                  statementList_FirstSet,
    [EXPR] =                            expr_FirstSet,
    [EXPRCONT] =                        exprCont_FirstSet,
    [RELEXPR] =                         relExpr_FirstSet,
    [ARITHEXPR] =                       arithExpr_FirstSet,
    [ARITHOPERATIONS] =                 arithOperations_FirstSet,
    [SIGN] =                            sign_FirstSet,
    [TERM] =                            term_FirstSet,
    [TERMOPERATIONS] =                  termOperations_FirstSet,
    [FACTOR] =                          factor_FirstSet,
    [VARIABLEORFUNCCALL] =              variableOrFunctionCall_FirstSet,
    [VARIABLE] =                        variable_FirstSet,
    [VARIABLEORFUNCCALL_CONT] =         variableOrFunctionCallCont_FirstSet,
    [INDICE_LIST] =                     indiceList_FirstSet,
    [INDICE] =                          indice_FirstSet,
    [ARRAYSIZE_CONT] =                  arraySizeCont_FirstSet,
    [ARRAYSIZE] =                       arraySize_FirstSet,
    [TYPE] =                            type_FirstSet,
    [RETURN_TYPE] =                     returnType_FirstSet,
    [FPARAMSTAIL_LIST] =                fParamsTailList_FirstSet,
    [FPARAMS] =                         fParams_FirstSet,
    [APARAMSTAIL_LIST] =                aParamsTailList_FirstSet,
    [APARAMS] =                         aParams_FirstSet,
    [ARRAYSIZE_LIST] =                  arraySizeList_FirstSet,
    [FPARAMSTAIL] =                     fParamsTail_FirstSet,
    [APARAMSTAIL] =                     aParamsTail_FirstSet,
    [ASSIGNOP] =                        assignOp_FirstSet,
    [RELOP] =                           relOp_FirstSet,
    [ADDOP] =                           addOp_FirstSet,
    [MULTOP] =                          multOp_FirstSet,
};

int *followSets[MAX_NONTERMINALS] = {
    [PROG] =                            prog_FollowSet,
    [CLASSORIMPORFUNC_LIST] =           classOrImplOrFuncList_FollowSet,
    [CLASSORIMPORFUNC] =                classOrImplOrFunc_FollowSet,
    [CLASSDECL] =                       classDecl_FollowSet,
    [IMPLDEF] =                         implDef_FollowSet,
    [FUNCDEF] =                         funcDef_FollowSet,
    [INHERITANCE_OPTIONAL] =            inheritanceOptional_FollowSet,
    [INHERITANCE_LIST] =                inheritanceList_FollowSet,
    [VISIBILITYMEMBERDECL_LIST] =       visibilityMemberDeclList_FollowSet,
    [IMPLDEF] =                         implDef_FollowSet,
    [FUNCDEF_LIST] =                    funcDefList_FollowSet,
    [FUNCDEF] =                         funcDef_FollowSet,
    [VISIBILITY] =                      visibility_FollowSet,
    [MEMBERDECL] =                      memberDecl_FollowSet,
    [FUNCHEAD] =                        funcHead_FollowSet,
    [FUNCDECL] =                        funcDecl_FollowSet,
    [FUNCBODY] =                        funcBody_FollowSet,
    [VARDECLORSTMT_LIST] =              varDeclOrStmtList_FollowSet,
    [VARDECLORSTMT] =                   varDeclOrStmt_FollowSet,
    [ATTRIBUTEDECL] =                   attributeDecl_FollowSet,
    [LOCALVARDECL] =                    localVarDecl_FollowSet,
    [VARDECL] =                         varDecl_FollowSet,
    [STATEMENT] =                       statement_FollowSet,
    [ASSIGNSTATORFUNCCALL] =            assignStatOrFunctionCall_FollowSet,
    [ASSIGNSTATORFUNCCALL_CONT] =       assignStatOrFunctionCallCont_FollowSet,
    [STATBLOCK] =                       statBlock_FollowSet,
    [STATEMENT_LIST] =                  statementList_FollowSet,
    [EXPR] =                            expr_FollowSet,
    [EXPRCONT] =                        exprCont_FollowSet,
    [RELEXPR] =                         relExpr_FollowSet,
    [ARITHEXPR] =                       arithExpr_FollowSet,
    [ARITHOPERATIONS] =                 arithOperations_FollowSet,
    [SIGN] =                            sign_FollowSet,
    [TERM] =                            term_FollowSet,
    [TERMOPERATIONS] =                  termOperations_FollowSet,
    [FACTOR] =                          factor_FollowSet,
    [VARIABLEORFUNCCALL] =              variableOrFunctionCall_FollowSet,
    [VARIABLE] =                        variable_FollowSet,
    [VARIABLEORFUNCCALL_CONT] =         variableOrFunctionCallCont_FollowSet,
    [INDICE_LIST] =                     indiceList_FollowSet,
    [INDICE] =                          indice_FollowSet,
    [ARRAYSIZE_CONT] =                  arraySizeCont_FollowSet,
    [ARRAYSIZE] =                       arraySize_FollowSet,
    [TYPE] =                            type_FollowSet,
    [RETURN_TYPE] =                     returnType_FollowSet,
    [FPARAMSTAIL_LIST] =                fParamsTailList_FollowSet,
    [FPARAMS] =                         fParams_FollowSet,
    [APARAMSTAIL_LIST] =                aParamsTailList_FollowSet,
    [APARAMS] =                         aParams_FollowSet,
    [ARRAYSIZE_LIST] =                  arraySizeList_FollowSet,
    [FPARAMSTAIL] =                     fParamsTail_FollowSet,
    [APARAMSTAIL] =                     aParamsTail_FollowSet,
    [ASSIGNOP] =                        assignOp_FollowSet,
    [RELOP] =                           relOp_FollowSet,
    [ADDOP] =                           addOp_FollowSet,
    [MULTOP] =                          multOp_FollowSet,
};

int *getFirstSet(int nonTerminal) {

    int *firstSet = firstSets[nonTerminal];

    if (!firstSet) {
        return NULL;
    } 

    return firstSet;
};

int *getFollowSet(int nonTerminal) {

    int *followSet = followSets[nonTerminal];

    if (!followSet) {
        return NULL;
    } 

    return followSet;
};