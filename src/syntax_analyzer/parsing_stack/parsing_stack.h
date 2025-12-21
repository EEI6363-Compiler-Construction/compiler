#ifndef PARSING_STACK_H
#define PARSING_STACK_H

#define STACK_SIZE 100

extern int parsingStack[STACK_SIZE]; 
extern int top;

void push(int entry);
int pop();

#endif