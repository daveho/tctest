/*
 * Demo functions (implementation)
 */

#include <stdlib.h>
#include "demofn.h"

Stack *stackCreate(void) {
	Stack *s = malloc(sizeof(Stack));
	s->count = 0;
	return s;
}

bool stackPush(Stack *s, int val) {
	/* NOTE: has bug */
	if (s->count > STACK_MAX) { return false; }
	s->values[s->count++] = val;
	return true;
}

bool stackPop(Stack *s, int *pVal) {
	if (stackIsEmpty(s)) { return false; }
	*pVal = s->values[--s->count];
	return true;
}

bool stackIsEmpty(Stack *s) {
	return s->count == 0;
}

bool stackSwapTopElts(Stack *s) {
	/* NOTE: has bug causing a segfault */
	int *tmp = NULL;
	if (s->count < 2) { return false; }
	*tmp = s->values[s->count - 1];
	s->values[s->count - 1] = s->values[s->count - 2];
	s->values[s->count - 2] = *tmp;
	return true;
}

bool stackSizeIsEven(Stack *s) {
	/* NOTE: has bug causing floating-point exception (divide by 0) */
	int x = 0;
	return s->count % x == 2;
}

void stackDestroy(Stack *s) {
	free(s);
}
