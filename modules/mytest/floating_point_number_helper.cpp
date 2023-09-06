#include "floating_point_number_helper.h"
#include "edit_distance.h"

#include <vector>

namespace mytesting
{
    // Constructs an empty Message.
    // We allocate the stringstream separately because otherwise each use of
    // ASSERT/EXPECT in a procedure adds over 200 bytes to the procedure's
    // stack frame leading to huge stack frames in some cases; gcc does not reuse
    // the stack space.
    Message::Message() : ss_(new ::std::stringstream) {
        // By default, we want there to be enough precision when printing
        // a double to a Message.
        *ss_ << std::setprecision(std::numeric_limits<double>::digits10 + 2);
    }

    // These two overloads allow streaming a wide C string to a Message
    // using the UTF-8 encoding.
    Message& Message::operator<<(const wchar_t* wide_c_str) {
        // return *this << internal::String::ShowWideCString(wide_c_str);
        return *this << "overloading operator<<< 1";
    }

    Message& Message::operator<<(wchar_t* wide_c_str) {
        // return *this << internal::String::ShowWideCString(wide_c_str);
        return *this << "overloading operator << 2";
    }

#if GTEST_HAS_STD_WSTRING
    // Converts the given wide string to a narrow string using the UTF-8
    // encoding, and streams the result to this Message object.
    Message& Message::operator<<(const ::std::wstring& wstr) {
        internal::StreamWideCharsToMessage(wstr.c_str(), wstr.length(), this);
        return *this;
    }
#endif  // GTEST_HAS_STD_WSTRING

    // Gets the text streamed to this object so far as an std::string.
    // Each '\0' character in the buffer is replaced with "\\0".
    std::string Message::GetString() const {
        return StringStreamToString(ss_.get());
        // return std::string("GetString method:" + std::string(__FILE__) +std::string(__LINE__);
        // return std::string("GetString method000");
    }

    // AssertionResult constructors.
    // Used in EXPECT_TRUE/FALSE(assertion_result).
    AssertionResult::AssertionResult(const AssertionResult& other)
      : success_(other.success_),
        message_(other.message_.get() != nullptr ? new ::std::string(*other.message_)
                                                 : static_cast< ::std::string*>(nullptr)) {}

    // Swaps two AssertionResults.
    void AssertionResult::swap(AssertionResult& other) {
        using std::swap;
        swap(success_, other.success_);
        swap(message_, other.message_);
    }

    // Returns the assertion's negation. Used with EXPECT/ASSERT_FALSE.
    AssertionResult AssertionResult::operator!() const {
        AssertionResult negation(!success_);
        if (message_.get() != nullptr) negation << *message_;
        return negation;
    }

    // Makes a successful assertion result.
    AssertionResult AssertionSuccess() { return AssertionResult(true); }

    // Makes a failed assertion result.
    AssertionResult AssertionFailure() { return AssertionResult(false); }

    // Makes a failed assertion result with the given failure message.
    // Deprecated; use AssertionFailure() << message.
    AssertionResult AssertionFailure(const Message& message) { return AssertionFailure() << message; }

    // Converts the buffer in a stringstream to an std::string, converting NUL
    // bytes to "\\0" along the way.
    std::string StringStreamToString(::std::stringstream* ss) {
        const ::std::string& str = ss->str();
        const char* const start = str.c_str();
        const char* const end = start + str.length();

        std::string result;
        result.reserve(static_cast<size_t>(2 * (end - start))); // 这里为什么要乘以2?
        for (const char* ch = start; ch != end; ++ch) {
            if (*ch == '\0') {
                result += "\\0";  // Replaces NUL with "\\0";
            } else {
                result += *ch;
            }
        }

        return result;
    }

    // The string representation of the values received in EqFailure() are already
    // escaped. Split them on escaped '\n' boundaries. Leave all other escaped
    // characters the same.
    std::vector<std::string> SplitEscapedString(const std::string& str) {
        std::vector<std::string> lines;
        size_t start = 0, end = str.size();
        if (end > 2 && str[0] == '"' && str[end - 1] == '"') {
            ++start;
            --end;
        }
        bool escaped = false;
        for (size_t i = start; i + 1 < end; ++i) {
            if (escaped) {
                escaped = false;
                if (str[i] == 'n') {
                    lines.push_back(str.substr(start, i - start - 1));
                    start = i + 1;
                }
            } else {
                escaped = str[i] == '\\';
            }
        }
        lines.push_back(str.substr(start, end - start));
        return lines;
    }

    // Constructs and returns the message for an equality assertion
    // (e.g. ASSERT_EQ, EXPECT_STREQ, etc) failure.
    //
    // The first four parameters are the expressions used in the assertion
    // and their values, as strings.  For example, for ASSERT_EQ(foo, bar)
    // where foo is 5 and bar is 6, we have:
    //
    //   lhs_expression: "foo"
    //   rhs_expression: "bar"
    //   lhs_value:      "5"
    //   rhs_value:      "6"
    //
    // The ignoring_case parameter is true if and only if the assertion is a
    // *_STRCASEEQ*.  When it's true, the string "Ignoring case" will
    // be inserted into the message.
    AssertionResult EqFailure(const char* lhs_expression, const char* rhs_expression, const std::string& lhs_value,
                              const std::string& rhs_value, bool ignoring_case) {
        Message msg;
        msg << "Expected equality of these values:";
        msg << "\n  " << lhs_expression;
        if (lhs_value != lhs_expression) {
            msg << "\n    Which is: " << lhs_value;
        }
        msg << "\n  " << rhs_expression;
        if (rhs_value != rhs_expression) {
            msg << "\n    Which is: " << rhs_value;
        }

        if (ignoring_case) {
            msg << "\nIgnoring case";
        }

        if (!lhs_value.empty() && !rhs_value.empty()) {
            const std::vector<std::string> lhs_lines = SplitEscapedString(lhs_value);
            const std::vector<std::string> rhs_lines = SplitEscapedString(rhs_value);
            if (lhs_lines.size() > 1 || rhs_lines.size() > 1) {
                msg << "\nWith diff:\n" << edit_distance::CreateUnifiedDiff(lhs_lines, rhs_lines);
            }
        }

        return AssertionFailure() << msg;
    }
}  // namespace mytesting