#include "../st.h"

st_test(zero_is_even)
{
        return 0 % 2 == 0;
}

st_test(two_is_even)
{
        return 2 % 2 == 0;
}

st_test(four_is_even)
{
        return 4 % 2 == 0;
}
