#include <lib_base.h>

int case_string_value() {
	struct string_t *s = string_init();
	string_push_char(s, 'h');
	string_push_char(s, 'e');
	string_push_char(s, 'l');
	string_push_char(s, 'l');
	string_push_char(s, 'o');

	ASSERT(!strcmp(string_to_str(s), "hello"));

	string_free(s);

	return TEST_SUCCESS;
}

int case_push_str() {
	struct string_t *s = string_init();
	string_push_str(s, "hello");

	ASSERT(!strcmp(string_to_str(s), "hello"));

	string_free(s);

	return TEST_SUCCESS;

}

struct suite_t*
string_suite() {
	struct suite_t *r = suite_init("string");

	CASE(r, string value, case_string_value);
	CASE(r, string push str, case_push_str);

	return r;
}

int case_format_int() {
	struct string_t *s = format("%d", 3);

	ASSERT(!strcmp(string_to_str(s), "3"));

	string_free(s);

	return TEST_SUCCESS;
}

int case_format_double() {
	struct string_t *s = format("%f", 3.3000);

	ASSERT(!strcmp(string_to_str(s), "3.300000"));

	string_free(s);

	return TEST_SUCCESS;
}

int case_format_char() {
	struct string_t *s = format("%c", 'c');

	ASSERT(!strcmp(string_to_str(s), "c"));

	string_free(s);

	return TEST_SUCCESS;
}

int case_format_str() {
	struct string_t *s = format("%s", "hello");

	ASSERT(!strcmp(string_to_str(s), "hello"));

	string_free(s);

	return TEST_SUCCESS;
}

int case_format_bool() {
	struct string_t *s = format("{b}", true);
	struct string_t *s2 = format("{b}", false);

	ASSERT(!strcmp(string_to_str(s), "true"));
	ASSERT(!strcmp(string_to_str(s2), "false"));

	string_free(s);
	string_free(s2);

	return TEST_SUCCESS;
}

int case_format_string() {
	struct string_t *s = format("{s}", format("Hello"));

	ASSERT(!strcmp(string_to_str(s), "Hello"));

	string_free(s);

	return TEST_SUCCESS;
}

struct suite_t* format_suite() {
	struct suite_t *r = suite_init("format");

	CASE(r, int, case_format_int);
	CASE(r, double, case_format_double);
	CASE(r, char, case_format_char);
	CASE(r, str, case_format_str);
	CASE(r, bool, case_format_bool);
	CASE(r, string, case_format_string);

	return r;
}

int
main()
{
	struct test_t* t = test_init();

	SUITE(t, string_suite());
	SUITE(t, format_suite());

	RUN_TEST(t);
}
