/*
 User defined logging
  LOG(format, param1, param2)
*/

#include <string.h>  // std::memchr
#include <cstring>

#include <array>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct LogParam;
struct LogContext;

using WriterFunc = std::function<bool(LogContext&)>;

inline void writeToRawBytes(const void* data, size_t size, void* bytes) noexcept {
    auto input = static_cast<const uint8_t*>(data);
    std::copy(input, input + size, static_cast<uint8_t*>(bytes));
}

struct LogContext {
    std::stringstream text;
    std::array<uint8_t, 512> data;
    uint32_t data_length{0};

    // Appends raw bytes without creating a corresponding param.
    LogContext& appendRaw(const void* bytes, uint32_t size) noexcept {
        writeToRawBytes(bytes, size, &data[data_length]);
        data_length += size;
        return *this;
    }

    /*
    In this article, we have presented different ways to convert an array of uint8_t (uint8_t*) to string in C++.
    String is a special datatype in C++ and is equivalent to an array of characters that is char*.

    In this case, we are converting array of uint8_t that is uint8_t*. So, uint8_t* is equivalent to char* or string.

    Approach 1: Explicit Casting
        uint8_t* input = {12, 1, 65, 90};
        char* input2 = (char *) input;
        string input3 = input2;

    Approach 2: reinterpret_cast
    As alternative to explicit casting will be to use reinterpret_cast.

        uint8_t* input = {12, 1, 65, 90};
        std::string str = reinterpret_cast<char *>(input);


    Approach 3: ostringstream
    ostringstream is a builder where each element from the uint8_t array is appended one by one. Following it, calling
    the str() method of ostringstream returns the string.

        uint8_t* input = {12, 1, 65, 90};
        std::ostringstream convert;
        for (int i = 0; i < 4; i++) {
            convert << (int)input[i];
        }
        std::string key_string = convert.str();


        // https://stackoverflow.com/questions/31418290/method-that-converts-uint8-t-to-string
   If the goal is to construct a string of 2-character hex values, you can use a string stream with IO manipulators like
this:

        std::string to_hex( uint8_t data[32] )
        {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for( uint8_t val : data )
        {
            oss << std::setw(2) << (unsigned int)val;
        }
        return oss.str();
        }
    */

    std::string to_string() {
        // convert uint8 to string
        std::string str = reinterpret_cast<char*>(this->data.data());
        return str;
    }
};

template <typename T>
const WriterFunc ResolveLogParamWriter(T&& param) {
    return [&param](LogContext& context) {
        std::cout << "use default" << std::endl;
        // context.text << param;
        std::stringstream ss;
        ss << param;
        auto serialized = ss.str();
        context.appendRaw(serialized.c_str(), serialized.length());
        return true;
    };
}

// 模板特化
// https://blog.csdn.net/gatieme/article/details/50953564
template <>
const WriterFunc ResolveLogParamWriter(bool&& param) {
    return [&param](LogContext& context) {
        std::stringstream ss;

        if (param) {
            ss << "True";
        } else {
            ss << "False";
        }
        auto serialized = ss.str();
        context.appendRaw(serialized.c_str(), serialized.length());
        return true;
    };
}

template <>
const WriterFunc ResolveLogParamWriter(float&& param) {
    return [&param](LogContext& context) {
        std::stringstream ss;
        // ss << std::fixed << std::setprecision(6) << param << "f";  0.100000f
        ss << std::setprecision(6) << param << "f";  // 0.1f
        auto serialized = ss.str();
        context.appendRaw(serialized.c_str(), serialized.length());
        return true;
    };
}

struct LogParam {
    LogParam() = default;

    template <typename param_t>
    LogParam(param_t&& param_) {
        param = &param_;
        writer = ResolveLogParamWriter(std::forward<param_t>(param_));
    }

    bool logPrint(LogContext& context) const { return writer(context); }

    const void* param{nullptr};
    WriterFunc writer{nullptr};
};

static LogContext log_context;

template <typename... Args>
void explore_log1(const std::string& format, Args... args) {
    std::cout << format << std::endl;
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
    std::cout << sizeof...(args) << std::endl;
    (LogParam(args).logPrint(log_context), ...);
    std::cout << log_context.text.str() << std::endl;
}

template <typename... Args>
void explore_log2(const std::string& format, Args&&... args) {
    std::cout << format << std::endl;
    // Parse format: delimeter

    auto segement_format = [](std::string s, const std::string& del) {
        std::vector<std::string> res;
        // Use find function to find 1st position of delimiter.
        int end = s.find(del);
        while (end != -1) {  // Loop until no delimiter is left in the string.
            res.emplace_back(s.substr(0, end));
            s.erase(s.begin(), s.begin() + end + 1);
            end = s.find(del);
        }
        res.emplace_back(s.substr(0, end));
        return res;
    };

    // LogParam[] params = {LogParam({std::forward<Args>(args)})...};

    // std::vector<LogParam> params;
    // params.emplace_back();
    // std::cout << "size: " << sizeof...(args) << ", " << sizeof(params) / sizeof(params[0]) << std::endl;

    auto segements = segement_format(format, "%%");

    // https://stackoverflow.com/questions/69107224/how-to-convert-variadic-function-arguments-to-array
    constexpr std::size_t n = sizeof...(args);
    static_assert(n >= 1, "must pass at least one argument");
    /*
    auto& tuple = std::tie(args...);

    error: cannot bind non-const lvalue reference of type 'std::tuple<int&, bool&, float&>&' to an rvalue of type
  'std::tuple<int&, bool&, float&>' 109 |     auto& tuple = std::tie(args...); |           ^~~~~
    */

    LogParam params[] = {LogParam({std::forward<Args>(args)})...};
    std::cout << "size=" << segements.size() << ", real size=" << sizeof(params) / sizeof(params[0]) << std::endl;
    for (auto& seq : segements) {
        std::cout << seq << std::endl;
    }
}

template <typename... Args>
std::string log(const std::string& format, Args&&... args) {
    constexpr std::size_t n = sizeof...(args);
    static_assert(n >= 1, "must pass at least one argument");

    LogParam params[] = {LogParam({std::forward<Args>(args)})...};

    uint64_t format_param_idx = 0;
    auto format_string = format.c_str();
    auto last_non_placeholder = format_string;
    auto format_string_end = format_string + format.length();

    while (format_string != format_string_end) {
        const auto next_placeholder =
          static_cast<const char*>(std::memchr(format_string, '%', format_string_end - format_string));

        if (!next_placeholder) {
            // No more placeholders
            break;
        }

        if (next_placeholder + 1 == format_string_end || *(next_placeholder + 1) != '%') {
            // A lone % character, skip over.
            format_string = next_placeholder + 1;
            continue;
        }

        // Found a %% placeholder 这里就是分割字符串
        log_context.appendRaw(last_non_placeholder, next_placeholder - last_non_placeholder);
        params[format_param_idx++].logPrint(log_context);

        format_string = last_non_placeholder = next_placeholder + 2;
    }
    std::cout << log_context.data_length << std::endl;
    std::cout << log_context.data.size() << std::endl;
    std::cout << log_context.to_string() << std::endl;
    return log_context.to_string();
}
