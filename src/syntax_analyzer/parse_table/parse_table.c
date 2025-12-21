#include <stdio.h>
#include <stdlib.h>

#include "../../lexical_analyzer/tokens.h"
#include "../../utils/logger.h"
#include "../../utils/token_strings.h"
#include "first_follow.h"
#include "parse_table.h"
#include "rules.h"

#define TRUE 0;
#define FALSE 0;

int parseTable[MAX_NONTERMINALS][MAX_TERMINALS];
int currentNonTerminal;
char *currentNonTerminalString;
Production currentProduction;
int currentProductionIndex;
int currentProductionFirstSymbol;
Rule rule;

void initParseTable(void) {

    s("Initializing Parse Table");

    //! populate parse table with initial value of (-1)
    for (int i = 0; i < MAX_NONTERMINALS; i++) {
        for (int j = 0; j < (MAX_TERMINALS); j++) {
            parseTable[i][j] = EMPTY_ENTRY;
        }
    }

    done();
}

void printParseTable(void) {

    p("\n\n\nParse table");

    //! Print header for terminal labels
    printf("%-30s", "");
    for (int terminal = TERMINALS_START; terminal < TERMINALS_END; terminal++) {
        printf("%-15s", getTerminalString(terminal));
    }
    printf("\n");

    //! Separator line; MAX_TERMINALS + 1 is because of the empty cell at the begining
    for (int terminal = 0; terminal <= MAX_TERMINALS + 1; terminal++) {
        printf("---------------");
    }
    printf("\n");

    //! Print row
    for (int currentNonTerminal = 0; currentNonTerminal < MAX_NONTERMINALS; currentNonTerminal++) {

        //! First column for the non terminal labels
        printf("%-30s", getNonTerminalString(currentNonTerminal));

        for (int terminal = 0; terminal < MAX_TERMINALS; terminal++) {

            //* Empty cell
            if (parseTable[currentNonTerminal][terminal] == -1) {
                printf("%-15s", "-");

                //* Rule index
            } else {
                printf("%-15d", parseTable[currentNonTerminal][terminal]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void printSet(const char *setName, int set[]) {

    printf("\n%s = { ", setName);

    for (int j = 1; j <= set[0]; j++) {
        printf("%s", getTerminalString(set[j]));
        if (j < set[0]) {
            printf(", ");
        } else {
            printf(" ");
        }
    }

    printf("} \n");
}

int addProductionToTerminalSet(int *firstOrFollowSet) {


    int epsilonFound = 0;

    printf("No. of elemets in the set: %d\n\n", firstOrFollowSet[0]);

    //! set is empty
    if (firstOrFollowSet[0] <= 0){
        printf("Set is empty\n");
        return 0;
    }
    

    //! go through all the terminals and add the production
    for (int j = 1; j <= firstOrFollowSet[0]; j++) {

        int terminal = firstOrFollowSet[j];

        if (terminal ==  EMPTY) {
            epsilonFound = 1;
            continue;
        }

        printf("#%d: Filling column %d: %-20s=> ", j, terminal - TERMINALS_START, getTerminalString(terminal));

        //! check for overlaps/conflicts
        if (parseTable[currentNonTerminal][terminal - TERMINALS_START] == -1) {
            parseTable[currentNonTerminal][terminal - TERMINALS_START] = currentProductionIndex;
            printf("Added [%s, %s] = %s", currentNonTerminalString, getTerminalString(terminal), currentProduction.grammar);
            done();

        } else {
            printf("Conflict detected for [%s, %s]", currentNonTerminalString, getTerminalString(terminal));
            fail();
            return 0;
        }
    }

    //! production rule produces ϵ, add follow set of the production
    if (epsilonFound) {

        printf("\n[ %s ] produces ε\n", currentProduction.grammar);

        int *followSet = getFollowSet(currentNonTerminal);

        if (!followSet) {
            printf("\nFollow set not found for NonTerminal: %s",currentNonTerminalString);
            fail();
            return 0;
        }

        printf("\nAdding [ %s ] production for Follow(%s) terminals", currentProduction.grammar, currentNonTerminalString);
        printSet("Follow set", followSet);

        int addProduction = addProductionToTerminalSet(followSet);
        if (!addProduction) return 0;
    }


};

Rule getRule() {

    for (int i = 0; i < RULES_COUNT; i++) {
        if (rules[i].lhs == currentNonTerminal) {
            printf("Found rule %s\n", getNonTerminalString(currentNonTerminal));
            return rules[i];
        }
    }

    //! if rule not found return empty rule
    Rule emptyRule = {.rhs = {
                              .productionsCount = 0,
                      }};

    return emptyRule;
}

void addUniqueSymbol(int *set, int symbol) {
    for (int i = 1; i <= set[0]; i++) {
        if (set[i] == symbol) {
            return;
        }
    }
    set[++set[0]] = symbol;
}

int *getProductionFirstSet() {

    //! productionFirstSet = [symbolCount, symbol1, symbol2, ....]
    int *productionFirstSet = malloc(sizeof(int) * MAX_TERMINALS);
    productionFirstSet[0] = 0;

    //! go to every symbol of the production to generate overall first set of the production
    for (int i = 0; i < currentProduction.productionSymbolCount; i++) {

        int symbol = currentProduction.productionSymbols[i];
        int epsilonFound = 0;

        if (symbol >= TERMINALS_START) {
            addUniqueSymbol(productionFirstSet, symbol);
            return productionFirstSet;
        }

        int *firstSet = getFirstSet(symbol);

        for (int j = 1; j <= firstSet[0]; j++) {
            
            int terminal = firstSet[j];

            if (terminal == EMPTY){
                epsilonFound = 1;
            } else {
                addUniqueSymbol(productionFirstSet, terminal);
            }
        }

        if (!epsilonFound) return productionFirstSet;
    }

    //! all the symbols can produce ε thus add the ε to the first set of the production
    addUniqueSymbol(productionFirstSet, EMPTY);
    return productionFirstSet;
}

int populateParserTableRow() {

    //! print header for each rule
    br();
    printf("Populating Row %d: %s\n\n", currentNonTerminal, currentNonTerminalString);

    //! Get the grammar rule for current NonTerminal
    rule = getRule();

    //! Exit if no gammer rule is found
    if (rule.rhs.productionsCount == 0) {
        char *currentNonTerminalString = getNonTerminalString(currentNonTerminal);
        printf("\nGrammar rule not found for NonTerminal: %s", currentNonTerminalString);
        fail();
        return 0;
    }

    printf("\n%d productions found in Rule: %s", rule.rhs.productionsCount, getNonTerminalString(rule.lhs));

    //! Iterate through each productions of current rule
    for (int i = 0; i < rule.rhs.productionsCount; i++) {

        currentProductionIndex = i;
        currentProduction = rule.rhs.productions[currentProductionIndex];
        currentProductionFirstSymbol = currentProduction.productionSymbols[0];

        printf("\n\nproduction #%d => [ %s ]\n", currentProductionIndex + 1, currentProduction.grammar);

       //! If production rule produces ϵ add follow set of the rule and go to the next currentProduction
        /* if (currentProductionFirstSymbol == EMPTY) {

            printf("\nRule %s produces ε\n", currentNonTerminalString);

            int *followSet = getFollowSet(currentNonTerminal);

            if (!followSet) {
                printf("\nFollow set not found for "
                       "NonTerminal: %s",
                       currentNonTerminalString);
                fail();
                return 0;
            }

            printSet("Follow set", followSet);
            printf("\nAdding currentProduction rule for Follow(%s) terminals\n\n",currentNonTerminalString);

            int addProduction = addProductionToTerminalSet(followSet);
            if (!addProduction) return 0;

            continue;
        } */

        //! get the first set of the production
        int *firstSet = getProductionFirstSet();
        if (!firstSet) return 0;

        printf("\nAdding [ %s ] production for First set terminals\n", currentProduction.grammar);
        printSet("First set", firstSet);

        //! go through all the terminals in the first set and add the production to the parse table
        int addProduction = addProductionToTerminalSet(firstSet);
        if (!addProduction) return 0;
        free(firstSet);



















        //! rule dont produces ϵ so get the first set of the current currentProduction
        /*
        int *firstSet = getFirstSet(currentProductionFirstSymbol);

        if (!firstSet) {
            printf("\nFirst set not found for "
                   "NonTerminal: %s",
                   getNonTerminalString(currentProductionFirstSymbol));
            fail();
            return 0;
        }

        //! Add currentProduction to the parse table for each terminal in first set
        printf("\nAdding currentProduction rule for "
               "First(%s) terminals\n\n",
               getNonTerminalString(currentProductionFirstSymbol));

        //! go through all the terminals in the first set of currentProductionFirstSymbol
        int addProduction = addProductionToTerminalSet(firstSet);
        if (!addProduction)
            return 0;

        //! If currentProductionContainsEpsilon triggered meaning empty/ε in first set so add follow set of the rule


        //!TODO meka waradi first set eka hoyanna oni mulu production ekema nathuwa currentProductionFirstSymbol newei so getFirstSet(currentProductionFirstSymbol)
        //!TODO okawenaskaranna  getFirstSetOfCurrentProduction kyl ekemea epsilon check karanna ethiokota if (currentProductionContainsEpsilon) oni wenne na.
        
        if (currentProductionContainsEpsilon) {


            printf("\nFound ε in First(%s)\n", getNonTerminalString(currentProductionFirstSymbol));

            int *followSet = getFollowSet(currentProductionFirstSymbol);
            if (!followSet) {
                printf("\nFollow set not found for "
                       "NonTerminal: %s",
                       currentNonTerminalString);
                fail();
                return 0;
            }

            printf("\nAdding currentProduction rule "
                   "for Follow(%s) terminals\n\n",
                   getNonTerminalString(currentProductionFirstSymbol));

            int addProduction = addProductionToTerminalSet(followSet);
            if (!addProduction)
                return 0;
        }
    */
    }


    return 1;
}

void populateParseTable(void) {

    s("Populating Parse Table");

    //! Fill the parse table with grammar rules
    for (int i = 0; i < MAX_NONTERMINALS; i++) {

        currentNonTerminal = i;
        currentNonTerminalString = getNonTerminalString(i);

        if (!populateParserTableRow()) {

            s("Populating Parse Table");
            fail();
            break;
        }
    }

    s("Populating Parse Table");
    done();
    printParseTable();
    /*


        populateParserTableRows(56);
        printf("%s",
       getNonTerminalString(rules[parseTable[56][14]].lhs));
        56 -> 13 14 23;



    */
}

Production *getProduction(int NonTerminal, int terminal) {

    if (parseTable[NonTerminal][terminal] == -1) {
        return NULL;
    }

    return &rules[NonTerminal].rhs.productions[parseTable[NonTerminal][terminal]];
}
