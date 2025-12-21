#include "parsing_stack.h"

int parsingStack[100]; 
int top = 0;

void push(int entry) { 
    parsingStack[top++] = entry; 
}

int pop() { 
    return parsingStack[--top]; 
}