#include <lib_base.h>

int case_push_item() {
	struct vec_t *v = vec_init();
	vec_push(v, (int*)1);
	vec_push(v, (int*)2);
	vec_push(v, (int*)3);
	vec_push(v, (int*)4);
	vec_push(v, (int*)5);

	ASSERT_EQ(vec_get(v, 0), (int*)1);
	ASSERT_EQ(vec_get(v, 1), (int*)2);
	ASSERT_EQ(vec_get(v, 2), (int*)3);
	ASSERT_EQ(vec_get(v, 3), (int*)4);
	ASSERT_EQ(vec_get(v, 4), (int*)5);

	vec_simple_free(v);

	return TEST_SUCCESS;
}

struct suite_t* vec_suite() {
	struct suite_t *r = suite_init("vec");

	CASE(r, push item, case_push_item);

	return r;
}

int
main() {
	struct test_t* t = test_init();

	SUITE(t, vec_suite());

	RUN_TEST(t);
}
