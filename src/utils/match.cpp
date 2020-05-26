#include "utils/match.h"

namespace pene
{
  namespace utils
  {
    bool match(const std::string& wilded_str, const std::string& to_match_str)
    {
      if (to_match_str.length() < wilded_str.length() - 1)
      {
        return false;
      }
      if (wilded_str[0] == '*')
      {
        if (wilded_str.length() == 1)
        {
          return true;
        }
        for (size_t i = 0; i < wilded_str.length() - 1; ++i)
        {
          if (wilded_str[1 + i] != to_match_str[to_match_str.length() - wilded_str.length() + 1 + i])
          {
            return false;
          }
        }
        return true;
      }
      if (wilded_str[wilded_str.length() - 1] == '*')
      {
        for (size_t i = 0; i < wilded_str.length() - 1; ++i)
        {
          if (wilded_str[i] != to_match_str[i])
          {
            return false;
          }
        }
        return true;
      }
      return false;
    }
  }
}

