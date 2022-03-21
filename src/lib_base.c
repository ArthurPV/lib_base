#include <time.h>
#include <assert.h>

#include "lib_base.h"

char*
color_red(char* s)
{
    size_t size = snprintf(NULL, 0, "\x1b[34m%s\x1b[0m", s) + 1;
    char* res = malloc(size);
    snprintf(res, size, "\x1b[34m%s\x1b[0m", s);
    return res;
}

char*
color_green(char* s)
{
    size_t size = snprintf(NULL, 0, "\x1b[32m%s\x1b[0m", s) + 1;
    char* res = malloc(size);
    snprintf(res, size, "\x1b[32m%s\x1b[0m", s);
    return res;
}

char*
color_yellow(char* s)
{
    size_t size = snprintf(NULL, 0, "\x1b[33m%s\x1b[0m", s) + 1;
    char* res = malloc(size);
    snprintf(res, size, "\x1b[33m%s\x1b[0m", s);
    return res;
}

char*
color_blue(char* s)
{
    size_t size = snprintf(NULL, 0, "\x1b[34m%s\x1b[0m", s) + 1;
    char* res = malloc(size);
    snprintf(res, size, "\x1b[34m%s\x1b[0m", s);
    return res;
}

char*
color_magenta(char* s)
{
    size_t size = snprintf(NULL, 0, "\x1b[35m%s\x1b[0m", s) + 1;
    char* res = malloc(size);
    snprintf(res, size, "\x1b[35m%s\x1b[0m", s);
    return res;
}

char*
color_cyan(char* s)
{
    size_t size = snprintf(NULL, 0, "\x1b[36m%s\x1b[0m", s) + 1;
    char* res = malloc(size);
    snprintf(res, size, "\x1b[36m%s\x1b[0m", s);
    return res;
}

char*
style_bold(char* s)
{
    size_t size = snprintf(NULL, 0, "\x1b[1m%s\x1b[0m", s) + 1;
    char* res = malloc(size);
    snprintf(res, size, "\x1b[1m%s\x1b[0m", s);
    return res;
}

void*
copy(void* ptr, size_t size)
{
    void* ptr2 = malloc(size);

    memcpy(ptr2, ptr, size);

    return ptr2;
}

char*
copy_str(char* str)
{
    char* str2 = malloc(sizeof(char));
    str[0] = '\0';

    memcpy(str2, str, strlen(str) + 1);

    return str2;
}

// test_t
// -------------------------

struct case_t*
case_init(const char* name, void(*f))
{
    struct case_t* c = malloc(sizeof(struct case_t));
    c->name = name;
    c->f = f;
    return c;
}

struct suite_t*
suite_init(const char* name)
{
    struct suite_t* s = malloc(sizeof(struct suite_t));
    s->name = name;
    s->time = 0.0;
    s->cases = vec_init();
    return s;
}

struct test_t*
test_init()
{
    struct test_t* t = malloc(sizeof(struct test_t));
    t->suites = vec_init();
    t->time = 0.0;
    return t;
}

void
suite_add_case_to_suite(struct suite_t* suite,
                              struct case_t* _case)
{
    vec_push(suite->cases, (struct case_t*)_case);
}

void
test_add_suite_to_test(struct test_t* test, struct suite_t* suite)
{
    vec_push(test->suites, (struct suite_t*)suite);
}

int
case_run_case(struct case_t* _case)
{
    if (_case->f() == 0) {
        char* failed = RED("failed");
        println("test %s ... %s", _case->name, failed);

        if (failed)
            free(failed);

        return TEST_FAILED;
    } else {
        char* ok = GREEN("ok");
        println("test %s ... %s", _case->name, ok);

        if (ok)
            free(ok);

        return TEST_SUCCESS;
    }
}

void
suite_run_suite(struct suite_t* suite)
{
    int failed = 0;

    clock_t start, end;
    start = clock();

    for (size_t i = 0; i < vec_len(suite->cases); i++) {
        if (case_run_case(suite->cases->items[i]) == 0) {
            failed = 1;
        }
    }

    end = clock();
    suite->time = ((double)end - start) / CLOCKS_PER_SEC;

    if (failed) {
        char* failed = RED("failed");
        println("suite %s ... %s", suite->name, failed);

        if (failed)
            free(failed);

        exit(1);
    } else {
        char* ok = GREEN("ok");
        if (vec_len(suite->cases) > 1) {
            println("%s suite run %zu cases in %fs ... %s\n",
                    suite->name,
                    vec_len(suite->cases),
                    suite->time,
                    ok);
        } else {
            println("%s suite run %zu case in %fs ... %s\n",
                    suite->name,
                    vec_len(suite->cases),
                    suite->time,
                    ok);
        }
        if (ok)
            free(ok);
    }
}

void
test_run_test(struct test_t* test)
{
    clock_t start, end;
    start = clock();

    for (size_t i = 0; i < vec_len(test->suites); i++) {
        suite_run_suite(test->suites->items[i]);
    }

    end = clock();
    test->time = ((double)end - start) / CLOCKS_PER_SEC;

    if (vec_len(test->suites) > 1) {
        println(
          "%zu suites run in %fs.", vec_len(test->suites), test->time);
    } else {
        println(
          "%zu suite run in %fs.", vec_len(test->suites), test->time);
    }
}

void
test_free(struct test_t* test)
{
    if (test) {
        for (size_t i = 0; i < vec_len(test->suites); i++) {
            for (size_t j = 0;
                 j <
                 vec_len(((struct suite_t*)test->suites->items[i])->cases);
                 j++)
                if (((struct suite_t*)test->suites->items[i])
                      ->cases->items[j])
                    free(((struct suite_t*)test->suites->items[i])
                           ->cases->items[j]);

            if (((struct suite_t*)test->suites->items[i])->cases)
                vec_simple_free(
                  ((struct suite_t*)test->suites->items[i])->cases);

            if (test->suites->items[i])
                free(test->suites->items[i]);
        }

        if (test->suites)
            vec_simple_free(test->suites);

        free(test);
    }
}

// -------------------------

// lily0_vec
// -------------------------

struct vec_t*
vec_init()
{
    struct vec_t* v = malloc(sizeof(struct vec_t));
    v->items = malloc(sizeof(void*) * 8);
    v->capacity = 8;
    v->len = 0;
    return v;
}

struct vec_t*
vec_init_with_capacity(size_t capacity)
{
    struct vec_t* v = malloc(sizeof(struct vec_t));
    v->items = malloc(sizeof(void*) * capacity);
    v->capacity = capacity;
    v->len = 0;
    return v;
}

void
vec_push(struct vec_t* v, void* item)
{
    if (v->len == v->capacity) {
        v->capacity *= 2;
        v->items = realloc(v->items, sizeof(void*) * v->capacity);
    }
    v->items[v->len++] = item;
}

void
vec_remove(struct vec_t* v, size_t idx)
{
    struct vec_t* v2 = vec_init();

    for (size_t i = 0; i < vec_len(v); i++)
        if (i != idx)
            vec_push(v2, v->items[i]);

    memcpy(v, v2, sizeof(struct vec_t));
}

size_t
vec_capacity(struct vec_t* v)
{
    return v->capacity;
}

size_t
vec_len(struct vec_t* v)
{
    return v->len;
}

void*
vec_get(struct vec_t* v, size_t n)
{
    assert(n < vec_len(v) && "n > len");
    return v->items[n];
}

int
vec_iter(struct vec_t* v, int (*f)(void*))
{
    for (size_t i = 0; i < vec_len(v); i++)
        f(vec_get(v, i));
}

void
vec_free(struct vec_t* v)
{
    if (v) {
        for (size_t i = 0; i < vec_len(v); i++)
            free(v->items[i]);
        free(v->items);
        free(v);
    }
}

void
vec_simple_free(struct vec_t* v)
{
    if (v) {
        free(v->items);
        free(v);
    }
}

// -------------------------

// result_t
// -------------------------

struct result_t*
result_init_ok(void* ok)
{
    struct result_t* res = malloc(sizeof(struct result_t));
    res->kind = RESULT_KIND_OK;
    res->ok = ok;
    return res;
}

struct result_t*
result_init_err(void* err)
{
    struct result_t* res = malloc(sizeof(struct result_t));
    res->kind = RESULT_KIND_ERR;
    res->err = err;
    return res;
}

void*
result_get_ok_value(struct result_t* res)
{
    assert(res->kind == RESULT_KIND_OK && "ok haven\'t value");
    return res->ok;
}

void*
result_get_err_value(struct result_t* res)
{
    assert(res->kind == RESULT_KIND_ERR && "err haven\'t value");
    return res->err;
}

void*
result_get_value(struct result_t* res)
{
    if (res->kind == RESULT_KIND_OK)
        return res->ok;
    else
        return res->err;
}

void
result_free_ok(struct result_t* res)
{
    if (res) {
        free(res->ok);
        free(res);
    }
}

void
result_free_err(struct result_t* res)
{
    if (res) {
        free(res->err);
        free(res);
    }
}

// -------------------------

// string_t
// -------------------------

struct string_t*
lily0_string_init()
{
    struct string_t* s = malloc(sizeof(struct string_t));
    s->buffer = malloc(sizeof(char) + 1);
    s->buffer[0] = '\0';
    s->len = 0;
    return s;
}

void
string_push_char(struct string_t* s, u8 c)
{
    s->buffer = realloc(s->buffer, strlen(s->buffer) + 2);
    s->buffer[s->len] = c;
    s->buffer[++s->len] = '\0';
}

void
string_push_str(struct string_t* s, char* s2)
{
    for (size_t i = 0; i < strlen(s2); i++)
        string_push_char(s, s2[i]);
}

char*
string_to_str(struct string_t* s)
{
    return s->buffer;
}

char*
string_to_str_and_free(struct string_t* s)
{
    char* s2 = copy_str(s->buffer);
    string_free(s);
    return s2;
}

struct string_t*
string_of_double(double f)
{
    struct string_t* string = string_init();
    size_t size = snprintf(NULL, 0, "%f", f) + 1;
    char* str = malloc(size);

    snprintf(str, size, "%f", f);
    string_push_str(string, str);

    free(str);

    return string;
}

struct string_t*
string_of_int(int i)
{
    struct string_t* string = string_init();
    size_t size = snprintf(NULL, 0, "%d", i) + 1;
    char* str = malloc(size);

    snprintf(str, size, "%d", i);
    string_push_str(string, str);

    free(str);

    return string;
}

struct string_t*
string_of_char(char c)
{
    struct string_t* string = string_init();
    size_t size = snprintf(NULL, 0, "%c", c) + 1;
    char* str = malloc(size);

    snprintf(str, size, "%c", c);
    string_push_str(string, str);

    free(str);

    return string;
}

struct string_t*
string_of_bool(bool b)
{
    struct string_t* string = string_init();

    if (b) {
        size_t size = snprintf(NULL, 0, "%s", TRUE_STR) + 1;
        char* str = malloc(size);

        snprintf(str, size, "%s", TRUE_STR);
        string_push_str(string, str);

        free(str);

        return string;
    } else if (b == 0) {
        size_t size = snprintf(NULL, 0, "%s", FALSE_STR) + 1;
        char* str = malloc(size);

        snprintf(str, size, "%s", FALSE_STR);
        string_push_str(string, str);

        free(str);

        return string;
    } else
        assert(0 && "expected bool");
}

struct string_t*
string_format(char* fmt, ...)
{
    struct string_t* s = string_init();

    va_list vl;
    va_start(vl, fmt);

    for (size_t i = 0; i < strlen(fmt); i++) {
        switch (fmt[i]) {
            case '{':
                assert(i++ < strlen(fmt) && "out of str");
                switch (fmt[i]) {
                    case 'b': {
                        struct string_t* str_b =
                        string_of_bool(va_arg(vl, int));
                        string_push_str(s, str_b->buffer);
                        string_free(str_b);
                        i++;
                        assert(fmt[i] == '}' && "expected \'}\'");
                        break;
                    }
                    case 's': {
                        struct string_t* str_s = va_arg(vl, string_t*);
                        string_push_str(s, string_to_str(str_s));
                        string_free(str_s);
                        i++;
                        assert(fmt[i] == '}' && "expected \'}\'");
                        break;
                    }
                    case '{': {
                        string_push_char(s, '{');
                        i++;
                        break;
                    }
                    default:
                        assert(0 && "invalid format specifier");
                }
                break;
            case '%':
                assert(i++ < strlen(fmt) && "out of str");
                switch (fmt[i]) {
                    case 'd': {
                        struct string_t* str_i =
                        string_of_int(va_arg(vl, int));
                        string_push_str(s, str_i->buffer);
                        string_free(str_i);
                        i++;
                        break;
                    }
                    case 'c': {
                        struct string_t* str_c =
                        string_of_char(va_arg(vl, int));
                        string_push_str(s, str_c->buffer);
                        string_free(str_c);
                        i++;
                        break;
                    }
                    case 'f': {
                        struct string_t* str_f =
                        string_of_double(va_arg(vl, double));
                        string_push_str(s, str_f->buffer);
                        string_free(str_f);
                        i++;
                        break;
                    }
                    case 's': {
                        string_push_str(s, va_arg(vl, char*));
                        i++;
                        break;
                    }
                    case '%': {
                        string_push_char(s, '%');
                        i++;
                        break;
                    }
                    default:
                        assert(0 && "invalid format specifier");
                }
            default:
                string_push_char(s, fmt[i]);
        }
    }

    va_end(vl);

    return s;
}

size_t
string_len(struct string_t* s)
{
    return s->len;
}

char
string_get(struct string_t* s, size_t n)
{
    assert(n < string_len(s) && "n > len");
    return s->buffer[n];
}

int
string_iter(struct string_t* s, int (*f)(char))
{
    for (size_t i = 0; i < string_len(s); i++)
        f(string_get(s, i));
    return 0;
}

void
string_free(struct string_t* s)
{
    if (s) {
        free(s->buffer);
        free(s);
    }
}

// -------------------------
