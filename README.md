# TCTest — a tiny unit test framework for C

TCTest is a tiny unit test framework for C.  It consists of two files [tctest.h](tctest.h) and [tctest.c](tctest.c).  It is distributed under the [MIT License](https://opensource.org/licenses/MIT).

Send comments to [david.hovemeyer@gmail.com](mailto:david.hovemeyer@gmail.com).

## Demo program

The demo program [demo.c](demo.c) shows how to write a test program using TCTest.  The expected output of the demo program is

```
testPush...passed!
testPushMany...failed assertion !stackPush(objs->s, 11) at line 78
testSwapTopElts...segmentation fault (most recent assertion at line 88)
2 test(s) failed
```

## "Advanced" features

There are a few "advanced" features you might find useful.

The `FAIL` macro can be used to immediate fail the current test.  The macro argument is a message describing the failure.  Using this macro is slightly nicer than doing something like `ASSERT(0)`.

If the `tctest_testname_to_execute` variable is set to point to a C string, only the test function with the name specified by the string will be executed.  This is useful for allowing the test program to take a command line argument specifying the specific test to run.  [demo.c](demo.c) is set up this way.

If the `tctest_on_test_executed` function pointer is set to a non-null value, the specified function will be called by the `TEST` macro after each test function has been executed. The function will be passed the name of the test (as a C string) and a boolean value indicating whether or not the named test passed.

If the `tctest_on_complete` function pointer is set to a non-null value, the specified function will be called by `TEST_FINI` after all tests have been executed.  This can be useful if the test program needs to do some specialized reporting of test results.  [demo.c](demo.c) shows how to use this feature.
