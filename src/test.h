extern int (*test)();

#define TEST(test_name) \
        int test_name(); \
        int (*test)() = &(test_name); \
        int test_name()

#define TEST_MAIN() \
        int main(void) \
        { \
                int success; \
                \
                success = (*test)(); \
                printf("<>== Test %s ==<>\n", success ? "passed" : "failed"); \
                return 0; \
        }