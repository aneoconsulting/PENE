#include <fstream>
#include <string>

namespace pene
{
  namespace utils
  {
    static inline std::string trim(const std::string& line)
    {
      const char* WhiteSpace = " \t\v\r\n";
      std::size_t start = line.find_first_not_of(WhiteSpace);
      std::size_t end = line.find_last_not_of(WhiteSpace);
      return start == end ? std::string() : line.substr(start, end - start + 1);
    }

    // same as std::getline but also handling \r to get windows/linux/macos portability.
    static inline std::istream& safeGetline(std::istream& is, std::string& t)
    {
      t.clear();
      // The characters in the stream are read one-by-one using a std::streambuf.
      // That is faster than reading them one-by-one using the std::istream.
      // Code that uses streambuf this way must be guarded by a sentry object.
      // The sentry object performs various tasks,
      // such as thread synchronization and updating the stream state.
      std::istream::sentry se(is, true);
      std::streambuf* sb = is.rdbuf();
      while (true)
      {
        int c = sb->sbumpc();
        if (c == '\n') return is;
        if (c == '\r')
        {
          if (sb->sgetc() == '\n')
            sb->sbumpc();
          return is;
        }
        if (c == std::streambuf::traits_type::eof())
        {
          // Also handle the case when the last line has no line ending
          if (t.empty())
            is.setstate(std::ios::eofbit);
          return is;
        }
        t += (char)c;
      }
    }
  }
}