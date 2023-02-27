#ifndef PUREFSM_LOGGER_HPP
#define PUREFSM_LOGGER_HPP

#include <ostream>

namespace pure {

  namespace logger {

#if defined(__GNUC__) || defined(__MINGW32__) || defined(__clang__) ||         \
    defined(__INTEL_COMPILER) || (defined(__ICC) && (__ICC >= 600)) ||         \
    (defined(__DMC__) && __DMC__ >= 0x810)

    template <typename T>
    std::string type_name() {
      using size_type = typename std::string::size_type;
      char const* str = __PRETTY_FUNCTION__;
      while (*str != '=') ++str;
      str += 2;
      size_type size = 0;
      while (str[size] != ';' && str[size] != ']') ++size;
      return std::string(str, size);
    }

#elif defined(__MSC_VER) || defined(__FUNCSIG__)

    template <typename T>
    std::string type_name() {
      using size_type = typename std::string::size_type;
      const std::string s = __FUNCSIG__;
      const std::string fname = "type_name<";
      const std::string rest = ">(void)";
      size_type pos = s.find(fname) + fname.size();
      size_type count = s.find(rest) - pos;
      return std::string(s, pos, count);
    }

#else

  #warning                                                                     \
      "Your compiler does not support the reflection required by the logger"

    template <typename T>
    std::string type_name() {
      return {};
    }

#endif

  } // namespace logger

  template <std::ostream& stream>
  class stdout_logger {
  public:
    stdout_logger() = default;

    template <typename T>
    inline void write(const char* str) {
      stream << str << logger::type_name<T>() << std::endl;
    }

    inline void write(const char* str) { stream << str << std::endl; }
  };

  class user_logger {
  private:
    std::ostream& stream;
  public:
    user_logger() = delete;

    user_logger(std::ostream& stream) : stream(stream) {}

    template <typename T>
    inline void write(const char* str) {
      stream << str << logger::type_name<T>() << std::endl;
    }

    inline void write(const char* str) { stream << str << std::endl; }
  };

} // namespace pure

#endif
