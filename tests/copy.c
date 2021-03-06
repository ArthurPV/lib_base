#include <lib_base.h>

int
case_copy_int()
{
    int a = 3;
    int* b = &a;
    int* c = copy(b, sizeof(int));

    ASSERT_EQ(*b, *c);

    free(c);

    return TEST_SUCCESS;
}

int
case_copy_str()
{
    char* a = "hello";
    char* b = copy_str(a);

    ASSERT(!strcmp(a, b));

    free(b);

    return TEST_SUCCESS;
}

int
case_copy_vec()
{
    struct vec_t* v = vec_init();
    vec_push(v, (int*)1);
    vec_push(v, (int*)2);
    vec_push(v, (int*)3);
    vec_push(v, (int*)4);
    struct vec_t* v2 = copy(v, VEC_T_SIZE);

    ASSERT_EQ(vec_get(v, 0), vec_get(v2, 0));
    ASSERT_EQ(vec_get(v, 1), vec_get(v2, 1));
    ASSERT_EQ(vec_get(v, 2), vec_get(v2, 2));
    ASSERT_EQ(vec_get(v, 3), vec_get(v2, 3));

    vec_free(v2);
    vec_free(v);

    return TEST_SUCCESS;
}

int case_copy_string() {
    struct string_t *s = string_init();
    string_push_char(s, 'h');
    string_push_char(s, 'e');
    string_push_char(s, 'l');
    string_push_char(s, 'l');
    string_push_char(s, 'o');

    struct string_t *s2 = copy(s, STRING_T_SIZE);

    ASSERT(!strcmp(string_to_str(s), string_to_str(s2)));

    string_free(s);
    free(s2);

    return TEST_SUCCESS;
}

struct suite_t*
copy_suite()
{
    struct suite_t* c = suite_init("copy");

    CASE(c, copy int, case_copy_int);
    CASE(c, copy str, case_copy_str);
    CASE(c, copy vec, case_copy_str);
    CASE(c, copy string, case_copy_string);

    return c;
}

int
main()
{
    struct test_t* t = test_init();

    SUITE(t, copy_suite());

    RUN_TEST(t);
}
