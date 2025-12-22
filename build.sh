#!/bin/sh

error_handle() {
    if [ $? -ne 0 ]; then
        echo "Exiting..."
        exit 1
    fi
}

#yacc compile
clear
echo "Compiling flex..."
flex -o "src/lexical_analyzer/lexical_analyzer(flex_generated).c" src/lexical_analyzer/lexical_analyzer.l
error_handle

#lexical_analyzer generator
echo "Generating executable..."

gcc \
-g -fsanitize=address\
 -I. -o build/compiler \
 \
 "src/lexical_analyzer/lexical_analyzer(flex_generated).c" \
 \
 src/syntax_analyzer/parse_table/parse_table.c \
 src/syntax_analyzer/parse_table/rules.c \
 src/syntax_analyzer/parse_table/first_follow.c \
 src/syntax_analyzer/parsing_stack/parsing_stack.c \
 src/syntax_analyzer/syntax_analyzer.c \
 src/syntax_analyzer/ast/ast.c \
 \
 src/semantic_analyzer/semantic_analyzer.c \
 src/symbol_table/symbol_table.c \
 src/symbol_table/symbol_table_printer.c \
  \
 src/intermediate_code_generator/code_generator.c \
 \
 src/utils/token_strings.c \
 src/utils/logger.c \
 \
 src/main.c

error_handle
echo "Done!"
