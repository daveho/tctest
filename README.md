# TCTest — a tiny unit test framework for C

TCTest is a tiny unit test framework for C.  It consists of two files [tctest.h](tctest.h) and [tctest.c](tctest.c).  It is distributed under the [MIT License](https://opensource.org/licenses/MIT).

The demo program [demo.c](demo.c) shows how to write a test program using TCTest.  The expected output of the demo program is

```
testPush...passed!
testPushMany...failed assertion !stackPush(objs->s, 11) at line 78
testSwapTopElts...segmentation fault (most recent assertion at line 88)
2 test(s) failed
```

Send comments to [david.hovemeyer@gmail.com](mailto:david.hovemeyer@gmail.com).
