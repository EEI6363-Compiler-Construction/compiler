%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/error_handle.h"
#include "tokens.h"

int line_no = 1;
int col_no = 1;


void increment_line_no() {
    line_no++;
    col_no = 0;
};

void update_column(const char* text, int length) {
    for (int i = 0; i < length; i++) {
        if (text[i] == ' ')
            col_no++;
        else if (text[i] == '\t')
            col_no += 4;
    }
}

TOKEN currentToken = {0, NULL};

void setCurrentToken( Terminals type, const char* lexeme) {

    currentToken.terminal = type;
    currentToken.lexeme = strdup(lexeme);

    for (int i = 0; lexeme[i] != '\0'; i++) {
       col_no++;
    }


}


TOKEN getCurrentToken(void){
    return currentToken;
}

%}


%%

"("                                                                     { setCurrentToken(L_PARAN, yytext); return L_PARAN; }
")"                                                                     { setCurrentToken(R_PARAN, yytext); return R_PARAN; }
"{"                                                                     { setCurrentToken(L_CURLY, yytext); return L_CURLY; }
"}"                                                                     { setCurrentToken(R_CURLY, yytext); return R_CURLY; }
"["                                                                     { setCurrentToken(L_BRACKET, yytext); return L_BRACKET; }
"]"                                                                     { setCurrentToken(R_BRACKET, yytext); return R_BRACKET; }
";"                                                                     { setCurrentToken(SEMI_COL, yytext); return SEMI_COL; }
","                                                                     { setCurrentToken(COMMA, yytext); return COMMA; } 
":"                                                                     { setCurrentToken(COLON, yytext); return COLON; }
"=>"                                                                    { setCurrentToken(ARROW, yytext); return ARROW; }


"+"                                                                     { setCurrentToken( ADD, yytext); return ADD; }
"-"                                                                     { setCurrentToken( SUBTRACT, yytext); return SUBTRACT; }
"*"                                                                     { setCurrentToken( MULTIPLY, yytext); return MULTIPLY; }
"/"                                                                     { setCurrentToken( DIVISION, yytext); return DIVISION; }
":="                                                                    { setCurrentToken( ASSIGN, yytext); return ASSIGN; }
"=="                                                                    { setCurrentToken( DOUBLE_EQ, yytext); return DOUBLE_EQ; }
"<>"                                                                    { setCurrentToken( NOT_EQ, yytext); return NOT_EQ; }
"<"                                                                     { setCurrentToken( L_THAN, yytext); return L_THAN; }
">"                                                                     { setCurrentToken( G_THAN, yytext); return G_THAN; }
"<="                                                                    { setCurrentToken( L_THAN_EQ, yytext); return L_THAN_EQ; }
">="                                                                    { setCurrentToken( G_THAN_EQ, yytext); return G_THAN_EQ; }
"or"                                                                    { setCurrentToken( OR, yytext); return OR; }
"and"                                                                   { setCurrentToken( AND, yytext); return AND; }
"not"                                                                   { setCurrentToken( NOT, yytext); return NOT; }


"else"                                                                  { setCurrentToken( ELSE, yytext); return ELSE; }
"float"                                                                 { setCurrentToken( FLOAT, yytext); return FLOAT; }
"func"                                                                  { setCurrentToken( FUNC, yytext); return FUNC; }
"if"                                                                    { setCurrentToken( IF, yytext); return IF; }
"implement"                                                             { setCurrentToken( IMPLEMENT, yytext); return IMPLEMENT; }
"class"                                                                 { setCurrentToken( CLASS, yytext); return CLASS; }
"attribute"                                                             { setCurrentToken( ATTRIBUTE, yytext); return ATTRIBUTE; }
"integer"                                                               { setCurrentToken( INTEGER, yytext); return INTEGER; }
"private"                                                               { setCurrentToken( PRIVATE, yytext); return PRIVATE; }
"public"                                                                { setCurrentToken( PUBLIC, yytext); return PUBLIC; }
"read"                                                                  { setCurrentToken( READ, yytext); return READ; }
"isa"                                                                   { setCurrentToken( ISA, yytext); return ISA; }
"return"                                                                { setCurrentToken( RETURN, yytext); return RETURN; }
"self"                                                                  { setCurrentToken( SELF, yytext); return SELF; }
"construct"                                                             { setCurrentToken( CONSTRUCT, yytext); return CONSTRUCT; }
"then"                                                                  { setCurrentToken( THEN, yytext); return THEN; }
"local"                                                                 { setCurrentToken( LOCAL, yytext); return LOCAL; }
"void"                                                                  { setCurrentToken( VOID, yytext); return VOID; }
"while"                                                                 { setCurrentToken( WHILE, yytext); return WHILE; }
"write"                                                                 { setCurrentToken( WRITE, yytext); return WRITE; }


[ \t]+                                                                  { update_column(yytext, yyleng); }                         
\n                                                                      { increment_line_no(); }


"//"[^\n]*                                                              ;
"/*"([^*]|\*+[^*/])*\*+\/                                               {for (int i = 0; yytext[i] != '\0'; i++) {if (yytext[i] == '\n') increment_line_no();}}
([1-9][0-9]*|0)(\.[0-9]*[1-9]|\.0)(e[+|-]([1-9][0-9]*|0))?              { setCurrentToken( FLOAT_LIT, yytext); return FLOAT_LIT; }  
[1-9][0-9]*|0                                                           { setCurrentToken( INT_LIT, yytext); return INT_LIT; }
[a-zA-Z]([a-zA-Z]|[0-9]|_)*                                             { setCurrentToken( ID, yytext); return ID; }
"."                                                                     { setCurrentToken( DOT, yytext); return DOT; }


.                                                                       { setCurrentToken( UNKNOWN, yytext); return UNKNOWN; }


%%


int yywrap () {
    setCurrentToken( END_OF_INPUT, "$"); return END_OF_INPUT;
    return 1;
}



