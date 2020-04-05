#include "Unittest.h"
#include "Util.h"

#include <string>
#include <stdlib.h>

LOGGED_TEST(UtilTests, EnvLowerCaseStr) {
    bool result;
    std::string val;

    setenv("UTIL_TEST_ENV_VAR", "TEST_VAL", true);
    result = Util::getLowercaseStrFromEnv("UTIL_TEST_ENV_VAR", &val);
    EXPECT_TRUE(result);
    EXPECT_EQ(val, std::string("test_val"));

    unsetenv("UTIL_TEST_ENV_VAR");
    result = Util::getLowercaseStrFromEnv("UTIL_TEST_ENV_VAR", &val);
    EXPECT_FALSE(result);

}
