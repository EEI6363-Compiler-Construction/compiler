#include <stdio.h>

#include "syntax_analyzer/syntax_analyzer.h"
#include "utils/logger.h"
#include "semantic_analyzer/semantic_analyzer.h"

int main(int argc, char *argv[]) {



    if (argc < 2) {

        printf("\nMissing file, use: \n%s <input_txt_file>\n", argv[0]);
        return 1;
    }

    runParser(argv[1]);
    runSemanticAnalyzer();

    return 0;
}

