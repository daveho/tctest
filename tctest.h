/*
 * TCTest - a tiny unit test framework for C
 * Copyright (c) 2013,2019-2020 David H. Hovemeyer <david.hovemeyer@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef TCTEST_H
#define TCTEST_H

#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

extern sigjmp_buf tctest_env;
extern int tctest_assertion_line;
extern int tctest_failures;
extern int tctest_num_executed;
void tctest_register_signal_handlers(void);

/*
 * Setting this pointer to a non-null value will cause tctest to
 * only execute the test with the specified name (which must
 * exactly match the name of a test function.)  This is useful
 * for allowing the test driver to run a single test.
 */
extern const char *tctest_testname_to_execute;

/*
 * If this function pointer is set to a non-null value, it will
 * be called after a test has been executed.  The testname parameter
 * is the name of the test function.  The passed parameter will
 * be true (nonzero) if the test passed, and false (zero) if the
 * test did not pass.
 */
extern void (*tctest_on_test_executed)(const char *testname, int passed);

/*
 * If this function pointer is set to a non-null value, it will
 * be called after all tests have executed.  The parameters
 * are the number of tests passed and the total number of tests
 * executed (respectively.)
 */
extern void (*tctest_on_complete)(int num_passed, int num_executed);

#define TEST_INIT() do { \
	tctest_register_signal_handlers(); \
} while (0)

#define TEST(func) do { \
	if (!tctest_testname_to_execute || strcmp(tctest_testname_to_execute, #func) == 0) { \
		tctest_num_executed++; \
		tctest_assertion_line = -1; \
		if (sigsetjmp(tctest_env, 1) == 0) { \
			TestObjs *t = setup(); \
			printf("%s...", #func); \
			fflush(stdout); \
			func(t); \
			cleanup(t); \
			printf("passed!\n"); \
			if (tctest_on_test_executed) { \
				tctest_on_test_executed(#func, 1); \
			} \
		} else { \
			tctest_failures++; \
			if (tctest_on_test_executed) { \
				tctest_on_test_executed(#func, 0); \
			} \
		} \
	} \
} while (0)

#define ASSERT(cond) do { \
	tctest_assertion_line = __LINE__; \
	if (!(cond)) { \
		printf("failed ASSERT %s at line %d\n", #cond, __LINE__); \
		siglongjmp(tctest_env, 1); \
	} \
} while (0)

/*
 * Use this macro to unconditionally fail the current test with
 * specified error message.  This is somewhat nicer than doing
 * ASSERT(0).
 */
#define FAIL(msg) do { \
	printf("%s\n", msg); \
	siglongjmp(tctest_env, 1); \
} while (0)

#define TEST_FINI() do { \
	if (tctest_failures == 0) { \
		printf("All tests passed!\n"); \
	} else { \
		printf("%d test(s) failed\n", tctest_failures); \
	} \
	if (tctest_on_complete) { \
		tctest_on_complete(tctest_num_executed - tctest_failures, tctest_num_executed); \
	} \
	return tctest_failures > 0; \
} while (0)

#ifdef __cplusplus
}
#endif

#endif /* TCTEST_H */
