#include "../../lexical_analyzer/tokens.h"
#include "rules.h"

Rule rules[] = {

    {
        PROG, 
        {
            1, 
            {
                { 1, { CLASSORIMPORFUNC_LIST },                          "prog ->  classOrImplOrFuncList" }
            }
        }
    },
    {
        CLASSORIMPORFUNC_LIST, 
        {
            2, 
            {
                { 2, { CLASSORIMPORFUNC, CLASSORIMPORFUNC_LIST },   "classOrImplOrFuncList ->  classOrImplOrFunc classOrImplOrFuncList" },
                { 1, { EMPTY },                                     "classOrImplOrFuncList ->  ϵ" },
            }
        }
    },
    {
        CLASSORIMPORFUNC, 
        {
            3, 
            {
                { 1, { CLASSDECL },                                   "classOrImplOrFunc ->  classDecl" },
                { 1, { IMPLDEF },                                     "classOrImplOrFunc ->  implDef" },
                { 1, { FUNCDEF },                                     "classOrImplOrFunc ->  funcDef" },
            }
        }
    },
    {
        CLASSDECL, 
        {
            1, 
            {
                { 7, { CLASS, ID, INHERITANCE_OPTIONAL, L_CURLY, VISIBILITYMEMBERDECL_LIST, R_CURLY, SEMI_COL },  "classDecl ->  class id inheritanceOptional { visibilityMemberDeclList } ;" },
            }
        }
    },
    {
        INHERITANCE_OPTIONAL, 
        {
            2, 
            {
                { 3, { ISA, ID, INHERITANCE_LIST },     "inheritanceOptional ->  isa id inheritanceList" },
                { 1, { EMPTY },                         "inheritanceOptional ->  ϵ" },
            }
        }
    },
    {
        INHERITANCE_LIST, 
        {
            2, 
            {
                { 3, { COMMA, ID, INHERITANCE_LIST },       "inheritanceList ->  , id inheritanceList" },
                { 1, { EMPTY },                             "inheritanceList ->  ϵ" },
            }
        }
    },
    {
        VISIBILITYMEMBERDECL_LIST, 
        {
            2, 
            {
                { 3, { VISIBILITY, MEMBERDECL, VISIBILITYMEMBERDECL_LIST },       "visibilityMemberDeclList ->  visibility memberDecl visibilityMemberDeclList" },
                { 1, { EMPTY },                                                   "visibilityMemberDeclList ->  ϵ" },
            }
        }
    },
    {
        IMPLDEF, 
        {
            1, 
            {
                { 5, { IMPLEMENT, ID, L_CURLY, FUNCDEF_LIST, R_CURLY },       "implement id { funcDefList }" },
            }
        }
    },
    {
        FUNCDEF_LIST, 
        {
            2, 
            {
                { 2, { FUNCDEF, FUNCDEF_LIST },         "funcDefList ->  funcDef funcDefList" },
                { 1, { EMPTY },                         "funcDefList ->  ϵ" },
            }
        }
    }, 
    {
        FUNCDEF, 
        {
            1, 
            {
                { 2, { FUNCHEAD, FUNCBODY },         "funcDef ->  funcHead funcBody" },
            }
        }
    },
    {
        VISIBILITY, 
        {
            2, 
            {
                { 1, { PUBLIC },         "visibility ->  public" },
                { 1, { PRIVATE },        "visibility ->  private" },
            }
        }
    },
    {
        MEMBERDECL, 
        {
            2, 
            {
                { 1, { FUNCDECL },         "memberDecl ->  funcDecl" },
                { 1, { ATTRIBUTEDECL },    "memberDecl ->  attributeDecl" },
            }
        }
    },
    {
        FUNCDECL, 
        {
            1, 
            {
                { 2, { FUNCHEAD, SEMI_COL },         "funcDecl ->  funcHead ;" },
            }
        }
    },
    {
        FUNCHEAD, 
        {
            2, 
            {
                { 7, { FUNC, ID, L_PARAN, FPARAMS, R_PARAN, ARROW, RETURN_TYPE },           "funcHead ->  func id ( fParams ) => returnType" },
                { 4, { CONSTRUCT, L_PARAN, FPARAMS, R_PARAN },                              "funcHead ->  constructor ( fParams )" },
            }
        }
    },
    {
        FUNCBODY, 
        {
            1, 
            {
                { 3, { L_CURLY, VARDECLORSTMT_LIST, R_CURLY },            "funcBody ->  { varDeclOrStmtList }" },
            }
        }
    },
    {
        VARDECLORSTMT_LIST, 
        {
            2, 
            {
                { 2, { VARDECLORSTMT, VARDECLORSTMT_LIST },             "varDeclOrStmtList ->  varDeclOrStmt varDeclOrStmtList | ϵ" },
                { 1, { EMPTY },                                         "varDeclOrStmtList ->  ϵ" },
            }
        }
    },
    {
        VARDECLORSTMT, 
        {
            2, 
            {
                { 1, { LOCALVARDECL },              "varDeclOrStmt ->  localVarDecl" },
                { 1, { STATEMENT },                 "varDeclOrStmt ->  statement" },
            }
        }
    },
    {
        ATTRIBUTEDECL, 
        {
            1, 
            {
                { 2, { ATTRIBUTE, VARDECL },              "attributeDecl ->  attribute varDecl" },
            }
        }
    },
    {
        LOCALVARDECL, 
        {
            1, 
            {
                { 2, { LOCAL, VARDECL },              "localVarDecl ->  local varDecl" },
            }
        }
    },
    {
        VARDECL, 
        {
            1, 
            {
                { 5, { ID, COLON, TYPE, ARRAYSIZE_LIST, SEMI_COL },              "varDecl ->  id : type arraySizeList ;" },
            }
        }
    },
    {
        STATEMENT, 
        {
            6, 
            {
                { 2, { ASSIGNSTATORFUNCCALL, SEMI_COL },                                                    "statement ->  assignStatOrFuncCall ;" },
                { 9, { IF, L_PARAN, RELEXPR, R_PARAN, THEN, STATBLOCK, ELSE, STATBLOCK, SEMI_COL },         "statement ->  if ( relExpr ) then statBlock else statBlock ;" },
                { 6, { WHILE, L_PARAN, RELEXPR, R_PARAN, STATBLOCK, SEMI_COL },                             "statement ->  while ( relExpr ) statBlock ; " },
                { 5, { READ, L_PARAN, VARIABLE, R_PARAN, SEMI_COL },                                        "statement ->  read ( variable ) ; " },
                { 5, { WRITE, L_PARAN, EXPR, R_PARAN, SEMI_COL },                                           "statement ->  write ( expr ) ;" },
                { 5, { RETURN, L_PARAN, EXPR, R_PARAN, SEMI_COL },                                          "statement ->  return ( expr ) ;" },
            }
        }
    },
    {
        ASSIGNSTATORFUNCCALL, 
        {
            1, 
            {
                { 2, { ID, ASSIGNSTATORFUNCCALL_CONT },              "assignStatOrFuncCall ->  id assignStatOrFuncCallCont" },
            }
        }
    },
    {
        ASSIGNSTATORFUNCCALL_CONT, 
        {
            2, 
            {
                { 3, { INDICE_LIST, ASSIGNOP, EXPR },                            "assignStatOrFuncCallCont ->  indiceList assignOp expr" },
                { 3, { L_PARAN, APARAMS, R_PARAN },                              "assignStatOrFuncCallCont ->  ( aParams )" },
            }
        }
    },
    {
        STATBLOCK, 
        {
            3, 
            {
                { 3, { L_CURLY, STATEMENT_LIST, R_CURLY },      "statBlock ->  { statementList }" },
                { 1, { STATEMENT },                             "statBlock ->  statement" },
                { 1, { EMPTY },                                 "statBlock ->  ϵ" },
            }
        }
    },
    {
        STATEMENT_LIST, 
        {
            2, 
            {
                { 2, { STATEMENT, STATEMENT_LIST },         "statementList ->  statement statementList" },
                { 1, { EMPTY },                             "statementList ->  ϵ" },
            }
        }
    },
    {
        EXPR, 
        {
            1, 
            {
                { 2, { ARITHEXPR, EXPRCONT },          "expr ->  arithExpr exprCont" },
            }
        }
    },
    {
        EXPRCONT, 
        {
            2, 
            {
                { 2, { RELOP, ARITHEXPR },          "exprCont ->  relOp arithExpr" },
                { 1, { EMPTY },                     "exprCont ->  ϵ" },
            }
        }
    },
    {
        RELEXPR, 
        {
            1, 
            {
                { 3, { ARITHEXPR, RELOP, ARITHEXPR },          "relExpr ->  arithExpr relOp arithExpr" },
            }
        }
    },
    {
        ARITHEXPR, 
        {
            1, 
            {
                { 2, { TERM, ARITHOPERATIONS },          "arithExpr ->  term arithOperations" },
            }
        }
    },
    {
        ARITHOPERATIONS, 
        {
            2, 
            {
                { 3, { ADDOP, TERM, ARITHOPERATIONS },          "arithOperations ->  addOp term arithOperations" },
                { 1, { EMPTY },                                 "arithOperations ->  ϵ" },
            }
        }
    },
    {
        SIGN, 
        {
            2, 
            {
                { 1, { ADD },               "sign ->  +" },
                { 1, { SUBTRACT },          "sign ->  -" },
            }
        }
    },
    {
        TERM, 
        {
            1, 
            {
                { 2, { FACTOR, TERMOPERATIONS },               "term ->  factor termOperations" },
            }
        }
    },
    {
        TERMOPERATIONS, 
        {
            2, 
            {
                { 2, { MULTOP, FACTOR, TERMOPERATIONS },            "termOperations ->  multOp factor termOperations" },
                { 1, { EMPTY },                                     "termOperations ->  ϵ" },
            }
        }
    },
    {
        FACTOR, 
        {
            6, 
            {
                { 1, { VARIABLEORFUNCCALL },                "factor ->  variableOrFunctionCall" },
                { 1, { INT_LIT },                           "factor ->  intLit" },
                { 1, { FLOAT_LIT },                         "factor ->  floatLit" },
                { 3, { L_PARAN, ARITHEXPR, R_PARAN },       "factor ->  ( arithExpr )" },
                { 2, { NOT, FACTOR },                       "factor ->  not factor" },
                { 2, { SIGN, FACTOR },                      "factor ->  sign factor" },
            }
        }
    },
    {
        VARIABLEORFUNCCALL, 
        {
            1, 
            {
                { 2, { ID, VARIABLEORFUNCCALL_CONT },                "variableOrFunctionCall ->  id variableOrFunctionCallCont" },
            }
        }
    },
    {
        VARIABLE, 
        {
            1, 
            {
                { 2, { ID, INDICE_LIST },              "variable ->  id indiceList " },
            }
        }
    },
    {
        VARIABLEORFUNCCALL_CONT, 
        {
            2, 
            {
                { 1, { INDICE_LIST },                               "variableOrFunctionCallCont ->  indiceList" },
                { 3, { L_PARAN, APARAMS, R_PARAN },                 "variableOrFunctionCallCont ->  ( aParams )" },
            }
        }
    },
    {
        INDICE_LIST, 
        {
            2, 
             {
                { 2, { INDICE, INDICE_LIST },        "indiceList ->  indice indiceList" },
                { 1, { EMPTY },                      "indiceList ->  ϵ" },
            }
        }
    },
    {
        INDICE, 
        {
            1, 
             {
                { 3, { L_BRACKET, ARITHEXPR, R_BRACKET },        "indice ->  [ arithExpr ]" },
            }
        }
    },
    {
        ARRAYSIZE_CONT, 
        {
            2, 
             {
                { 2, { INT_LIT, R_BRACKET },        "arraySizeCont ->  intLit ] " },
                { 1, { R_BRACKET },                 "arraySizeCont ->  ] " },
            }
        }
    },
    {
        ARRAYSIZE, 
        {
            1, 
             {
                { 2, { L_BRACKET, ARRAYSIZE_CONT },        "arraySize ->  [ arraySizeCont" },
            }
        }
    },
    {
        TYPE, 
        {
            3, 
             {
                { 1, { INTEGER },       "type ->  integer" },
                { 1, { FLOAT },         "type ->  float" },
                { 1, { ID },            "type ->  id" },
            }
        }
    },
    {
        RETURN_TYPE, 
        {
            2, 
             {
                { 1, { TYPE },          "returnType ->  type" },
                { 1, { VOID },          "returnType ->  void" },
            }
        }
    },
    {
        FPARAMSTAIL_LIST, 
        {
            2, 
             {
                { 2, { FPARAMSTAIL, FPARAMSTAIL_LIST },         "fParamsTailList ->  fParamsTail fParamsTailList" },
                { 1, { EMPTY },                                 "fParamsTailList ->  ϵ" },
            }
        }
    },
    {
        FPARAMS, 
        {
            2, 
             {
                { 5, { ID, COLON, TYPE, ARRAYSIZE_LIST, FPARAMSTAIL_LIST },         "fParams ->  id : type arraySizeList fParamsTailList " },
                { 1, { EMPTY },                                                     "fParams ->  ϵ" },
            }
        }
    },
    {
        APARAMSTAIL_LIST, 
        {
            2, 
             {
                { 2, { APARAMSTAIL, APARAMSTAIL_LIST },         "aParamsTailList ->  aParamsTail aParamsTailList" },
                { 1, { EMPTY },                                 "aParamsTailList ->  ϵ" },
            }
        }
    },
    {
        APARAMS, 
        {
            2, 
             {
                { 2, { EXPR, APARAMSTAIL_LIST },            "aParams ->  expr aParamsTailList" },
                { 1, { EMPTY },                             "aParams ->  ϵ" },
            }
        }
    },
    {
        ARRAYSIZE_LIST, 
        {
            2, 
             {
                { 2, { ARRAYSIZE, ARRAYSIZE_LIST },         "arraySizeList ->   arraySize arraySizeList" },
                { 1, { EMPTY },                             "arraySizeList ->  ϵ" },
            }
        }
    },
    {
        FPARAMSTAIL, 
        {
            1, 
             {
                { 5, { COMMA, ID, COLON, TYPE, ARRAYSIZE_LIST },         "fParamsTail ->   , id : type arraySizeList" },
            }
        }
    },
    {
        APARAMSTAIL, 
        {
            1, 
             {
                { 2, { COMMA, EXPR },         "aParamsTail ->   , expr" },
            }
        }
    },
    {
        ASSIGNOP, 
        {
            1, 
             {
                { 1, { ASSIGN },         "assignOp ->   :=" },
            }
        }
    },
    {
        RELOP, 
        {
            6, 
             {
                { 1, { DOUBLE_EQ },         "assignOp ->   ==" },
                { 1, { NOT_EQ },            "assignOp ->   <>" },
                { 1, { L_THAN },            "assignOp ->   <" },
                { 1, { G_THAN },            "assignOp ->   >" },
                { 1, { L_THAN_EQ },         "assignOp ->   <=" },
                { 1, { G_THAN_EQ },         "assignOp ->   >=" },
            }
        }
    },
    {
        ADDOP, 
        {
            3, 
            {
                { 1, { ADD },       "addOp ->  +" },
                { 1, { SUBTRACT },  "addOp ->  -" },
                { 1, { OR },        "addOp ->  or" }
            }
        }
    },
    {
        MULTOP, 
        {
            3, 
            {
                { 1, { MULTIPLY }, "multOp ->  *" },
                { 1, { DIVISION }, "multOp ->  /" },
                { 1, { AND },      "multOp ->  and" }
            }
        }
    },
    

/* 
 {
        ARRAYSIZE_LIST, 
        {
            2, 
            {
                { 2, { ARRAYSIZE, ARRAYSIZE_LIST },         "arraySizeList ->  arraySize arraySizeList" },
                { 1, { EMPTY },                             "arraySizeList ->  ϵ" },
            }
        }
    },


    {
        ADDOP, 
        {
            3, 
            {
                { 1, { ADD },       "addOp ->  +" },
                { 1, { SUBTRACT },  "addOp ->  -" },
                { 1, { OR },        "addOp ->  or" }
            }
        }
    },
    {
        MULTOP, 
        {
            3, 
            {
                { 1, { MULTIPLY }, "multOp ->  *" },
                { 1, { DIVISION }, "multOp ->  /" },
                { 1, { AND },      "multOp ->  and" }
            }
        }
    },
     


    // 1. prog ->  classOrImplOrFuncList
    { PROG, {1, {{1, {CLASSORIMPORFUNC_LIST}, "prog ->  classOrImplOrFuncList"}}} },

    // 2. classOrImplOrFuncList ->  classOrImplOrFunc classOrImplOrFuncList | ε
    { CLASSORIMPORFUNC_LIST, {2, {
        {2, {CLASSORIMPORFUNC, CLASSORIMPORFUNC_LIST}, "classOrImplOrFuncList ->  classOrImplOrFunc classOrImplOrFuncList"},
        {1, {EMPTY}, "classOrImplOrFuncList ->  ε"}
    }}},

    // 3. classOrImplOrFunc ->  classDecl | implDef | funcDef
    { CLASSORIMPORFUNC, {3, {
        {1, {CLASSDECL}, "classOrImplOrFunc ->  classDecl"},
        {1, {IMPLDEF}, "classOrImplOrFunc ->  implDef"},
        {1, {FUNCDEF}, "classOrImplOrFunc ->  funcDef"}
    }}},

    // 4. classDecl ->  CLASS ID inheritanceOptional L_CURLY visibilityMemberDeclList R_CURLY
    { CLASSDECL, {1, {
        {6, {CLASS, ID, INHERITANCE_OPTIONAL, L_CURLY, VISIBILITYMEMBERDECL_LIST, R_CURLY}, "classDecl ->  CLASS ID inheritanceOptional L_CURLY visibilityMemberDeclList R_CURLY"}
    }}},

    // 5. inheritanceOptional ->  ISA inheritanceList | ε
    { INHERITANCE_OPTIONAL, {2, {
        {2, {ISA, INHERITANCE_LIST}, "inheritanceOptional ->  ISA inheritanceList"},
        {1, {EMPTY}, "inheritanceOptional ->  ε"}
    }}},

    // 6. inheritanceList ->  ID inheritanceListTail
    { INHERITANCE_LIST, {1, {
        {2, {ID, INHERITANCE_LIST_TAIL}, "inheritanceList ->  ID inheritanceListTail"}
    }}},

    // 7. inheritanceListTail ->  COMMA ID inheritanceListTail | ε
    { INHERITANCE_LIST_TAIL, {2, {
        {3, {COMMA, ID, INHERITANCE_LIST_TAIL}, "inheritanceListTail ->  COMMA ID inheritanceListTail"},
        {1, {EMPTY}, "inheritanceListTail ->  ε"}
    }}},

    // 8. visibilityMemberDeclList ->  visibility memberDecl visibilityMemberDeclList | ε
    { VISIBILITYMEMBERDECL_LIST, {2, {
        {3, {VISIBILITY, MEMBERDECL, VISIBILITYMEMBERDECL_LIST}, "visibilityMemberDeclList ->  visibility memberDecl visibilityMemberDeclList"},
        {1, {EMPTY}, "visibilityMemberDeclList ->  ε"}
    }}},

    // 9. implDef ->  IMPLEMENT ID L_CURLY funcDefList R_CURLY
    { IMPLDEF, {1, {
        {5, {IMPLEMENT, ID, L_CURLY, FUNCDEF_LIST, R_CURLY}, "implDef ->  IMPLEMENT ID L_CURLY funcDefList R_CURLY"}
    }}},

    // 10. funcDefList ->  funcDef funcDefList | ε
    { FUNCDEF_LIST, {2, {
        {2, {FUNCDEF, FUNCDEF_LIST}, "funcDefList ->  funcDef funcDefList"},
        {1, {EMPTY}, "funcDefList ->  ε"}
    }}},

    // 11. funcDef ->  FUNC funcHead funcBody
    { FUNCDEF, {1, {
        {3, {FUNC, FUNCHEAD, FUNCBODY}, "funcDef ->  FUNC funcHead funcBody"}
    }}},

    // 12. visibility ->  PUBLIC | PRIVATE
    { VISIBILITY, {2, {
        {1, {PUBLIC}, "visibility ->  PUBLIC"},
        {1, {PRIVATE}, "visibility ->  PRIVATE"}
    }}},

    // 13. memberDecl ->  attributeDecl | funcDecl
    { MEMBERDECL, {2, {
        {1, {ATTRIBUTEDECL}, "memberDecl ->  attributeDecl"},
        {1, {FUNCDECL}, "memberDecl ->  funcDecl"}
    }}},

    // 14. funcDecl ->  FUNC funcHead SEMI_COL
    { FUNCDECL, {1, {
        {3, {FUNC, FUNCHEAD, SEMI_COL}, "funcDecl ->  FUNC funcHead SEMI_COL"}
    }}},

    // 15. funcHead ->  ID L_PARAN fParams R_PARAN returnType
    { FUNCHEAD, {1, {
        {5, {ID, L_PARAN, FPARAMS, R_PARAN, RETURNTYPE}, "funcHead ->  ID L_PARAN fParams R_PARAN returnType"}
    }}},

    // 16. funcBody ->  L_CURLY varDeclOrStmt_List R_CURLY
    { FUNCBODY, {1, {
        {3, {L_CURLY, VARDECLORSTMT_LIST, R_CURLY}, "funcBody ->  L_CURLY varDeclOrStmt_List R_CURLY"}
    }}},

    // 17. varDeclOrStmt_List ->  varDeclOrStmt varDeclOrStmt_List | ε
    { VARDECLORSTMT_LIST, {2, {
        {2, {VARDECLORSTMT, VARDECLORSTMT_LIST}, "varDeclOrStmt_List ->  varDeclOrStmt varDeclOrStmt_List"},
        {1, {EMPTY}, "varDeclOrStmt_List ->  ε"}
    }}},

    // 18. varDeclOrStmt ->  varDecl | statement
    { VARDECLORSTMT, {2, {
        {1, {VARDECL}, "varDeclOrStmt ->  varDecl"},
        {1, {STATEMENT}, "varDeclOrStmt ->  statement"}
    }}},

    // 19. attributeDecl ->  ATTRIBUTE ID COLON type SEMI_COL
    { ATTRIBUTEDECL, {1, {
        {5, {ATTRIBUTE, ID, COLON, TYPE, SEMI_COL}, "attributeDecl ->  ATTRIBUTE ID COLON type SEMI_COL"}
    }}},

    // 20. localVarDecl ->  LOCAL varDecl
    { LOCALVARDECL, {1, {
        {2, {LOCAL, VARDECL}, "localVarDecl ->  LOCAL varDecl"}
    }}},

    // 21. varDecl ->  ID arraySizeList COLON type SEMI_COL
    { VARDECL, {1, {
        {5, {ID, ARRAYSIZE_LIST, COLON, TYPE, SEMI_COL}, "varDecl ->  ID arraySizeList COLON type SEMI_COL"}
    }}},

    // 22. arraySizeList ->  arraySize arraySizeList | ε
    { ARRAYSIZE_LIST, {2, {
        {2, {ARRAYSIZE, ARRAYSIZE_LIST}, "arraySizeList ->  arraySize arraySizeList"},
        {1, {EMPTY}, "arraySizeList ->  ε"}
    }}},

    // 23. statement ->  assignStatOrFuncCall SEMI_COL | returnStat SEMI_COL | ifStat | whileStat | readStat SEMI_COL | writeStat SEMI_COL | statBlock
    { STATEMENT, {7, {
        {2, {ASSIGNSTATORFUNCCALL, SEMI_COL}, "statement ->  assignStatOrFuncCall SEMI_COL"},
        {2, {RETURN, SEMI_COL}, "statement ->  returnStat SEMI_COL"},
        {1, {IF}, "statement ->  ifStat"},
        {1, {WHILE}, "statement ->  whileStat"},
        {2, {READ, SEMI_COL}, "statement ->  readStat SEMI_COL"},
        {2, {WRITE, SEMI_COL}, "statement ->  writeStat SEMI_COL"},
        {1, {STATBLOCK}, "statement ->  statBlock"}
    }}},

    // 24. assignStatOrFuncCall ->  variableOrFuncCall assignStatOrFuncCallCont
    { ASSIGNSTATORFUNCCALL, {1, {
        {2, {VARIABLEORFUNCCALL, ASSIGNSTATORFUNCCALLCONT}, "assignStatOrFuncCall ->  variableOrFuncCall assignStatOrFuncCallCont"}
    }}},

    // 25. assignStatOrFuncCallCont ->  assignOp arithExpr | ε
    { ASSIGNSTATORFUNCCALLCONT, {2, {
        {2, {ASSIGNOP, ARITHEXPR}, "assignStatOrFuncCallCont ->  assignOp arithExpr"},
        {1, {EMPTY}, "assignStatOrFuncCallCont ->  ε"}
    }}},

    // 26. statBlock ->  L_CURLY statementList R_CURLY
    { STATBLOCK, {1, {
        {3, {L_CURLY, STATEMENTLIST, R_CURLY}, "statBlock ->  L_CURLY statementList R_CURLY"}
    }}},

    // 27. statementList ->  statement statementList | ε
    { STATEMENTLIST, {2, {
        {2, {STATEMENT, STATEMENTLIST}, "statementList ->  statement statementList"},
        {1, {EMPTY}, "statementList ->  ε"}
    }}},

    // 28. expr ->  relExpr exprCont
    { EXPR, {1, {
        {2, {RELEXPR, EXPRCONT}, "expr ->  relExpr exprCont"}
    }}},

    // 29. exprCont ->  relOp relExpr exprCont | ε
    { EXPRCONT, {2, {
        {3, {RELOP, RELEXPR, EXPRCONT}, "exprCont ->  relOp relExpr exprCont"},
        {1, {EMPTY}, "exprCont ->  ε"}
    }}},

    // 30. relExpr ->  arithExpr
    { RELEXPR, {1, {
        {1, {ARITHEXPR}, "relExpr ->  arithExpr"}
    }}},

    // 31. arithExpr ->  term arithOperations
    { ARITHEXPR, {1, {
        {2, {TERM, ARITHOPERATIONS}, "arithExpr ->  term arithOperations"}
    }}},

    // 32. arithOperations ->  addOp term arithOperations | ε
    { ARITHOPERATIONS, {2, {
        {3, {ADDOP, TERM, ARITHOPERATIONS}, "arithOperations ->  addOp term arithOperations"},
        {1, {EMPTY}, "arithOperations ->  ε"}
    }}},

    // 33. sign ->  ADD | SUBTRACT
    { SIGN, {2, {
        {1, {ADD}, "sign ->  ADD"},
        {1, {SUBTRACT}, "sign ->  SUBTRACT"}
    }}},

    // 34. term ->  factor termOperations
    { TERM, {1, {
        {2, {FACTOR, TERMOPERATIONS}, "term ->  factor termOperations"}
    }}},

    // 35. termOperations ->  multOp factor termOperations | ε
    { TERMOPERATIONS, {2, {
        {3, {MULTOP, FACTOR, TERMOPERATIONS}, "termOperations ->  multOp factor termOperations"},
        {1, {EMPTY}, "termOperations ->  ε"}
    }}},

    // 36. factor ->  primary factorSuffix
    { FACTOR, {1, {
        {2, {PRIMARY, FACTORSUFFIX}, "factor ->  primary factorSuffix"}
    }}},

    // 37. primary ->  idOrSelf | INT_LIT | FLOAT_LIT | L_PARAN arithExpr R_PARAN | NOT factor | sign factor
    { PRIMARY, {6, {
        {1, {IDORSELF}, "primary ->  idOrSelf"},
        {1, {INT_LIT}, "primary ->  INT_LIT"},
        {1, {FLOAT_LIT}, "primary ->  FLOAT_LIT"},
        {3, {L_PARAN, ARITHEXPR, R_PARAN}, "primary ->  L_PARAN arithExpr R_PARAN"},
        {2, {NOT, FACTOR}, "primary ->  NOT factor"},
        {2, {SIGN, FACTOR}, "primary ->  sign factor"}
    }}},

    // 38. factorSuffix ->  postfixList | ε
    { FACTORSUFFIX, {2, {
        {1, {POSTFIXLIST}, "factorSuffix ->  postfixList"},
        {1, {EMPTY}, "factorSuffix ->  ε"}
    }}},

    // 39. postfixList ->  postfix postfixList | ε
    { POSTFIXLIST, {2, {
        {2, {POSTFIX, POSTFIXLIST}, "postfixList ->  postfix postfixList"},
        {1, {EMPTY}, "postfixList ->  ε"}
    }}},

    // 40. postfix ->  L_PARAN aParams R_PARAN | indiceList | DOT idOrSelf
    { POSTFIX, {3, {
        {3, {L_PARAN, APARAMS, R_PARAN}, "postfix ->  L_PARAN aParams R_PARAN"},
        {1, {INDICELIST}, "postfix ->  indiceList"},
        {2, {DOT, IDORSELF}, "postfix ->  DOT idOrSelf"}
    }}},

    // 41. variableOrFuncCall ->  idOrSelf variableOrFuncCallCont
    { VARIABLEORFUNCCALL, {1, {
        {2, {IDORSELF, VARIABLEORFUNCCALLCONT}, "variableOrFuncCall ->  idOrSelf variableOrFuncCallCont"}
    }}},

    // 42. variableOrFuncCallCont ->  postfixList
    { VARIABLEORFUNCCALLCONT, {1, {
        {1, {POSTFIXLIST}, "variableOrFuncCallCont ->  postfixList"}
    }}},

    // 43. variable ->  idOrSelf indiceList
    { VARIABLE, {1, {
        {2, {IDORSELF, INDICELIST}, "variable ->  idOrSelf indiceList"}
    }}},

    // 44. indiceList ->  indice indiceList | ε
    { INDICELIST, {2, {
        {2, {INDICE, INDICELIST}, "indiceList ->  indice indiceList"},
        {1, {EMPTY}, "indiceList ->  ε"}
    }}},

    // 45. indice ->  L_BRACKET arithExpr R_BRACKET
    { INDICE, {1, {
        {3, {L_BRACKET, ARITHEXPR, R_BRACKET}, "indice ->  L_BRACKET arithExpr R_BRACKET"}
    }}},

    // 46. idNest ->  ID idNestCont
    { IDNEST, {1, {
        {2, {ID, IDNESTCONT}, "idNest ->  ID idNestCont"}
    }}},

    // 47. idNestCont ->  DOT ID idNestCont | ε
    { IDNESTCONT, {2, {
        {3, {DOT, ID, IDNESTCONT}, "idNestCont ->  DOT ID idNestCont"},
        {1, {EMPTY}, "idNestCont ->  ε"}
    }}},

    // 48. idOrSelf ->  ID | SELF
    { IDORSELF, {2, {
        {1, {ID}, "idOrSelf ->  ID"},
        {1, {SELF}, "idOrSelf ->  SELF"}
    }}},

    // 49. arraySize ->  L_BRACKET INT_LIT R_BRACKET arraySizeCont
    { ARRAYSIZE, {1, {
        {4, {L_BRACKET, INT_LIT, R_BRACKET, ARRAYSIZECONT}, "arraySize ->  L_BRACKET INT_LIT R_BRACKET arraySizeCont"}
    }}},

    // 50. arraySizeCont ->  L_BRACKET INT_LIT R_BRACKET arraySizeCont | ε
    { ARRAYSIZECONT, {2, {
        {4, {L_BRACKET, INT_LIT, R_BRACKET, ARRAYSIZECONT}, "arraySizeCont ->  L_BRACKET INT_LIT R_BRACKET arraySizeCont"},
        {1, {EMPTY}, "arraySizeCont ->  ε"}
    }}},

    // 51. type ->  ID | INTEGER | FLOAT | VOID
    { TYPE, {4, {
        {1, {ID}, "type ->  ID"},
        {1, {INTEGER}, "type ->  INTEGER"},
        {1, {FLOAT}, "type ->  FLOAT"},
        {1, {VOID}, "type ->  VOID"}
    }}},

    // 52. returnType ->  COLON type | ε
    { RETURNTYPE, {2, {
        {2, {COLON, TYPE}, "returnType ->  COLON type"},
        {1, {EMPTY}, "returnType ->  ε"}
    }}},

    // 53. fParams ->  ID COLON type fParamsTailList | ε
    { FPARAMS, {2, {
        {4, {ID, COLON, TYPE, FPARAMSTAILLIST}, "fParams ->  ID COLON type fParamsTailList"},
        {1, {EMPTY}, "fParams ->  ε"}
    }}},

    // 54. fParamsTailList ->  fParamsTail fParamsTailList | ε
    { FPARAMSTAILLIST, {2, {
        {2, {FPARAMSTAIL, FPARAMSTAILLIST}, "fParamsTailList ->  fParamsTail fParamsTailList"},
        {1, {EMPTY}, "fParamsTailList ->  ε"}
    }}},

    // 55. fParamsTail ->  COMMA ID COLON type
    { FPARAMSTAIL, {1, {
        {4, {COMMA, ID, COLON, TYPE}, "fParamsTail ->  COMMA ID COLON type"}
    }}},

    // 56. aParams ->  arithExpr aParamsTailList | ε
    { APARAMS, {2, {
        {2, {ARITHEXPR, APARAMSTAILLIST}, "aParams ->  arithExpr aParamsTailList"},
        {1, {EMPTY}, "aParams ->  ε"}
    }}},

    // 57. aParamsTailList ->  aParamsTail aParamsTailList | ε
    { APARAMSTAILLIST, {2, {
        {2, {APARAMSTAIL, APARAMSTAILLIST}, "aParamsTailList ->  aParamsTail aParamsTailList"},
        {1, {EMPTY}, "aParamsTailList ->  ε"}
    }}},

    // 58. aParamsTail ->  COMMA arithExpr
    { APARAMSTAIL, {1, {
        {2, {COMMA, ARITHEXPR}, "aParamsTail ->  COMMA arithExpr"}
    }}},

    // 59. assignOp ->  ASSIGN
    { ASSIGNOP, {1, {
        {1, {ASSIGN}, "assignOp ->  ASSIGN"}
    }}},

    // 60. relOp ->  DOUBLE_EQ | NOT_EQ | L_THAN | G_THAN | L_THAN_EQ | G_THAN_EQ
    { RELOP, {6, {
        {1, {DOUBLE_EQ}, "relOp ->  DOUBLE_EQ"},
        {1, {NOT_EQ}, "relOp ->  NOT_EQ"},
        {1, {L_THAN}, "relOp ->  L_THAN"},
        {1, {G_THAN}, "relOp ->  G_THAN"},
        {1, {L_THAN_EQ}, "relOp ->  L_THAN_EQ"},
        {1, {G_THAN_EQ}, "relOp ->  G_THAN_EQ"}
    }}},

    // 61. addOp ->  ADD | SUBTRACT
    { ADDOP, {2, {
        {1, {ADD}, "addOp ->  ADD"},
        {1, {SUBTRACT}, "addOp ->  SUBTRACT"}
    }}},

    // 62. multOp ->  MULTIPLY | DIVISION
    { MULTOP, {2, {
        {1, {MULTIPLY}, "multOp ->  MULTIPLY"},
        {1, {DIVISION}, "multOp ->  DIVISION"}
    }}},







*/

    
};

