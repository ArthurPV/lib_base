#ifndef LIB_BASE_H
#define LIB_BASE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define print(...) printf(__VA_ARGS__)

#define println(...)                                                           \
    {                                                                          \
        printf(__VA_ARGS__);                                                   \
        printf("\n");                                                          \
    }

// Transform to red color from str
char*
color_red(char* s);

#define RED(s) color_red(s)

// Transform to green color from str
char*
color_green(char* s);

#define GREEN(s) color_green(s)

// Transform to yellow color from str
char*
color_yellow(char* s);

#define YELLOW(s) color_yellow(s)

// Transform to blue color from str
char*
color_blue(char* s);

#define BLUE(s) color_blue(s)

// Transform to cyan color from str
char*
color_cyan(char* s);

#define CYAN(s) color_cyan(s)

// Change style to bold from str
char*
style_bold(char* s);

#define BOLD(s) style_bold(s)

// Copy ptr
void*
copy(void* ptr, size_t size);

// Copy str
char*
copy_str(char* str);

typedef struct case_t case_t;
typedef struct suite_t suite_t;
typedef struct test_t test_t;
typedef struct vec_t vec_t;
typedef enum result_kind result_kind;
typedef struct result_t result_t;
typedef struct string_t string_t;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef __int128 i128;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define CASE_T_SIZE sizeof(struct case_t)
#define SUITE_T_SIZE sizeof(struct suite_t)
#define TEST_T_SIZE sizeof(struct test_t)
#define VEC_T_SIZE sizeof(struct vec_t)
#define RESULT_T_SIZE sizeof(struct result_t)
#define STRING_T_SIZE sizeof(struct string_t)

// test_t
// -------------------------

#define TEST_SUCCESS 1
#define TEST_FAILED 0

#define ASSERT_EQ(l, r)                                                        \
    {                                                                          \
        if (l != r) {                                                          \
            return TEST_FAILED;                                                \
        }                                                                      \
    }

#define ASSERT_NE(l, r)                                                        \
    {                                                                          \
        if (l == r) {                                                          \
            return TEST_FAILED;                                                \
        }                                                                      \
    }

#define ASSERT(expr)                                                           \
    {                                                                          \
        if (!expr) {                                                           \
            return TEST_FAILED;                                                \
        }                                                                      \
    }

struct case_t
{
    const char* name; // Name of case
    int (*f)(void);   // Function test
};

struct suite_t
{
    const char* name;    // Name of suite
    double time;         // Time of case(s) execution
    struct vec_t* cases; // Vector of case_t
};

struct test_t
{
    struct vec_t* suites; // Vector of suite_t
    double time;          // Time of suite(s) execution
};

// Init case_t
// name: Name of case
// f: Function test
struct case_t*
case_init(const char* name, void(*f));

// Init suite_t
// name: Name of suite
struct suite_t*
suite_init(const char* name);

// Init test_t
struct test_t*
test_init();

// Add case to suite_t
void
suite_add_case_to_suite(struct suite_t* suite, struct case_t* _case);

#define CASE(suite, name, f) suite_add_case_to_suite(suite, case_init(#name, f))

// Add suite to test_t
void
test_add_suite_to_test(struct test_t* test, struct suite_t* suite);

#define SUITE(test, suite) test_add_suite_to_test(test, suite)

// Run a case
int
case_run_case(struct case_t* _case);

// Run a suite
void
suite_run_suite(struct suite_t* suite);

// Run all tests
void
test_run_test(struct test_t* test);

#define RUN_TEST(test)                                                         \
    {                                                                          \
        test_run_test(test);                                                   \
        test_free(test);                                                       \
    }

// Free test_t type
void
test_free(struct test_t* test);

// -------------------------

// vec_t
// -------------------------

struct vec_t
{
    void** items;    // Vec items
    size_t capacity; // Vec capacity
    size_t len;      // Vec len
};

// Init vec_t type
// initial capacity: 8
struct vec_t*
vec_init();

// Init vec_t type with custom initial capacity
struct vec_t*
vec_init_with_capacity(size_t capacity);

// Push item to vec
void
vec_push(struct vec_t* v, void* item);

// Remove item from index
void
vec_remove(struct vec_t* v, size_t idx);

// Get capacity of vec
size_t
vec_capacity(struct vec_t* v);

// Get len of vec
size_t
vec_len(struct vec_t* v);

// Get n item of vec
void*
vec_get(struct vec_t* v, size_t n);

// Iter on vec_t
int
vec_iter(struct vec_t* v, int (*f)(void*));

// Map on vec_t
struct vec_t*
vec_map(struct vec_t* v, void* (*f)(void*));

// Filter on vec_t
struct vec_t*
vec_filter(struct vec_t* v, bool (*f)(void*));

// Free vec_t type with custom ptr like result_t*
void
vec_free_custom_ptr(struct vec_t* v, void(*f)(void*));

// Free vec_t type
void vec_free(struct vec_t *v);

// Simple free vec_t type
void
vec_simple_free(struct vec_t* v);

// -------------------------

// result_t
// -------------------------

#define IS_OK(res) res->kind == RESULT_KIND_OK
#define IS_ERR(res) res->kind == RESULT_KIND_ERR

enum result_kind
{
    RESULT_KIND_OK,
    RESULT_KIND_ERR
};

struct result_t
{
    enum result_kind kind;
    union
    {
        void* ok;
        void* err;
    };
};

// Init result_t with ok value
struct result_t*
result_init_ok(void* ok);

#define Ok(ok) result_init_ok(ok)

// Init result_t with err value
struct result_t*
result_init_err(void* err);

#define Err(err) result_init_err(err)

// Get ok value
void*
result_get_ok_value(struct result_t* res);

// Get err value
void*
result_get_err_value(struct result_t* res);

// Get value (ok or err)
void*
result_get_value(struct result_t* res);

// Free result_t
void
result_free(struct result_t* res);

// -------------------------

// string_t
// -------------------------

#define TRUE_STR "true"
#define FALSE_STR "false"

struct string_t
{
    char* buffer;
    size_t len;
};

// Init string_t type
struct string_t*
string_init();

// Push character
void
string_push_char(string_t* s, u8 c);

// Push str
void
string_push_str(string_t* s, char* s2);

// Convert string_t to char*
char*
string_to_str(string_t* s);

#define to_str(s) lily0_string_to_str(s)

// Convert string_t to char* and free ptr
char*
string_to_str_and_free(string_t* s);

#define to_str_and_free(s) string_to_str_and_free(s)

// Convert double to string_t
struct string_t*
string_of_double(double f);

// Convert int to string_t
struct string_t*
string_of_int(int i);

// Convert char to string_t
struct string_t*
string_of_char(char c);

// Convert bool to string_t
struct string_t*
string_of_bool(bool b);

// Format string
struct string_t*
string_format(char* fmt, ...);

#define format(...) string_format(__VA_ARGS__)

// Get a of string_t
size_t
string_len(struct string_t* s);

// Get a n char of string_t
char
string_get(struct string_t* s, size_t n);

// Iter a string_t
int
string_iter(struct string_t* s, int (*f)(char));

// Free string_t
void
string_free(struct string_t* s);

// -------------------------

#endif // LIB_BASE_H
