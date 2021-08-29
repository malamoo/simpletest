#ifndef INCLUDE_ST_H
#define INCLUDE_ST_H
#include <stdlib.h>

typedef int (*st_func_t)(void);
typedef struct st_state st_state_t;

st_state_t *st_state_new();
void st_state_delete(st_state_t *self);
void st_state_add_test(st_state_t *self, char *test_name, st_func_t test_func);
void st_state_run_tests(st_state_t *self);

/*
 * The global state of the program.
 * The state is initialized with references to each defined
 * test before st_main is executed.
 * This approach has the desirable property of not having to
 * explicitly depend on specific tests within st_main.
 */
extern st_state_t *st_state;

/*
 * Defines a test with a given name and tracks it in the
 * global state before st_main is executed.
 */
#define ST_TEST(test_name)                                                \
        extern st_state_t *st_state;                                      \
        static int test_name(void);                                       \
        static int run_##test_name(void)                                  \
        {                                                                 \
                return test_name();                                       \
        }                                                                 \
        static void init_##test_name(void) __attribute__((constructor));  \
        static void init_##test_name(void)                                \
        {                                                                 \
                if (st_state == NULL)                                     \
                        st_state = st_state_new();                        \
                st_state_add_test(st_state, #test_name, run_##test_name); \
        }                                                                 \
        static int test_name(void)

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

struct st_test {
        char *name;
        st_func_t func;
        int success;
};

struct st_state {
        struct st_test *tests;
        size_t test_count;
};

/* Creates the global state. */
st_state_t *st_state_new()
{
        st_state_t *self;

        self = malloc(sizeof(struct st_state));
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
void st_state_add_test(st_state_t *self, char *test_name, st_func_t test_func)
{
        size_t new_size;
        size_t index;

        index = self->test_count++;
        new_size = self->test_count * sizeof(struct st_test);
        self->tests = realloc(self->tests, new_size);
        self->tests[index].name = test_name;
        self->tests[index].func = test_func;
}

/*
 * Runs all tests in the global state and prints the
 * results.
 */
void st_state_run_tests(st_state_t *self)
{
        size_t i;
        char *test_name;
        st_func_t test_func;
        int success;

        for (i = 0; i < self->test_count; i++) {
                test_name = self->tests[i].name;
                test_func = self->tests[i].func;
                success = test_func();
                printf("%s %s\n", test_name, success ? "passed" : "failed");
        }
}

#endif /* ST_IMPLEMENTATION */
