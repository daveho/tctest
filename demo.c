/*
 * Demo test program: this is a pretty reasonable example
 * of how to write a test program using TCTest
 */

#include <stdlib.h>
#include "tctest.h"
#include "demofn.h"

/*
 * An instance of the TestObjs data type contains the
 * test fixture available to each test function.
 */
typedef struct {
	Stack *s;
} TestObjs;

/* The setup function creates the test fixture. */
TestObjs *setup();

/* The cleanup function cleans up the test fixture. */
void cleanup(TestObjs *objs);

/* Each test function gets a pointer to the test fixture object. */
void testPush(TestObjs *objs);
void testPushMany(TestObjs *objs);
void testSwapTopElts(TestObjs *objs);

int main(void) {
	/* Prepare to run tests */
	TEST_INIT();

	/* Execute test functions */
	TEST(testPush);
	TEST(testPushMany);
	TEST(testSwapTopElts);

	/*
	 * Report results: exits with nonzero exit code if
	 * any test failed
	 */
	TEST_FINI();
}

TestObjs *setup() {
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->s = stackCreate();
	return objs;
}

void cleanup(TestObjs *objs) {
	stackDestroy(objs->s);
	free(objs);
}

void testPush(TestObjs *objs) {
	int x;

	ASSERT(stackIsEmpty(objs->s));
	ASSERT(stackPush(objs->s, 1));
	ASSERT(stackPush(objs->s, 2));
	ASSERT(stackPush(objs->s, 3));

	ASSERT(stackPop(objs->s, &x));
	ASSERT(3 == x);
	ASSERT(stackPop(objs->s, &x));
	ASSERT(2 == x);
	ASSERT(stackPop(objs->s, &x));
	ASSERT(1 == x);
}

void testPushMany(TestObjs *objs) {
	for (int i = 1; i < STACK_MAX; i++) {
		ASSERT(stackPush(objs->s, i));
	}

	/* stack should be full at this point */
	ASSERT(!stackPush(objs->s, 11));
}

void testSwapTopElts(TestObjs *objs) {
	int x;

	ASSERT(stackPush(objs->s, 1));
	ASSERT(stackPush(objs->s, 2));
	ASSERT(stackPush(objs->s, 3));

	ASSERT(stackSwapTopElts(objs->s));

	ASSERT(stackPop(objs->s, &x));
	ASSERT(2 == x);
	ASSERT(stackPop(objs->s, &x));
	ASSERT(3 == x);
}
