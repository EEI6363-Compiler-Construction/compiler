#include "../syntax_analyzer/parse_table/parse_table.h"


char* terminalStrings[] = {
   [100] = "L_PARAN",
   [101] = "R_PARAN",
   [102] = "L_CURLY",
   [103] = "R_CURLY",
   [104] = "L_BRACKET",
   [105] = "R_BRACKET",
   [106] = "SEMI_COL",
   [107] = "COMMA",
   [108] = "COLON",
   [109] = "DOT",
   [110] = "ARROW",
   [111] = "ADD",
   [112] = "SUBTRACT",
   [113] = "MULTIPLY",
   [114] = "DIVISION",
   [115] = "ASSIGN",
   [116] = "DOUBLE_EQ",
   [117] = "NOT_EQ",
   [118] = "L_THAN",
   [119] = "G_THAN",
   [120] = "L_THAN_EQ",
   [121] = "G_THAN_EQ",
   [122] = "OR",
   [123] = "AND",
   [124] = "NOT",
   [125] = "IF",
   [126] = "ELSE",
   [127] = "FLOAT",
   [128] = "FUNC",
   [129] = "IMPLEMENT",
   [130] = "CLASS",
   [131] = "ATTRIBUTE",
   [132] = "INTEGER",
   [133] = "PRIVATE",
   [134] = "PUBLIC",
   [135] = "READ",
   [136] = "RETURN",
   [137] = "SELF",
   [138] = "CONSTRUCT",
   [139] = "THEN",
   [140] = "LOCAL",
   [141] = "VOID",
   [142] = "WHILE",
   [143] = "WRITE",
   [144] = "ISA",
   [145] = "ID",
   [146] = "UNKNOWN",
   [147] = "INT_LIT",
   [148] = "FLOAT_LIT",
   [149] = "INLINE_COM",
   [150] = "BLOCK_COM",
   [151] = "END_OF_INPUT",
   [152] = "EMPTY"
};

char* nonTerminalStrings[] = {
    "PROG",
    "CLASSORIMPORFUNC_LIST",
    "CLASSORIMPORFUNC",
    "CLASSDECL",
    "INHERITANCE_OPTIONAL",
    "INHERITANCE_LIST",
    "VISIBILITYMEMBERDECL_LIST",
    "IMPLDEF",
    "FUNCDEF_LIST",
    "FUNCDEF",
    "VISIBILITY",
    "MEMBERDECL",
    "FUNCDECL",
    "FUNCHEAD",
    "FUNCBODY",
    "VARDECLORSTMT_LIST",
    "VARDECLORSTMT",
    "ATTRIBUTEDECL",
    "LOCALVARDECL",
    "VARDECL",
    "STATEMENT",
    "ASSIGNSTATORFUNCCALL",
    "ASSIGNSTATORFUNCCALL_CONT",
    "STATBLOCK",
    "STATEMENT_LIST",
    "EXPR",
    "EXPRCONT",
    "RELEXPR",
    "ARITHEXPR",
    "ARITHOPERATIONS",
    "SIGN",
    "TERM",
    "TERMOPERATIONS",
    "FACTOR",
    "VARIABLEORFUNCCALL",
    "VARIABLE",
    "VARIABLEORFUNCCALL_CONT",
    "INDICE_LIST",
    "INDICE",
    "ARRAYSIZE_CONT",
    "ARRAYSIZE",
    "TYPE",
    "RETURN_TYPE",
    "FPARAMSTAIL_LIST",
    "FPARAMS",
    "APARAMSTAIL_LIST",
    "APARAMS",
    "ARRAYSIZE_LIST",
    "FPARAMSTAIL",
    "APARAMSTAIL",
    "ASSIGNOP",
    "RELOP",
    "ADDOP",
    "MULTOP",
};

char* getTerminalString(int value) {

    // * (value <= MAX_TERMINALS) because EMPTY is 52nd enum
    if (value >= TERMINALS_START && value <= TERMINALS_END) {
        return terminalStrings[value];
    }
    return "UNKNOWN_TERMINAL";
};

char* getNonTerminalString(int value) {
    
    if (value >= 0 && value < MAX_NONTERMINALS) {
        return nonTerminalStrings[value];
    }
    return "UNKNOWN_NON_TERMINAL";
};