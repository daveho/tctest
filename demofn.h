/*
 * Demo functions (header)
 */

#ifndef DEMOFN_H
#define DEMOFN_H

#include <stdbool.h>

#define STACK_MAX 10

typedef struct { int values[STACK_MAX]; int count; } Stack;

Stack *stackCreate(void);
bool stackPush(Stack *s, int val);
bool stackPop(Stack *s, int *pVal);
bool stackIsEmpty(Stack *s);
bool stackSwapTopElts(Stack *s);
void stackDestroy(Stack *s);

#endif /* DEMOFN_H */
