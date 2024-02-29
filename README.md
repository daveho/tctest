# TCTest — a tiny unit test framework for C and C++

TCTest is a tiny unit test framework for C and C++.  It consists of two
files [tctest.h](tctest.h) and [tctest.c](tctest.c).  It is distributed
under the [MIT License](https://opensource.org/licenses/MIT).  It has been
developed and tested using Linux, but should work on other Posix-compliant
operating systems.

Send comments to [david.hovemeyer@gmail.com](mailto:david.hovemeyer@gmail.com).

## Concepts

TCTest is modeled on [JUnit 4](https://junit.org/junit4/).

The test program should define a `TestObjs` data type (typically a
typedef for a struct type.)  An instance of `TestObjs` is the test
fixture, i.e., the objects/data being tested.

The test program should define `setup` and `cleanup` functions.
The `setup` function creates a new instance of `TestObjs`, and is run
automatically before each test.  The `cleanup` function destroys an
instance of `TestObjs`, and is run automatically after each test.

Test functions take a pointer to an instance of `TestObjs` as a parameter.
They use the `ASSERT` macro to make assertions about expected behavior.
TCTest installs a signal handler for common runtime exceptions such
as segfault and floating-point exception, and considers these as test
failures.

Test functions are executed using the `TEST` macro.  All invocations of
the `TEST` macro should be surrounded by calls to the `TEST_INIT` and
`TEST_FINI` macros.  *Important*: the test program must call `TEST_INIT`,
`TEST`, and `TEST_FINI` from its `main` function.

## Limitations

Because C and C++ are memory-unsafe languages, the execution of test
functions can't be made truly independent from each other, and it is
possible that the behavior of a later test function could be affected
by the invocation of an earlier one.  To make testing more robust, you
can execute a separate test program process for each test function.
See the description of `tctest_testname_to_execute` in the [advanced
features](#advanced-features) section, and also the demo programs
([demo.c](demo.c) and [demo\_cplusplus.cpp](demo_cplusplus.cpp).)

Support for C++ is fairly new (added November 5th, 2023.)
However, it appears to work reasonably well.

## Demo program

The demo programs ([demo.c](demo.c) and
[demo\_cplusplus.cpp](demo_cplusplus.cpp)) show how to write a test
program using TCTest.

The expected output of the C demo program (`./demo`) is

```
testPush...passed!
testPushMany...failed ASSERT !stackPush(objs->s, 11) at line 132
testSwapTopElts...segmentation fault (most recent ASSERT at line 142)
testSizeIsEven...floating point exception (most recent ASSERT at line 154)
testSegfaultBeforeAssert...segmentation fault
4 test(s) failed
```

The expected output of the C++ demo program (`./demo_cplusplus`) is

```
test_convert_to_lower_1...passed!
test_convert_to_lower_2...failed ASSERT "hello, world" == objs->s2 at line 56
test_convert_to_lower_3...std::exception (what='J causes exception for some reason!')
2 test(s) failed
```

## Debugging

We have a couple of tips for using a debugger (e.g., `gdb`) to
debug when a unit test fails.

First, make sure your `main` function uses the `tctest_testname_to_execute`
trick so that a command line argument can be provided to control which
unit test is executed. (See the demo programs.) That way, your execution can
run just the unit test you want to debug.

You can set a breakpoint on the `tctest_fail()` function if you want to
gain control of the program on the first failed `ASSERT()` or
`FAIL()`.

## "Advanced" features

There are a few "advanced" features you might find useful.

The `FAIL` macro can be used to immediate fail the current test.
The macro argument is a message describing the failure.  Using this
macro is slightly nicer than doing something like `ASSERT(0)`.

If the `tctest_testname_to_execute` variable is set to point to a C
string, only the test function with the name specified by the string
will be executed.  This is useful for allowing the test program to
take a command line argument specifying the specific test to run.
[demo.c](demo.c) is set up this way.

If the `tctest_on_test_executed` function pointer is set to a non-null
value, the specified function will be called by the `TEST` macro after
each test function has been executed. The function will be passed the name
of the test (as a C string) and a boolean value indicating whether or not
the named test passed.  [demo.c](demo.c) shows how to use this feature.

If the `tctest_on_complete` function pointer is set to a non-null value,
the specified function will be called by `TEST_FINI` after all tests
have been executed.  This can be useful if the test program needs to do
some specialized reporting of test results.  [demo.c](demo.c) shows how
to use this feature.
