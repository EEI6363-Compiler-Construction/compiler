#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "../../utils/logger.h"

typedef struct RowInfo {
    char* identifier;
    char* symbolType;
    char* dataType;
    char* isArray;
    char* dimensions;
    char* size;
    char* offset;
    char* extraInfo;

} RowInfo;

RowInfo *createEmptyRowInfo(){

    RowInfo *info = malloc(sizeof(RowInfo));

    info->identifier = "-";
    info->symbolType = "-";
    info->dataType = "-";
    info->isArray = "-";
    info->dimensions = "-";
    info->size = "-";
    info->offset = "-";
    info->extraInfo = "-";
};

void formatArray(TypeInfo *typeInfo, char *buffer, int bufSize) {

    buffer[0] = '\0';

    for (int i = 0; i < typeInfo->size; i++) {
        char temp[32];
        snprintf(temp, sizeof(temp), "[%d]", typeInfo->dimensions[i]);
        strncat(buffer, temp, bufSize - strlen(buffer) - 1);
    }
}

RowInfo *getSymbolInfo(Symbol *symbol) {

    RowInfo *info = createEmptyRowInfo();

    if (!symbol) {
        return info;
    }

    info->identifier = symbol->identifier;
    info->symbolType = getSymbolTypeString(symbol->symbolType);

    if (symbol->typeInfo){
        
        info->dataType = getDataTypeString(symbol->typeInfo->dataType);
        
        if (symbol->typeInfo->isArray == 1){
            info->isArray = "YES";
        }
        
        if (symbol->typeInfo->numDimensions){
            char str[12];
            snprintf(str, sizeof(str), "%d", symbol->typeInfo->numDimensions);
            info->dimensions = strdup(str);
        }
        
        if (symbol->typeInfo->size != 0){
            char str[12];
            snprintf(str, sizeof(str), "%d", symbol->typeInfo->size);
            info->size = strdup(str);
        }
        
        if (symbol->typeInfo->offset != 0){
            char str[12];
            snprintf(str, sizeof(str), "%d", symbol->typeInfo->offset);
            info->offset = strdup(str);
        }
        
    }

    char extra[256] = {0};

    if (symbol->numIsa) {

        char isaPart[124] = {0};

        strncat(isaPart, "ISA -> ", sizeof(isaPart) - strlen(isaPart) - 1);

        for (int i = 0; i < symbol->numIsa; i++) {
            strncat(isaPart, symbol->isaList[i], sizeof(isaPart) - strlen(isaPart) - 1);
            if (i < symbol->numIsa - 1)
                strncat(isaPart, ", ", sizeof(isaPart) - strlen(isaPart) - 1);
        }

        strncat(extra, isaPart, sizeof(extra) - strlen(extra) - 1);
    }
    
    if (symbol->numParams) {
        
        if (strlen(extra) > 0) strncat(extra, "\n", sizeof(extra) - strlen(extra) - 1);

        char paramPart[124] = {0};

        strncat(paramPart, "Params -> ", sizeof(paramPart) - strlen(paramPart) - 1);


        for (int i = 0; i < symbol->numParams; i++) {
            strncat(paramPart, symbol->paramList[i]->identifier, sizeof(paramPart) - strlen(paramPart) - 1);

            strncat(paramPart, " (", sizeof(paramPart) - strlen(paramPart) - 1);

            char sizeStr[16];
            snprintf(sizeStr, sizeof(sizeStr), "%d", symbol->paramList[i]->typeInfo->size);

            strncat(paramPart, sizeStr, sizeof(paramPart) - strlen(paramPart) - 1);

            strncat(paramPart, ")", sizeof(paramPart) - strlen(paramPart) - 1);

            if (i < symbol->numParams - 1) strncat(paramPart, ", ", sizeof(paramPart) - strlen(paramPart) - 1);
        }


        strncat(extra, paramPart, sizeof(extra) - strlen(extra) - 1);
    }

    if (symbol->typeInfo && symbol->typeInfo->numDimensions) {

        char dimensionsPart[124] = {0};

        strncat(dimensionsPart, "Dimensions -> ", sizeof(dimensionsPart) - strlen(dimensionsPart) - 1);

        for (int i = 0; i < symbol->typeInfo->numDimensions; i++) {

            char temp[16];
            snprintf(temp, sizeof(temp), "%d", symbol->typeInfo->dimensions[i]);

            strncat(dimensionsPart, temp, sizeof(dimensionsPart) - strlen(dimensionsPart) - 1);

            if (i < symbol->typeInfo->numDimensions - 1) strncat(dimensionsPart, ", ", sizeof(dimensionsPart) - strlen(dimensionsPart) - 1);
        }

        strncat(extra, dimensionsPart, sizeof(extra) - strlen(extra) - 1);
    }

    info->extraInfo = strdup(extra);

    


/* 
    switch (typeInfo->dataType) {

        case INT_TYPE:{

            info[0] = strdup("INT_TYPE");
            info[1] = malloc(32);
            if (typeInfo->isSet)
            snprintf(info[1], 32, "%d", typeInfo->data.intValue);
            else
            strcpy(info[1], "-");

            break;
        }

        case FLOAT_TYPE:{

            info[0] = strdup("FLOAT_TYPE");
            info[1] = malloc(32);

            if (typeInfo->isSet) {
                snprintf(info[1], 32, "%f", typeInfo->data.floatValue);
            } else {
                strcpy(info[1], "-");
            }
         
            break;
        }

        case VARIABLE_TYPE: {

            char buffer[256];
            sprintf(buffer, "VARIABLE_TYPE (%s)", typeInfo->data.stringValue);
            info[0] = strdup(buffer);
            
            if (typeInfo->data.stringValue) {
                info[1] = strdup(typeInfo->data.stringValue);
            } else {
                info[1] = strdup("-");
            }
         
            break;
        }

        case INT_ARRAY_TYPE:

        case FLOAT_ARRAY_TYPE:

        case VARIABLE_ARRAY_TYPE: {

            char arrayStr[128];
            formatArray(typeInfo, arrayStr, sizeof(arrayStr));

            info[0] = malloc(128);
            info[1] = malloc(128);

            if (v->type == INT_ARRAY_TYPE) {
                snprintf(info[0], 128, "INT_ARRAY_TYPE %s", arrayStr);
            } else if (v->type == FLOAT_ARRAY_TYPE) {
                snprintf(info[0], 128, "FLOAT_ARRAY_TYPE  %s", arrayStr);
            } else {
                snprintf(info[0], 128, "VARIABLE_ARRAY_TYPE (%s)%s", v->data.stringValue, arrayStr);
            }
            
            if (v->isSet) {
                snprintf(info[1], 128, "%s", arrayStr);
            } else {
                strcpy(info[1], "-");
            }

            break;
        }

        default: {

            info[0] = strdup("-");
            info[1] = strdup("-");
         
            break;
        }
    } */

    return info;
}

void printSymbolTable(SymbolTable *table, int indent) {

    if (!table) return;

    for (int i = 0; i < table->symbolCount; i++) {

        Symbol *currentSymbol = table->symbols[i];

        RowInfo *info = getSymbolInfo(currentSymbol);
        
        printRow(
            indent * 8, 
            8,
            info->identifier,
            info->symbolType,
            info->dataType,
            info->isArray,
            info->dimensions,
            info->size,
            info->offset,
            info->extraInfo);
            
            if (currentSymbol->link != NULL) {
                int newIndent = indent + 1;
                printPointer(8 * newIndent, 2);
                printTableTitle(8 * newIndent, 8, currentSymbol->identifier, getSymbolTypeString(currentSymbol->symbolType));
                printHeader(8 * newIndent, 8, "Identifier", "symbol type", "data type", "is array", "dimensions", "size", "offset", "extra info");
                  
                printSymbolTable(currentSymbol->link, newIndent);
            }

    }
}

void printSymbolTableToFileRecursive(
    SymbolTable *table, 
    int indent, 
    FILE *outFile, 
    const char *tableName, 
    const char *symbolTypeStr
) {

    if (!table) return;


    printTableTitleToFile(outFile, indent, 8, tableName, symbolTypeStr);
    printHeaderToFile(outFile, indent, 8, "Identifier", "symbol type", "data type", "is array", "dimensions", "size", "offset", "extra info");

    for (int i = 0; i < table->symbolCount; i++) {
        Symbol *currentSymbol = table->symbols[i];
        RowInfo *info = getSymbolInfo(currentSymbol);

        printRowToFile(outFile,
                       indent,
                       8,
                       info->identifier,
                       info->symbolType,
                       info->dataType,
                       info->isArray,
                       info->dimensions,
                       info->size,
                       info->offset,
                       info->extraInfo);


        if (currentSymbol->link != NULL) {
            
            int newIndent = indent + 1;

            printPointerToFile(outFile, newIndent, 2);
            printSymbolTableToFileRecursive(currentSymbol->link, newIndent,outFile, currentSymbol->identifier, getSymbolTypeString(currentSymbol->symbolType));
        }
    }
}

void printSymbolTableToFile(SymbolTable *table, int indent) {
    if (!table) return;

    FILE *outFile = fopen("output/symbol_table.txt", "w");
    if (!outFile) {
        perror("Error opening file");
        return;
    }

    printSymbolTableToFileRecursive(table, indent, outFile, "Global Table", "Global");

    fclose(outFile);
}
