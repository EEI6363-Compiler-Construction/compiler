#ifndef TOKENS_H
#define TOKENS_H


typedef enum  {
    L_PARAN = 100,
    R_PARAN,
    L_CURLY,
    R_CURLY,
    L_BRACKET,
    R_BRACKET,
    SEMI_COL,
    COMMA,
    COLON,
    DOT,
    ARROW,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVISION,
    ASSIGN,
    DOUBLE_EQ,
    NOT_EQ,
    L_THAN,
    G_THAN,
    L_THAN_EQ,
    G_THAN_EQ,
    OR,
    AND,
    NOT,
    IF,
    ELSE,
    FLOAT,
    FUNC,
    IMPLEMENT,
    CLASS,
    ATTRIBUTE,
    INTEGER,
    PRIVATE,
    PUBLIC,
    READ,
    RETURN,
    SELF,
    CONSTRUCT,
    THEN,
    LOCAL,
    VOID,
    WHILE,
    WRITE,
    ISA,
    ID,
    UNKNOWN,
    INT_LIT,
    FLOAT_LIT,
    INLINE_COM,
    BLOCK_COM,
    END_OF_INPUT,
    EMPTY

} Terminals;


typedef enum  {
    PROG = 0,
    CLASSORIMPORFUNC_LIST,
    CLASSORIMPORFUNC,
    CLASSDECL,
    INHERITANCE_OPTIONAL,
    INHERITANCE_LIST,
    VISIBILITYMEMBERDECL_LIST,
    IMPLDEF,
    FUNCDEF_LIST,
    FUNCDEF,
    VISIBILITY,
    MEMBERDECL,
    FUNCDECL,
    FUNCHEAD,
    FUNCBODY,
    VARDECLORSTMT_LIST,
    VARDECLORSTMT,
    ATTRIBUTEDECL,
    LOCALVARDECL,
    VARDECL,
    STATEMENT,
    ASSIGNSTATORFUNCCALL,
    ASSIGNSTATORFUNCCALL_CONT,
    STATBLOCK,
    STATEMENT_LIST,
    EXPR,
    EXPRCONT,
    RELEXPR,
    ARITHEXPR,
    ARITHOPERATIONS,
    SIGN,
    TERM,
    TERMOPERATIONS,
    FACTOR,
    VARIABLEORFUNCCALL,
    VARIABLE,
    VARIABLEORFUNCCALL_CONT,
    INDICE_LIST,
    INDICE,
    ARRAYSIZE_CONT,
    ARRAYSIZE,
    TYPE,
    RETURN_TYPE,
    FPARAMSTAIL_LIST,
    FPARAMS,
    APARAMSTAIL_LIST,
    APARAMS,
    ARRAYSIZE_LIST,
    FPARAMSTAIL,
    APARAMSTAIL,
    ASSIGNOP,
    RELOP,
    ADDOP,
    MULTOP,
    
} NonTerminals;


typedef struct {
    Terminals terminal;
    char* lexeme;
} TOKEN;

extern TOKEN currentToken;

void setCurrentToken(Terminals type, const char* lexeme);

TOKEN getCurrentToken(void);

#endif