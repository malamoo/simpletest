#include "../st.h"

ST_TEST(zero_is_even)
{
        ST_ASSERT_TRUE(0 % 2 == 0);
}

ST_TEST(two_is_even)
{
        ST_ASSERT_TRUE(2 % 2 == 0);
}

ST_TEST(four_is_even)
{
        ST_ASSERT_TRUE(4 % 2 == 0);
}
