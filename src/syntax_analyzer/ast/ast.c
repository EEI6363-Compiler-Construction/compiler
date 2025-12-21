#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "../../semantic_analyzer/symbol_table/symbol_table.h"

int semTop = 0;
int penTop = 0;
node* semanticStack[MAX_SEMANTIC_STACK] = {NULL};
pendingNonTerminal* pendingStack[250] = {NULL};

char* nodeTypeStrings[MAX_NODE_TYPES] = {
    "CLASS_NODE",
    "IMPL_NODE",
    "FUNC_NODE",
    "FUNCBODY_NODE",
    "FUNCDEF_NODE",
    "ATTRIBUTE_NODE",
    "ASSIGN_NODE",
    "IF_NODE",
    "WHILE_NODE",
    "READ_NODE",
    "WRITE_NODE",
    "RETURN_NODE",
    "BIN_OP_NODE",
    "UN_OP_NODE",
    "ID_NODE",
    "LITERAL_NODE",
    "TYPE_NODE",
    "BLOCK_NODE",
    "PARAM_NODE",
    "MULTOP_NODE",
    "FUNCTIONCALL_NODE",
    "VARIABLECALL_NODE",
    "INHERITANCE_NODE",
    "EMPTY_NODE",
    "PROGRAM_NODE",
    "VISIBILITY_NODE",
    "LEAF_NODE",
    "CLASSBODY_NODE",
    "FPARAMS_NODE",
    "FPARAM_NODE",
    "APARAMS_NODE",
    "APARAM_NODE",
    "ARRAY_NODE",
    "ARRAYSIZE_NODE",
    "CONSTRUCTORDEF_NODE",
    "CONSTRUCTOR_NODE",
    "CONSTRUCTORBODY_NODE",
    "LOCALVAR_NODE",
    "VARDECL_NODE",
    "ATTRIBUTE_OR_LOCAL_NODE",
    "VARIABLE_NODE",
    "INDICELIST_NODE",
    "ARITHEXPR_NODE",
    "OPEREND_NODE",
    "RIGHTOPR_NODE",
    "RELOP_NODE",
    "ASSIGNOP_NODE",
    "STATBLOCK_NODE",
    "THEN_NODE",
    "ELSE_NODE",
};

char* getNodeTypeString(int type) {

    if (type >= 0 && type < MAX_NODE_TYPES) {
        return nodeTypeStrings[type];
    }
    return "UNKNOWN_NODE_TYPE";
}

void pushSemanticStack(node* node) {
    if (semTop >= MAX_SEMANTIC_STACK) {
        fprintf(stderr, "Semantic stack overflow!\n");
        exit(1);
    }
    semanticStack[semTop++] = node;
}

node* popSemanticStack() {
    if (semTop < 0) {
        fprintf(stderr, "Semantic stack underflow!\n");
        exit(1);
    }
    if (semTop == 0)
    {
        return semanticStack[0];
    }
    

    return semanticStack[--semTop];
}

void pushPendingStack(NonTerminals nonTerminal, int symbolCount) {
    if (penTop >= 250) {
        fprintf(stderr, "Pending stack overflow!\n");
        exit(1);
    }

    pendingNonTerminal* penTerminal = malloc(sizeof(pendingNonTerminal));
    penTerminal->nonTerminal = nonTerminal;
    penTerminal->symbolCount = symbolCount;
    penTerminal->currentSymbol = 0;

    pendingStack[penTop++] = penTerminal;
}

pendingNonTerminal* popPendingStack() {
    if (penTop <= 0) {
        fprintf(stderr, "Pending stack underflow!\n");
        exit(1);
    }

    pendingNonTerminal *p = pendingStack[--penTop];
    return p;
}

node* operatorNode(nodeType type, int nops, ...) {
    va_list ap;
    node* p = malloc(sizeof(node));
    if (!p) { perror("out of memory"); exit(1); }

    p->type = type;
    p->nops = nops;
    p->op = malloc(sizeof(node*) * nops);

    va_start(ap, nops);
    for (int i = 0; i < nops; i++)
        p->op[i] = (struct node*) va_arg(ap, node*);
    va_end(ap);

    p->value = NULL;
    p->typeName = NULL;
    return p;
}

node* literalIntNode(int value, int line_no, int col_no) {
    node* p = malloc(sizeof(node));
    p->type = LITERAL_NODE;
    p->intValue = value;
    p->floatValue = 0.0f;
    p->value = malloc(20);
    snprintf(p->value, 20, "%d", value);
    p->nops = 0;
    p->op = NULL;
    p->col_no = col_no;
    p->line_no = line_no;
    return p;
}

node* literalFloatNode(float value, int line_no, int col_no) {
    node* p = malloc(sizeof(node));
    p->type = LITERAL_NODE;
    p->floatValue = value;
    p->intValue = 0;
    p->value = malloc(20);
    snprintf(p->value, 20, "%d", value);
    p->nops = 0;
    p->op = NULL;
    p->col_no = col_no;
    p->line_no = line_no;
    return p;
}

node* idNode(char* name, int line_no, int col_no) {
    node* p = malloc(sizeof(node));
    p->type = ID_NODE;
    p->value = strdup(name);
    p->nops = 0;
    p->op = NULL;
    p->col_no = col_no;
    p->line_no = line_no;
    return p;
}

node* visibilityNode(char* name) {
    node* p = malloc(sizeof(node));
    p->type = VISIBILITY_NODE;
    p->value = strdup(name);
    p->nops = 0;
    p->op = NULL;
    return p;
}

node* typeNode(char* name) {
    node* p = malloc(sizeof(node));
    p->type = TYPE_NODE;
    p->value = strdup(name);
    p->nops = 0;
    p->op = NULL;
    return p;
}

node* createLeafNode(char* value, nodeType nodeType, int line_no, int col_no) {

    node* p = malloc(sizeof(node));
    p->type = nodeType;
    p->value = strdup(value);
    p->nops = 0;
    p->op = NULL;
    p->col_no = col_no;
    p->line_no = line_no;
    return p;
}

void reverseChildrenNodesOrder(node* nodeToReverse){

    int i;
    int j;

    for (i = 0; i < nodeToReverse->nops / 2; i++) {
            
        j = nodeToReverse->nops - i - 1;

        node* temp = nodeToReverse->op[i];
        nodeToReverse->op[i] = nodeToReverse->op[j];
        nodeToReverse->op[j] = temp;
    }

}

void createASTNode(pendingNonTerminal *pendingNonTerminal){

    int nonterminal = pendingNonTerminal->nonTerminal;

    switch (nonterminal){

        case FUNCDEF_LIST :{

            node* topNode = semanticStack[semTop - 1];

            if (topNode->type == IMPL_NODE){

                node* implDef = popSemanticStack(); 
                node* funcOrConstruct = popSemanticStack(); 

                implDef->op = realloc(implDef->op, sizeof(node*) * (implDef->nops + 1));
                implDef->op[implDef->nops] = funcOrConstruct; 
                implDef->nops++;

                pushSemanticStack(implDef);
            }
            
            else {

                node* funcOrConstruct = popSemanticStack(); 

                node* implDef = operatorNode(IMPL_NODE, 0);
                implDef->op = malloc(sizeof(node*));
                implDef->op[implDef->nops] = funcOrConstruct;
                implDef->nops++;
                pushSemanticStack(implDef);

            }

            break;
        }

        case IMPLDEF: {

            if(pendingNonTerminal->symbols[3].symbol == EMPTY) {

                node* implNode = operatorNode(IMPL_NODE, 0);; 
                node* id = popSemanticStack(); 
    
                implNode->value = id->value;

                pushSemanticStack(implNode);
            } 

            else {

                node* implNode = popSemanticStack(); 
                node* id = popSemanticStack(); 
    
                implNode->value = id->value;
    
                reverseChildrenNodesOrder(implNode);
                pushSemanticStack(implNode);
            }

            break;
        }

        case FUNCDEF: {

            node* topNode = semanticStack[semTop - 2];

            if (topNode->type == FUNCDEF_NODE){

                node* funcBody = popSemanticStack();        
                node* funcDef = popSemanticStack();     

                funcDef->type = FUNC_NODE;
                funcDef->op = realloc(funcDef->op, sizeof(node*) * (funcDef->nops + 1));
                funcDef->op[funcDef->nops] = funcBody; 
                funcDef->nops++;

                pushSemanticStack(funcDef);

                topNode = semanticStack[semTop];
            }

            else if (topNode->type == CONSTRUCTORDEF_NODE){
                
                node* constructBody = popSemanticStack();        
                node* constructDef = popSemanticStack();    

                constructDef->type = CONSTRUCTOR_NODE;
                constructBody->type = CONSTRUCTORBODY_NODE;
                constructDef->op = realloc(constructDef->op, sizeof(node*) * (constructDef->nops + 1));
                constructDef->op[constructDef->nops] = constructBody; 
                constructDef->nops++;

                pushSemanticStack(constructDef);

                topNode = semanticStack[semTop - 1];
            }
            
            break;
        }
        
        case FUNCBODY: {
            
            if(pendingNonTerminal->symbols[1].symbol == EMPTY) {
                
                node* funcBody = operatorNode(FUNCBODY_NODE, 0);
                funcBody->value = "emptyBody";
                pushSemanticStack(funcBody);
            } 
            
            else {

                node* funcBody = popSemanticStack();
                reverseChildrenNodesOrder(funcBody);
                pushSemanticStack(funcBody);
            }

            break;
        }

        case CLASSDECL: {

            //! CLASSDECL => [ CLASS  ID  INHERITANCE_OPTIONAL  L_CURLY  VISIBILITYMEMBERDECL_LIST  R_CURLY  SEMI_COL ]

            if(pendingNonTerminal->symbols[4].symbol == EMPTY && pendingNonTerminal->symbols[2].symbol == EMPTY) {

                printf("\nBOTH empty");
                node* id = popSemanticStack();
                node* classDecl = operatorNode(CLASS_NODE, 0);
                classDecl->value = id->value;
                classDecl->col_no = id->col_no;
                classDecl->line_no = id->line_no;
                pushSemanticStack(classDecl);
            } 

            else if(pendingNonTerminal->symbols[4].symbol != EMPTY && pendingNonTerminal->symbols[2].symbol != EMPTY) {

                node* classBody = popSemanticStack();
                node* isa = popSemanticStack();

                reverseChildrenNodesOrder(isa);

                node* id = popSemanticStack();
                node* classDecl = operatorNode(CLASS_NODE, 2, isa, classBody);
                classDecl->value = id->value;
                classDecl->col_no = id->col_no;
                classDecl->line_no = id->line_no;
                pushSemanticStack(classDecl);

            }
            
            else if (pendingNonTerminal->symbols[2].symbol == EMPTY) {

                printf("\nINHERITANCE_OPTIONAL empty");
                node* classBody = popSemanticStack();
                node* id = popSemanticStack();
                node* classDecl = operatorNode(CLASS_NODE, 1, classBody);
                classDecl->value = id->value;
                classDecl->col_no = id->col_no;
                classDecl->line_no = id->line_no;
                pushSemanticStack(classDecl);
            } 


            else if (pendingNonTerminal->symbols[4].symbol == EMPTY) {

                printf("\nVISIBILITYMEMBERDECL_LIST empty");
                node* isa = popSemanticStack();
                reverseChildrenNodesOrder(isa);
                node* id = popSemanticStack();
                node* classDecl = operatorNode(CLASS_NODE, 1, isa);
                classDecl->value = id->value;
                classDecl->col_no = id->col_no;
                classDecl->line_no = id->line_no;
                pushSemanticStack(classDecl);
            } 
        
            break;
        }

        case INHERITANCE_OPTIONAL: {

            if (pendingNonTerminal->symbols[2].symbol == EMPTY) {

                node* id = popSemanticStack();
                node* inheritanceOptional = operatorNode(INHERITANCE_NODE, 1, id);
                inheritanceOptional->value = "isa";
                inheritanceOptional->col_no = id->col_no;
                inheritanceOptional->line_no = id->line_no;
                pushSemanticStack(inheritanceOptional);
            } 
            
            else {

                node* inheritanceOptional = popSemanticStack();
                node* id = popSemanticStack();
                inheritanceOptional->op = realloc(inheritanceOptional->op, sizeof(node*) * (inheritanceOptional->nops + 1));
                inheritanceOptional->op[inheritanceOptional->nops] = id;
                inheritanceOptional->nops++;
        
                pushSemanticStack(inheritanceOptional);
            }

            break;

        }
        
        case INHERITANCE_LIST: {

            if (pendingNonTerminal->symbols[2].symbol != EMPTY) {

                node* inheritanceOptional = popSemanticStack();
                node* id = popSemanticStack();

                inheritanceOptional->op = realloc(inheritanceOptional->op, sizeof(node*) * (inheritanceOptional->nops + 1));
                inheritanceOptional->op[inheritanceOptional->nops] = id;
                inheritanceOptional->nops++;

                pushSemanticStack(inheritanceOptional);
            } 
            
            else {
                
                node* id = popSemanticStack();
                node* inheritanceOptional = operatorNode(INHERITANCE_NODE, 1, id);
                inheritanceOptional->value = "isa";
                inheritanceOptional->col_no = id->col_no;
                inheritanceOptional->line_no = id->line_no;
                pushSemanticStack(inheritanceOptional);
            }
            
            break;
        
        }

        case PROG: {

            if (pendingNonTerminal->symbols[0].symbol != EMPTY) {
                
                node* program = operatorNode(PROGRAM_NODE, semTop);
                program->value = "program";

                int noOfChildren = semTop;

                for (int i = 0; i < noOfChildren; i++){
                    node* child = popSemanticStack();
                    program->op[i] = child;
                }
                
                reverseChildrenNodesOrder(program);
                pushSemanticStack(program);
            } 
            
            else {

                node* program = operatorNode(PROGRAM_NODE, 0);
                program->value = "empty_program";
                pushSemanticStack(program);
            }

            break;
        }

        case LOCALVARDECL: {

            node* varDecl = popSemanticStack();
            node* local = popSemanticStack();

            varDecl->type = LOCALVAR_NODE;

            reverseChildrenNodesOrder(varDecl);

            pushSemanticStack(varDecl);

            break;
        }
        
        case ATTRIBUTEDECL: {

            node* varDecl = popSemanticStack();
            node* attribute = popSemanticStack();

            varDecl->type = ATTRIBUTE_NODE;

            pushSemanticStack(varDecl);

            break;
        }

        case VARDECL: {

            if (pendingNonTerminal->symbols[3].symbol == EMPTY){

                node* type = popSemanticStack();
                type->type = TYPE_NODE;
                node* id = popSemanticStack();
                id->op = malloc(sizeof(node*));
                id->type = VARDECL_NODE;
                id->op[id->nops] = type;
                id->nops++;
                pushSemanticStack(id);
            } 
            
            else {

                node* arraySize = popSemanticStack();
                node* type = popSemanticStack();
                type->type = TYPE_NODE;
                node* id = popSemanticStack();
                id->op = malloc(sizeof(node*) * 2);
                id->type = VARDECL_NODE;
                id->op[id->nops] = type;
                id->nops++;
                id->op[id->nops] = arraySize;
                id->nops++;
                reverseChildrenNodesOrder(id);
                pushSemanticStack(id);
            }

            break;
        }

        case VARDECLORSTMT_LIST: {

             node* topNode = semanticStack[semTop - 1];

             if (topNode->type == FUNCBODY_NODE){

                node* funcBody = popSemanticStack(); 
                node* varDeclOrStmt = popSemanticStack(); 

                funcBody->op = realloc(funcBody->op, sizeof(node*) * (funcBody->nops + 1));
                funcBody->op[funcBody->nops] = varDeclOrStmt; 
                funcBody->nops++;

                pushSemanticStack(funcBody);
            }
            
            else {

                node* varDeclOrStmt = popSemanticStack(); 
                node* funcBody = operatorNode(FUNCBODY_NODE, 1, varDeclOrStmt);
                funcBody->col_no = varDeclOrStmt->col_no;
                funcBody->line_no = varDeclOrStmt->line_no;
                pushSemanticStack(funcBody);

            }

            break;
        }

        case FUNCHEAD: {

            //! FUNCHEAD => [ FUNC  ID  L_PARAN  FPARAMS  R_PARAN  ARROW  RETURN_TYPE ]
            if (pendingNonTerminal->symbols[0].symbol == FUNC){
                
                if (pendingNonTerminal->symbols[3].symbol == EMPTY){

                    node* type = popSemanticStack();
                    type->type = TYPE_NODE;
                    node* id = popSemanticStack();
                    id->op = malloc(sizeof(node*));
                    id->type = FUNCDEF_NODE;
                    id->op[id->nops] = type;
                    id->nops++;
                    pushSemanticStack(id);
                } 

                else {

                    node* type = popSemanticStack();
                    type->type = TYPE_NODE;
                    node* fparams = popSemanticStack();
                    reverseChildrenNodesOrder(fparams);
                    node* id = popSemanticStack();
                    id->op = malloc(sizeof(node*) * 2);
                    id->type = FUNCDEF_NODE;
                    id->op[id->nops] = type;
                    id->nops++;
                    id->op[id->nops] = fparams;
                    id->nops++;
                    reverseChildrenNodesOrder(id);
                    pushSemanticStack(id);
                }
            }

            //!FUNCHEAD => [ CONSTRUCT  L_PARAN  FPARAMS  R_PARAN ]
            else if (pendingNonTerminal->symbols[0].symbol == CONSTRUCT){

                if (pendingNonTerminal->symbols[2].symbol == EMPTY){
                    
                    node* constructor = operatorNode(CONSTRUCTORDEF_NODE, 0);
                    constructor->value = "constructor";
                    pushSemanticStack(constructor);
                }

                else {

                    node* fParams = popSemanticStack();
                    node* constructor = operatorNode(CONSTRUCTORDEF_NODE, 1, fParams);

                    constructor->value = "constructor";
                    reverseChildrenNodesOrder(fParams); 
                    pushSemanticStack(constructor);
                }
            }

            break;
        }

        case MEMBERDECL: {

            node* memberDecl = popSemanticStack();
            node* visibility = popSemanticStack();

            memberDecl->op = realloc(memberDecl->op, sizeof(node*) * (memberDecl->nops + 1));
            memberDecl->op[memberDecl->nops] = visibility;
            memberDecl->nops++;
            reverseChildrenNodesOrder(memberDecl);

            node* topNode = semanticStack[semTop - 1];
             
            if (topNode->type == CLASSBODY_NODE){

                node* classBody = popSemanticStack();
                classBody->op = realloc(classBody->op, sizeof(node*) * (classBody->nops + 1));
                classBody->op[classBody->nops] = memberDecl; 
                classBody->nops++;
                pushSemanticStack(classBody);
                
            }   
            
            else {

                node* classBody = operatorNode(CLASSBODY_NODE, 0);
                classBody->value = "body";
                classBody->op = malloc(sizeof(node*));
                classBody->op[classBody->nops] = memberDecl;
                classBody->nops++;
                pushSemanticStack(classBody);
            }
            
            break;
        }

        case FPARAMS: {

            if (pendingNonTerminal->symbols[3].symbol ==  EMPTY && pendingNonTerminal->symbols[4].symbol ==  EMPTY){

                node* type = popSemanticStack();
                type->type = TYPE_NODE;
                node* id = popSemanticStack();
                node* fparam = operatorNode(FPARAM_NODE, 1, type);
                fparam->value = id->value;
                fparam->col_no = id->col_no;
                fparam->line_no = id->line_no;

                node* fparams = operatorNode(FPARAMS_NODE, 1, fparam);
                fparams->value = "fparams";

                pushSemanticStack(fparams);
            } 
            
            else if (pendingNonTerminal->symbols[3].symbol !=  EMPTY && pendingNonTerminal->symbols[4].symbol !=  EMPTY){

                node* fparams = popSemanticStack();
                node* arraySize = popSemanticStack();
                node* type = popSemanticStack();
                node* id = popSemanticStack();

                type->type = TYPE_NODE;
                node* fparam = operatorNode(FPARAM_NODE, 2, type, arraySize);
                fparam->value = id->value;
                fparam->col_no = id->col_no;
                fparam->line_no = id->line_no;

                fparams->op = realloc(fparams->op, sizeof(node*) * (fparams->nops + 1));
                reverseChildrenNodesOrder(fparams);
                fparams->op[fparams->nops] = fparam; 
                fparams->nops++;
                pushSemanticStack(fparams);
            } 
            
            else if (pendingNonTerminal->symbols[3].symbol ==  EMPTY){

                node* fparams = popSemanticStack();
                node* type = popSemanticStack();
                node* id = popSemanticStack();
                type->type = TYPE_NODE;
                node* fparam = operatorNode(FPARAM_NODE, 1, type);
                fparam->value = id->value;
                fparam->col_no = id->col_no;
                fparam->line_no = id->line_no;

                fparams->op = realloc(fparams->op, sizeof(node*) * (fparams->nops + 1));
                reverseChildrenNodesOrder(fparams);
                fparams->op[fparams->nops] = fparam; 
                fparams->nops++;
                pushSemanticStack(fparams);
            } 
            
            else if (pendingNonTerminal->symbols[4].symbol ==  EMPTY){

                node* arraySize = popSemanticStack();
                node* type = popSemanticStack();
                node* id = popSemanticStack();
                type->type = TYPE_NODE;
                node* fparam = operatorNode(FPARAM_NODE, 2, type, arraySize);
                fparam->value = id->value;
                fparam->col_no = id->col_no;
                fparam->line_no = id->line_no;

                node* fparams = operatorNode(FPARAMS_NODE, 1, fparam);
                fparams->value = "fparams";

                pushSemanticStack(fparams);

            }

            break;
        }

        case FPARAMSTAIL: {

            if (pendingNonTerminal->symbols[4].symbol ==  EMPTY){

                node* topNode = semanticStack[semTop - 3];

                if (topNode->type == FPARAMS_NODE){

                    node* type = popSemanticStack();
                    node* id = popSemanticStack();
                    node* fparams = popSemanticStack();
                    type->type = TYPE_NODE;
                    node* fparam = operatorNode(FPARAM_NODE, 1, type);
                    fparam->value = id->value;
                    fparam->col_no = id->col_no;
                    fparam->line_no = id->line_no;

                    fparams->op = realloc(fparams->op, sizeof(node*) * (fparams->nops + 1));
                    fparams->op[fparams->nops] = fparam; 
                    fparams->nops++;
                    
                    pushSemanticStack(fparams);
                } 
                
                else {

                    node* type = popSemanticStack();
                    node* id = popSemanticStack();
                    type->type = TYPE_NODE;
                    node* fparam = operatorNode(FPARAM_NODE, 1, type);
                    fparam->value = id->value;
                    fparam->col_no = id->col_no;
                    fparam->line_no = id->line_no;

                    node* fparams = operatorNode(FPARAMS_NODE, 1, fparam);
                    fparams->value = "fparams";

                    pushSemanticStack(fparams);

                }
            } 

            else {

                node* topNode = semanticStack[semTop - 4];

                if (topNode->type == FPARAMS_NODE){

                    node* arraySize = popSemanticStack();
                    node* type = popSemanticStack();
                    node* id = popSemanticStack();
                    node* fparams = popSemanticStack();
                    type->type = TYPE_NODE;
                    node* fparam = operatorNode(FPARAM_NODE, 2, type, arraySize);
                    fparam->value = id->value;
                    fparam->col_no = id->col_no;
                    fparam->line_no = id->line_no;

                    fparams->op = realloc(fparams->op, sizeof(node*) * (fparams->nops + 1));
                    fparams->op[fparams->nops] = fparam; 
                    fparams->nops++;
                    pushSemanticStack(fparams);
                } 
                
                else {

                    node* arraySize = popSemanticStack();
                    node* type = popSemanticStack();
                    node* id = popSemanticStack();
                    type->type = TYPE_NODE;
                    node* fparam = operatorNode(FPARAM_NODE, 2, type, arraySize);
                    fparam->value = id->value;
                    fparam->col_no = id->col_no;
                    fparam->line_no = id->line_no;  
        
                    node* fparams = operatorNode(FPARAMS_NODE, 1, fparam);
                    fparams->value = "fparams";
        
                    pushSemanticStack(fparams);
                }
            }

            break;
        }

        case ARRAYSIZE_CONT: {

            if (pendingNonTerminal->symbols[0].symbol == INT_LIT){
                
                node* literal = popSemanticStack();
                literal->type = ARRAY_NODE;

                node* topNode = semanticStack[semTop - 1];

                if (topNode->type == ARRAYSIZE_NODE){

                    node* arraySize = popSemanticStack();
                    arraySize->op = realloc(arraySize->op, sizeof(node*) * (arraySize->nops + 1));
                    arraySize->op[arraySize->nops] = literal; 
                    arraySize->nops++;

                    pushSemanticStack(arraySize);
                }  
                
                else {

                    node* arraySize = operatorNode(ARRAYSIZE_NODE, 1, literal);
                    arraySize->value = "arrayList";

                    pushSemanticStack(arraySize);
                }
            } 
            
            else if (pendingNonTerminal->symbols[0].symbol == R_BRACKET){

                node* topNode = semanticStack[semTop - 1];

                node* emptyArrayNode = createLeafNode("emptyArray", ARRAY_NODE, -1, -1);

                if (topNode->type == ARRAYSIZE_NODE){
                    
                    node* arraySize = popSemanticStack();
                    arraySize->op = realloc(arraySize->op, sizeof(node*) * (arraySize->nops + 1));
                    arraySize->op[arraySize->nops] = emptyArrayNode; 
                    arraySize->nops++;

                    pushSemanticStack(arraySize);
                } 
                
                else {

                    node* arraySize = operatorNode(ARRAYSIZE_NODE, 1, emptyArrayNode);
                    arraySize->value = "arrayList";

                    pushSemanticStack(arraySize);
                }
            }
            
            break;
        }

        case STATEMENT: {

            int symbol = pendingNonTerminal->symbols[0].symbol;

            switch (symbol){

                case READ: {

                    node* variable = popSemanticStack();
                    variable->type = READ_NODE;
                    pushSemanticStack(variable);

                    break;
                }
                    
                case WRITE:{

                    node* expr = popSemanticStack();

                    if (expr->type == ID_NODE){
                        expr->type = VARIABLECALL_NODE;
                    }
                    
                    node* writeNode = operatorNode(WRITE_NODE, 1 ,expr);
                    pushSemanticStack(writeNode);

                    break;
                }

                case RETURN:{

                    node* expr = popSemanticStack();

                    if (expr->type == ID_NODE){
                        expr->type = VARIABLECALL_NODE;
                    }
                    
                    node* writeNode = operatorNode(RETURN_NODE, 1 ,expr);
                    pushSemanticStack(writeNode);

                    break;
                }

                case ASSIGNSTATORFUNCCALL: {

                    node* topNode = semanticStack[semTop - 1];

                    if (topNode->type != FUNCTIONCALL_NODE){
                        
                        node* assignment = popSemanticStack(); 
                        node* assignTo = popSemanticStack(); 

                        if (assignment->type == ID_NODE){
                            assignment->type = VARIABLECALL_NODE;
                        }
                        
                        if (assignTo->type == ID_NODE){
                            assignTo->type = VARIABLECALL_NODE;
                        }       

                        node* assignOp = operatorNode(ASSIGN_NODE, 2, assignTo, assignment);
                        assignOp->value = "=";

                        pushSemanticStack(assignOp);
                    }

                    break;
                }

                case WHILE: {

                    node* topNode = semanticStack[semTop - 1];

                    if (topNode->type != RELOP_NODE ){

                        node* statBlock = popSemanticStack(); 
                        node* relOp = popSemanticStack(); 

                        node* assignOp = operatorNode(WHILE_NODE, 2, relOp, statBlock);

                        pushSemanticStack(assignOp);
                    }

                    else {

                        node* relOp = popSemanticStack(); 
                        node* assignOp = operatorNode(WHILE_NODE, 1, relOp);

                        pushSemanticStack(assignOp);
                    }

                    break;
                }

                case IF: {

                    node* elseBlock = popSemanticStack();
                    node* thenBlock = popSemanticStack();
                    node* relOp = popSemanticStack();

                    if (elseBlock->type == EMPTY_NODE && thenBlock->type == EMPTY_NODE) {

                        node* ifNode = operatorNode(IF_NODE, 1, relOp);
                        pushSemanticStack(ifNode);
                    }
                    
                    else if (elseBlock->type == EMPTY_NODE){

                        thenBlock->type = THEN_NODE;
                        reverseChildrenNodesOrder(thenBlock);

                        node* ifNode = operatorNode(IF_NODE, 2, relOp, thenBlock);
                        pushSemanticStack(ifNode);
                    }

                    else if(thenBlock->type == EMPTY_NODE){

                        elseBlock->type = ELSE_NODE;

                        reverseChildrenNodesOrder(elseBlock);

                        node* ifNode = operatorNode(IF_NODE, 2, relOp, elseBlock);
                        pushSemanticStack(ifNode);
                    }

                    else {

                        elseBlock->type = ELSE_NODE;
                        thenBlock->type = THEN_NODE;

                        reverseChildrenNodesOrder(elseBlock);
                        reverseChildrenNodesOrder(thenBlock);

                        node* ifNode = operatorNode(IF_NODE, 3, relOp, thenBlock, elseBlock);

                        pushSemanticStack(ifNode);
                    }
                }

                default:
                    break;
            }

            break;
        }

        case VARIABLE: {

            if (pendingNonTerminal->symbols[1].symbol == EMPTY){

                node* id = popSemanticStack();
                id->type = VARIABLE_NODE;
                pushSemanticStack(id);
                
            } 
            
            else {
                
                node* arithExpr = popSemanticStack();
                node* id = popSemanticStack();
                id->type = VARIABLE_NODE;

                if (arithExpr->type == FUNCTIONCALL_NODE){
                    arithExpr->type = FUNCTIONCALL_NODE;
                }
                else if (arithExpr->type == ID_NODE || arithExpr->type == VARIABLECALL_NODE) {
                    arithExpr->type = VARIABLECALL_NODE;
                }
                else if (arithExpr->type == INDICELIST_NODE){
                    
                    if (arithExpr->op[0]->type == LITERAL_NODE){
                        arithExpr->type = ARRAYSIZE_NODE;
                        reverseChildrenNodesOrder(arithExpr);
                    }
                }
                else {
                    arithExpr->type = ARITHEXPR_NODE;
                }

                id->op = realloc(id->op, sizeof(node*) * (id->nops + 1));
                id->op[id->nops] = arithExpr; 
                id->nops++;

                pushSemanticStack(id);

            }

            break;
        }

        case ARITHEXPR: {

            node* topNode = semanticStack[semTop - 1];

            if (pendingNonTerminal->symbols[1].symbol != EMPTY){

                if (topNode->nops == 1){
                    
                    node* arithExpr = popSemanticStack();
                    node* leftOperend = popSemanticStack();

                    if (leftOperend->type == ID_NODE){
                        leftOperend->type = VARIABLECALL_NODE;
                    }

                    arithExpr->op = realloc(arithExpr->op, sizeof(node*) * (arithExpr->nops + 1));
                    arithExpr->op[arithExpr->nops] = leftOperend; 
                    arithExpr->nops++;

                    reverseChildrenNodesOrder(arithExpr);
                    pushSemanticStack(arithExpr);

                }
            } 

            break;
        }

        case ARITHOPERATIONS: {

            node* topNode = semanticStack[semTop - 1];

            if (topNode->type == ARITHEXPR_NODE){

                if (topNode->nops == 2){

                    node* rightOperator = popSemanticStack();
                    node* operator = popSemanticStack();
                    node* leftOperend = popSemanticStack();

                    operator->op = realloc(operator->op, sizeof(node*) * (operator->nops + 1));
                    operator->op[operator->nops] = leftOperend; 
                    operator->nops++;
                    
                    operator->op = realloc(operator->op, sizeof(node*) * (operator->nops + 1));
                    operator->op[operator->nops] = rightOperator; 
                    operator->nops++;
                    
                    reverseChildrenNodesOrder(rightOperator);
                    pushSemanticStack(operator);
                }
                
                else {

                    node* rightOperator = popSemanticStack();
                    node* leftOperend = popSemanticStack();
                    node* operator = popSemanticStack();

                    rightOperator->op = realloc(rightOperator->op, sizeof(node*) * (rightOperator->nops + 1));
                    rightOperator->op[rightOperator->nops] = leftOperend; 
                    rightOperator->nops++;

                    reverseChildrenNodesOrder(rightOperator);

                    operator->op = realloc(operator->op, sizeof(node*) * (operator->nops + 1));
                    operator->op[operator->nops] = rightOperator; 
                    operator->nops++;

                    pushSemanticStack(operator);
                }
            } 

            else {

                node* rightOperend = popSemanticStack();
                node* operator = popSemanticStack();

                if (rightOperend->type == ID_NODE){
                    rightOperend->type = VARIABLECALL_NODE;
                }
                    
                operator->op = realloc(operator->op, sizeof(node*) * (operator->nops + 1));
                operator->op[operator->nops] = rightOperend; 
                operator->nops++;

                pushSemanticStack(operator);
            }

            break;
        }

        case TERM: {

            node* topNode = semanticStack[semTop - 1];

            if (pendingNonTerminal->symbols[1].symbol != EMPTY){

                if (topNode->nops == 1){

                    node* arithExpr = popSemanticStack();
                    node* leftOperend = popSemanticStack();

                    arithExpr->op = realloc(arithExpr->op, sizeof(node*) * (arithExpr->nops + 1));
                    arithExpr->op[arithExpr->nops] = leftOperend; 
                    arithExpr->nops++;

                    reverseChildrenNodesOrder(arithExpr);
                    pushSemanticStack(arithExpr);
                } 

                else {

                    node* rightOperend = popSemanticStack();
                    node* arithExpr = popSemanticStack();
                    node* leftOperend = popSemanticStack();

                    arithExpr->op = realloc(arithExpr->op, sizeof(node*) * (arithExpr->nops + 1));
                    arithExpr->op[arithExpr->nops] = leftOperend; 
                    arithExpr->nops++;

                    arithExpr->op = realloc(arithExpr->op, sizeof(node*) * (arithExpr->nops + 1));
                    arithExpr->op[arithExpr->nops] = rightOperend; 
                    arithExpr->nops++;

                    pushSemanticStack(arithExpr);
                }
            } 

            break;
        }

        case TERMOPERATIONS: {

            node* topNode = semanticStack[semTop - 1];

            if (topNode->type == ARITHEXPR_NODE){
               
                if (topNode->nops == 1){

                    node* rightOperator = popSemanticStack();
                    node* leftOperend = popSemanticStack();
                    node* operator = popSemanticStack();

                    rightOperator->op = realloc(rightOperator->op, sizeof(node*) * (rightOperator->nops + 1));
                    rightOperator->op[rightOperator->nops] = leftOperend; 
                    rightOperator->nops++;

                    reverseChildrenNodesOrder(rightOperator);

                    operator->op = realloc(operator->op, sizeof(node*) * (operator->nops + 1));
                    operator->op[operator->nops] = rightOperator; 
                    operator->nops++;

                    pushSemanticStack(operator);
                }
            } 

            else {

                node* rightOperend = popSemanticStack();
                node* operator = popSemanticStack();

                operator->op = realloc(operator->op, sizeof(node*) * (operator->nops + 1));
                operator->op[operator->nops] = rightOperend; 
                operator->nops++;

                pushSemanticStack(operator);

            }

            break;
        }

        case VARIABLEORFUNCCALL_CONT: {

            if (pendingNonTerminal->symbols[0].symbol == L_PARAN){

                if (pendingNonTerminal->symbols[1].symbol == EMPTY){
                    
                    node* id = popSemanticStack();
                    id->type = FUNCTIONCALL_NODE;
                    pushSemanticStack(id);
                }
            }
            
            else if (pendingNonTerminal->symbols[0].symbol == INDICE_LIST) {

                node* indiceList = popSemanticStack();
                node* id = popSemanticStack();

                indiceList->type = VARIABLECALL_NODE;
                indiceList->value = id->value;

                pushSemanticStack(indiceList);
            }

            break;
        }

        case APARAMS: {

            if (pendingNonTerminal->symbols[1].symbol == EMPTY){

                node* literal = popSemanticStack();
                node* id = popSemanticStack();

                id->type = FUNCTIONCALL_NODE;
                literal->type = APARAM_NODE;

                id->op = realloc(id->op, sizeof(node*) * (id->nops + 1));
                id->op[id->nops] = literal; 
                id->nops++;

                pushSemanticStack(id);

            }

            else {

                node* aParams = popSemanticStack();
                node* literal = popSemanticStack();
                node* id = popSemanticStack();

                reverseChildrenNodesOrder(aParams);
                aParams->type = FUNCTIONCALL_NODE;
                aParams->value = id->value;

                literal->type = APARAM_NODE;

                aParams->op = realloc(aParams->op, sizeof(node*) * (aParams->nops + 1));
                aParams->op[aParams->nops] = literal; 
                aParams->nops++;
                reverseChildrenNodesOrder(aParams);


                pushSemanticStack(aParams);

            }

            break;
        }

        case APARAMSTAIL: {

            node* topNode = semanticStack[semTop - 2];

            if (topNode->type == APARAMS_NODE){

                node* param = popSemanticStack();
                param->type = APARAM_NODE;
                node* aParamsNode = popSemanticStack();

                aParamsNode->op = realloc(aParamsNode->op, sizeof(node*) * (aParamsNode->nops + 1));
                aParamsNode->op[aParamsNode->nops] = param; 
                aParamsNode->nops++;

                pushSemanticStack(aParamsNode);
            }
            
            else {

                node* param = popSemanticStack();
                param->type = APARAM_NODE;
                node* aParamsNode = operatorNode(APARAMS_NODE, 1, param);

                pushSemanticStack(aParamsNode);
            }

            break;
        }

        case INDICE_LIST: {

            node* topNode = semanticStack[semTop - 1];

            if(topNode->type == FUNCTIONCALL_NODE || topNode->type == ARITHEXPR_NODE) return;

            if (topNode->type == INDICELIST_NODE){

                    node* indiceList = popSemanticStack(); 
                    node* arithExpr = popSemanticStack(); 

                    if (arithExpr->type == ID_NODE){
                        arithExpr->type = VARIABLECALL_NODE;
                    }
                    
                    reverseChildrenNodesOrder(arithExpr);
                    indiceList->op = realloc(indiceList->op, sizeof(node*) * (indiceList->nops + 1));
                    indiceList->op[indiceList->nops] = arithExpr; 
                    indiceList->nops++;

                    pushSemanticStack(indiceList);
            }

            else if (topNode->type == VARIABLECALL_NODE) {

                node* variableCallNode = popSemanticStack(); 
                reverseChildrenNodesOrder(variableCallNode);
                pushSemanticStack(variableCallNode);
            }
            
            else {

                node* arithExpr = popSemanticStack(); 

                if (arithExpr->type == ID_NODE){
                    arithExpr->type = VARIABLECALL_NODE;
                }

                reverseChildrenNodesOrder(arithExpr);
                node* indiceList = operatorNode(INDICELIST_NODE, 0);
                indiceList->op = malloc(sizeof(node*));
                indiceList->op[indiceList->nops] = arithExpr;
                indiceList->nops++;
                pushSemanticStack(indiceList);
            }
            break;
        }

        case EXPR: {

            if (pendingNonTerminal->symbols[1].symbol != EMPTY){

                node* leftArithExpr = popSemanticStack();
                node* relOp = popSemanticStack();
                node* rightArithExpr = popSemanticStack();

                if (leftArithExpr->type == ID_NODE){
                    leftArithExpr->type = VARIABLECALL_NODE;
                }
                
                if (rightArithExpr->type == ID_NODE){
                    rightArithExpr->type = VARIABLECALL_NODE;
                }

                relOp->op = realloc(relOp->op, sizeof(node*) * (relOp->nops + 1));
                relOp->op[relOp->nops] = rightArithExpr; 
                relOp->nops++;
                
                relOp->op = realloc(relOp->op, sizeof(node*) * (relOp->nops + 1));
                relOp->op[relOp->nops] = leftArithExpr; 
                relOp->nops++;

                pushSemanticStack(relOp);

            } 

            break;
        }

        case RELEXPR: {

            node* rhs = popSemanticStack(); 
            node* relOp = popSemanticStack(); 
            node* lhs = popSemanticStack(); 

            if (lhs->type == ID_NODE){
                    lhs->type = VARIABLECALL_NODE;
            }
            
            if (rhs->type == ID_NODE){
                rhs->type = VARIABLECALL_NODE;
            }

            relOp->op = realloc(relOp->op, sizeof(node*) * (relOp->nops + 1));
            relOp->op[relOp->nops] = lhs; 
            relOp->nops++;
            
            relOp->op = realloc(relOp->op, sizeof(node*) * (relOp->nops + 1));
            relOp->op[relOp->nops] = rhs; 
            relOp->nops++;

            pushSemanticStack(relOp);
            

            break;
        }

        case ASSIGNSTATORFUNCCALL_CONT: {

            if (pendingNonTerminal->symbols[0].symbol == L_PARAN){

                if (pendingNonTerminal->symbols[1].symbol == EMPTY){

                    node* id = popSemanticStack();
                    id->type = FUNCTIONCALL_NODE;
                    pushSemanticStack(id);
                }
            }
            
            else {

                if (pendingNonTerminal->symbols[0].symbol == INDICE_LIST){

                    node* assignment = popSemanticStack();
                    node* indiceList = popSemanticStack();
                    node* id = popSemanticStack();
                    indiceList->type = VARIABLECALL_NODE;
                    indiceList->value = id->value;

                    reverseChildrenNodesOrder(indiceList);

                    pushSemanticStack(indiceList);
                    pushSemanticStack(assignment);

                }
            }

            break;
        }

        case STATEMENT_LIST: {

            node* topNode = semanticStack[semTop - 1];

            if (topNode->type == STATBLOCK_NODE){
                
                node* statBlock = popSemanticStack(); 
                node* statement = popSemanticStack(); 

                statBlock->op = realloc(statBlock->op, sizeof(node*) * (statBlock->nops + 1));
                statBlock->op[statBlock->nops] = statement; 
                statBlock->nops++;               
                
                pushSemanticStack(statBlock);
            }
            
            else {


                node* statement = popSemanticStack(); 
                node* statBlock = operatorNode(STATBLOCK_NODE, 1, statement);

                pushSemanticStack(statBlock);
            }
            

            break;
        }

        case STATBLOCK: {

            if (pendingNonTerminal->symbols[1].symbol == EMPTY){

                node* emptyNode = createLeafNode("empty", EMPTY_NODE, -1, -1);
                pushSemanticStack(emptyNode);
            }

            break;
        }

        default:
            break;
    }

}
