/*
 * TCTest - a tiny unit test framework for C
 * Copyright (c) 2013,2019 David H. Hovemeyer <david.hovemeyer@gmail.com>
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

#include <signal.h>
#include <unistd.h>
#include "tctest.h"

typedef struct {
	int signum;
	const char *msg;
} tctest_signal;

static tctest_signal tctest_signal_list[] = {
	{ SIGFPE, "floating point exception" },
	{ SIGSEGV, "segmentation fault" },
	{ SIGBUS, "bus error" },
	{ SIGABRT, "abort (assert failed?)" },
	{ SIGTRAP, "trap" },
	{ SIGSYS, "bad system call" },
	{ -1, "unknown signal" }
};

sigjmp_buf tctest_env;
int tctest_assertion_line;
int tctest_failures;
int tctest_num_executed;
const char *tctest_testname_to_execute;
void (*tctest_on_test_executed)(const char *testname, int passed);
void (*tctest_on_complete)(int num_passed, int num_executed);

/*
 * Workaround for the stdio functions not being
 * async signal safe.
 */
static void tctest_print_signal_msg(const char *msg) {
	char buf[512];
	strcpy(buf, msg);
	strcat(buf, " (most recent ASSERT at line ");

	/* convert ASSERT line number to text */
	char stack[16];
	size_t ndig = 0;
	int val = tctest_assertion_line;
	do {
		stack[ndig++] = '0' + (val % 10);
		val /= 10;
	} while (val > 0);

	/* append text digits of ASSERT line number */
	size_t n = strlen(buf);
	while (ndig > 0) {
		buf[n++] = stack[--ndig];
	}

	/* append ')' and newline */
	buf[n++] = ')';
	buf[n++] = '\n';

	/* write to standard output */
	write(1, buf, n);
}

static void tctest_signal_handler(int signum, siginfo_t *info, void *addr) {
	/* shut up warnings about unused parameter(s) */
	(void)info;
	(void)addr;

	/* look up message describing signal */
	int i;
	const char *msg = NULL;
	for (i = 0; tctest_signal_list[i].signum != -1 ; i++) {
		if (signum == tctest_signal_list[i].signum) {
			msg = tctest_signal_list[i].msg;
			break;
		}
	}

	/* print message about failure */
	tctest_print_signal_msg(msg);

	/* jump back to TEST context */
	siglongjmp(tctest_env, 1);
}

void tctest_register_signal_handlers(void) {
	struct sigaction sa;
	int i;

	sa.sa_sigaction = &tctest_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	for (i = 0; tctest_signal_list[i].signum != -1; i++) {
		sigaction(tctest_signal_list[i].signum, &sa, NULL);
	}
}
