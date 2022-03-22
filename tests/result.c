#include <lib_base.h>

int case_ok_value() {
	struct result_t *res = result_init_ok((int*)30);

	ASSERT_EQ((int*)res->ok, (int*)30);
	ASSERT_EQ(res->kind, RESULT_KIND_OK);

	result_free(res);

	return TEST_SUCCESS;
}

int case_err_value() {
	struct result_t *res = result_init_err((int*)20);

	ASSERT_EQ((int*)res->ok, (int*)20);
	ASSERT_EQ(res->kind, RESULT_KIND_ERR);

	result_free(res);

	return TEST_SUCCESS;
}

struct suite_t*
result_suite() {
	struct suite_t *r = suite_init("result");

	CASE(r, ok value, case_ok_value);
	CASE(r, err value, case_err_value);

	return r;
}

int
main()
{
	struct test_t* t = test_init();

	SUITE(t, result_suite());

	RUN_TEST(t);
}
