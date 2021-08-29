#ifndef INCLUDE_ST_H
#define INCLUDE_ST_H
#include <stdlib.h>

typedef struct st_state st_state_t;
typedef struct st_result st_result_t;
typedef st_result_t *(*st_func_t)(void);

st_state_t *st_state_new();
void st_state_delete(st_state_t *self);
void st_state_add_test(st_state_t *self, char *name, st_func_t func);
void st_state_run_tests(st_state_t *self);
st_result_t *st_result_new(int success, char *expr);
void st_result_delete(st_result_t *self);

/*
 * The global state of the program.
 * The state is initialized with references to each defined
 * test before st_main is executed.
 * This approach has the desirable property of not having to
 * explicitly depend on specific tests within st_main.
 */
extern st_state_t *st_state;

/*
 * Asserts that a given Boolean expression evaluates
 * to true and returns the result along with the
 * expression as a string.
 */
#define ST_ASSERT_TRUE(TRUTH) \
        return st_result_new(TRUTH, #TRUTH)

/*
 * Defines a test with a given name and tracks it in the
 * global state before st_main is executed.
 */
#define ST_TEST(NAME)                                           \
        extern st_state_t *st_state;                            \
        static st_result_t *NAME(void);                         \
        static st_result_t *run_##NAME(void)                    \
        {                                                       \
                return NAME();                                  \
        }                                                       \
        void init_##NAME(void) __attribute__((constructor));    \
        void init_##NAME(void)                                  \
        {                                                       \
                if (st_state == NULL)                           \
                        st_state = st_state_new();              \
                st_state_add_test(st_state, #NAME, run_##NAME); \
        }                                                       \
        static st_result_t *NAME(void)

/*
 * Executes each test in the global state and prints
 * the results.
 */
#define ST_MAIN()                             \
        st_state_t *st_state = NULL;          \
        int main(void)                        \
        {                                     \
                st_state_run_tests(st_state); \
                st_state_delete(st_state);    \
                return 0;                     \
        }

#endif /* INCLUDE_ST_H */

/******************************************************
 * Implementation
 *****************************************************/
#ifdef ST_IMPLEMENTATION
#include <stdio.h>

struct st_state {
        struct st_test *tests;
        size_t test_count;
};

struct st_test {
        char *name;
        st_func_t func;
};

struct st_result {
        int success;
        char *expr;
};

/* Creates the global state. */
st_state_t *st_state_new()
{
        st_state_t *self;

        self = malloc(sizeof(st_state_t));
        if (self == NULL) {
                printf("Error: failed to allocate memory for st_state");
                exit(EXIT_FAILURE);
        }
        return self;
}

/* Deletes the global state. */
void st_state_delete(st_state_t *self)
{
        free(self->tests);
        free(self);
}

/* Adds a test to the global state. */
void st_state_add_test(st_state_t *self, char *name, st_func_t func)
{
        size_t new_size;
        size_t index;

        index = self->test_count++;
        new_size = self->test_count * sizeof(struct st_test);
        self->tests = realloc(self->tests, new_size);
        self->tests[index].name = name;
        self->tests[index].func = func;
}

/*
 * Runs all tests in the global state and prints the
 * results.
 */
void st_state_run_tests(st_state_t *self)
{
        size_t i;
        char *name;
        st_func_t func;
        st_result_t *result;

        for (i = 0; i < self->test_count; i++) {
                name = self->tests[i].name;
                func = self->tests[i].func;
                result = func();
                if (result->success)
                        printf("%s passed\n", name);
                else
                        printf("%s failed: (%s) is false\n", name,
                               result->expr);
                st_result_delete(result);
        }
}

/* Creates a new test result. */
st_result_t *st_result_new(int success, char *expr)
{
        st_result_t *self;

        self = malloc(sizeof(st_result_t));
        if (self == NULL) {
                printf("Error: failed to allocate memory for st_result");
                exit(EXIT_FAILURE);
        }
        self->success = success;
        self->expr = expr;
        return self;
}

/* Deletes a test result. */
void st_result_delete(st_result_t *self)
{
        free(self);
}

#endif /* ST_IMPLEMENTATION */
