#include "utils/match.h"

#include <gtest/gtest.h>

namespace cpp_unit
{
  using namespace pene::utils;

  class match_test : public ::testing::Test
  {};

  TEST_F(match_test, wildcard_only)
  {
    EXPECT_TRUE(match("*", "a"));
    EXPECT_TRUE(match("*", "avdsvf"));
  }

  TEST_F(match_test, start_wildcard)
  {
    EXPECT_TRUE(match("*a", "a"));
    EXPECT_TRUE(match("*a", "ba"));
    EXPECT_TRUE(match("*a", "bsadfsa"));
    EXPECT_TRUE(match("*adzefez", "bcedzaadzefez"));
    EXPECT_FALSE(match("*a", "b"));
    EXPECT_FALSE(match("*a", "ab"));
    EXPECT_FALSE(match("*adzefez", "bcedzaadXXzefez"));
  }

  TEST_F(match_test, end_wildcard)
  {
    EXPECT_TRUE(match("a*", "a"));
    EXPECT_TRUE(match("a*", "ab"));
    EXPECT_TRUE(match("a*", "abscfzcfez"));
    EXPECT_TRUE(match("adzefez*", "adzefezefezfz"));
    EXPECT_FALSE(match("a*", "b"));
    EXPECT_FALSE(match("a*", "ba"));
    EXPECT_FALSE(match("adzefez*", "adzeXXfezefezfz"));
  }

  TEST_F(match_test, no_wildcard)
  {
    EXPECT_FALSE(match("a", "a"));
    EXPECT_FALSE(match("azerty", "aze"));
    EXPECT_FALSE(match("azerty", "rty"));
    EXPECT_FALSE(match("azerty*", "aze"));
    EXPECT_FALSE(match("*azerty", "rty"));
  }
}
