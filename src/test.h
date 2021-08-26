#include <stdio.h>
#include <stdlib.h>

typedef int (*st_fp)(void);

/* Global state required by main() to execute tests. */
struct st_state {
        st_fp *tests;
        int test_count;
};

/*
 * Declared extern to allow the global state to
 * be visible across all test files.
 */
extern struct st_state st_state;

/*
 * Defines a initialization procedure for a given test
 * to be called before main() to store that test
 * in the global state.
 */
#define ST_INIT(test_name)                                               \
        static void init_##test_name(void) __attribute__((constructor)); \
        static void init_##test_name(void)

/*
 * Defines a test and a procedure to store the test in the global state.
 * main() implicitly requires that all tests to be executed are already
 * stored in the global state. The procedure run_##test_name() is defined
 * so that we can indirectly do this before the test is actually defined.
 */
#define ST_TEST(test_name)                                                 \
        extern struct st_state st_state;                                   \
        static int test_name(void);                                        \
        static int run_##test_name(void)                                   \
        {                                                                  \
                return test_name();                                        \
        }                                                                  \
        ST_INIT(test_name)                                                 \
        {                                                                  \
                int new_size;                                              \
                                                                           \
                st_state.test_count++;                                     \
                new_size = st_state.test_count * sizeof(st_fp);            \
                st_state.tests = realloc(st_state.tests, new_size);        \
                st_state.tests[st_state.test_count - 1] = run_##test_name; \
        }                                                                  \
        static int test_name(void)

/* Executes each test stored in the global state and prints the results. */
#define ST_MAIN()                                                           \
        struct st_state st_state = {0};                                     \
        int main(void)                                                      \
        {                                                                   \
                int i;                                                      \
                int success;                                                \
                                                                            \
                for (i = 0; i < st_state.test_count; i++) {                 \
                        success = st_state.tests[i]();                      \
                        printf("Test %s\n", success ? "passed" : "failed"); \
                }                                                           \
                return 0;                                                   \
        }