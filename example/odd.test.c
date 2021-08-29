#include "../st.h"

ST_TEST(one_is_odd)
{
        return 1 % 2 != 0;
}

ST_TEST(three_is_odd)
{
        return 3 % 2 != 0;
}

ST_TEST(five_is_odd)
{
        return 5 % 2 != 0;
}
