// Demo program illustrating how to test C++ code
// using tctest

#include <string>
#include "demofn_cplusplus.h"
#include "tctest.h"

struct TestObjs {
	std::string s1;
	std::string s2;
	std::string s3;
};

TestObjs *setup();
void cleanup(TestObjs *objs);

void test_convert_to_lower_1(TestObjs *objs);
void test_convert_to_lower_2(TestObjs *objs);
void test_convert_to_lower_3(TestObjs *objs);

int main(int argc, char **argv) {
	if (argc > 1) {
		tctest_testname_to_execute = argv[1];
	}

	TEST_INIT();

	TEST(test_convert_to_lower_1);
	TEST(test_convert_to_lower_2);
	TEST(test_convert_to_lower_3);

	TEST_FINI();
}

TestObjs *setup() {
	TestObjs *objs = new TestObjs;

	objs->s1 = "foo bar";
	objs->s2 = "Hello, world";
	objs->s3 = "Peanut Butter and Jelly";

	return objs;
}

void cleanup(TestObjs *objs) {
	delete objs;
}

void test_convert_to_lower_1(TestObjs *objs) {
	convert_to_lower(objs->s1);
	ASSERT("foo bar" == objs->s1);
}

void test_convert_to_lower_2(TestObjs *objs) {
	convert_to_lower(objs->s2);
	ASSERT("hello, world" == objs->s2);
}

void test_convert_to_lower_3(TestObjs *objs) {
	convert_to_lower(objs->s3);
	ASSERT("peanut butter and jelly" == objs->s3);
}
