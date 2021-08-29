#include "../st.h"

ST_TEST(one_is_odd)
{
        ST_ASSERT_TRUE(1 % 2 != 0);
}

ST_TEST(three_is_odd)
{
        ST_ASSERT_TRUE(3 % 2 != 0);
}

ST_TEST(five_is_odd)
{
        ST_ASSERT_TRUE(5 % 2 != 0);
}
