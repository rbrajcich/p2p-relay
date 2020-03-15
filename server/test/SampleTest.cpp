#include "gtest/gtest.h"
#include "Sample.h"

TEST(MyTestSuite, MyTestCase) {
    EXPECT_EQ(11, addIntegers(5, 6));
}
