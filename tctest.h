// TCTest - a tiny unit test framework for C
// Copyright (c) 2013, David H. Hovemeyer <david.hovemeyer@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef TCTEST_H
#define TCTEST_H

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

extern sigjmp_buf tctest_env;
extern int tctest_assertion_line;
extern int tctest_failures;
void tctest_segfault_handler(int signum, siginfo_t *info, void *addr);

#define TEST_INIT() do { \
	struct sigaction sa; \
	sa.sa_sigaction = &tctest_segfault_handler; \
	sigemptyset(&sa.sa_mask); \
	sa.sa_flags = 0; \
	sigaction(SIGSEGV, &sa, NULL); \
} while (0)

#define TEST(func) do { \
	TestObjs *t = setup(); \
	if (sigsetjmp(tctest_env, 1) == 0) { \
		printf("%s...", #func); \
		fflush(stdout); \
		func(t); \
		printf("passed!\n"); \
	} else { \
		tctest_failures++; \
	} \
	cleanup(t); \
} while (0)

#define ASSERT(cond) do { \
	tctest_assertion_line = __LINE__; \
	if (!(cond)) { \
		printf("failed assertion %s at line %d\n", #cond, __LINE__); \
		siglongjmp(tctest_env, 1); \
	} \
} while (0)

#define TEST_FINI() do { \
	if (tctest_failures == 0) { \
		printf("All tests passed!\n"); \
	} else { \
		printf("%d test(s) failed\n", tctest_failures); \
	} \
	return tctest_failures > 0; \
} while (0)

#endif // TCTEST_H
