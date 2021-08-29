#ifndef INCLUDE_ST_H
#define INCLUDE_ST_H
#include <stdlib.h>

typedef struct st_state st_state_t;
typedef void (*st_func_t)(size_t index);

st_state_t *st_state_new();
void st_state_delete(st_state_t *self);
void st_state_add_test(st_state_t *self, char *name, st_func_t func);
void st_state_run_tests(st_state_t *self);
void st_state_fail_test(st_state_t *self, size_t index, char *reason);

/*
 * The global state of the program.
 * The state is initialized with references to each
 * defined test before ST_MAIN is executed.
 * This approach leads to the desirable property of not
 * having to explicitly depend on specific tests
 * within ST_MAIN.
 */
extern st_state_t *st_state;

/*
 * Asserts that a given Boolean expression evaluates
 * to true and if not, fails the associated test.
 */
#define ST_ASSERT_TRUE(TRUTH)                              \
        if (!(TRUTH)) {                                    \
                st_state_fail_test(st_state, st_index,     \
                                   "(" #TRUTH ")"          \
                                   " evaluates to false"); \
                return;                                    \
        }

/*
 * Defines a test with a given name and tracks it in the
 * global state before ST_MAIN is executed.
 */
#define ST_TEST(NAME)                                           \
        extern st_state_t *st_state;                            \
        static void NAME(size_t st_index);                      \
        static void run_##NAME(size_t st_index)                 \
        {                                                       \
                return NAME(st_index);                          \
        }                                                       \
        void init_##NAME(void) __attribute__((constructor));    \
        void init_##NAME(void)                                  \
        {                                                       \
                if (st_state == NULL)                           \
                        st_state = st_state_new();              \
                st_state_add_test(st_state, #NAME, run_##NAME); \
        }                                                       \
        static void NAME(size_t st_index)

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
        int success;
        char *fail_reason;
};

/* Creates the global state. */
st_state_t *st_state_new()
{
        st_state_t *self;

        self = malloc(sizeof(st_state_t));
        if (self == NULL) {
                printf("Error: failed to allocate memory for st_state_t");
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
        self->tests[index].success = 1;
        self->tests[index].fail_reason = NULL;
}

/*
 * Runs all tests in the global state and prints the
 * results.
 */
void st_state_run_tests(st_state_t *self)
{
        size_t i;
        struct st_test *tests;

        tests = self->tests;
        for (i = 0; i < self->test_count; i++) {
                tests[i].func(i);
                if (tests[i].success)
                        printf("%s PASSED\n", tests[i].name);
                else
                        printf("%s FAILED: %s\n", tests[i].name,
                               tests[i].fail_reason);
        }
}

/*
 * Fails a test in the global state at the given index
 * with a reason.
 */
void st_state_fail_test(st_state_t *self, size_t index, char *reason)
{
        self->tests[index].success = 0;
        self->tests[index].fail_reason = reason;
}

#endif /* ST_IMPLEMENTATION */
