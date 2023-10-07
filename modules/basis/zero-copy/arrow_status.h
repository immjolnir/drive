#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

// cpp/src/arrow/util/macros.h
// From Google gutil
#ifndef ARROW_DISALLOW_COPY_AND_ASSIGN
#define ARROW_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;          \
    void operator=(const TypeName&) = delete
#endif

#ifndef ARROW_DEFAULT_MOVE_AND_ASSIGN
#define ARROW_DEFAULT_MOVE_AND_ASSIGN(TypeName) \
    TypeName(TypeName&&) = default;             \
    TypeName& operator=(TypeName&&) = default
#endif

//
// GCC can be told that a certain branch is not likely to be taken (for
// instance, a CHECK failure), and use that information in static analysis.
// Giving it this information can help it optimize for the common case in
// the absence of better information (ie. -fprofile-arcs).
//
// https://stackoverflow.com/questions/42281658/what-is-the-builtin-expect-prototype
//
#if defined(__GNUC__)
#define ARROW_PREDICT_FALSE(x) (__builtin_expect(!!(x), 0))
#define ARROW_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#define ARROW_NORETURN __attribute__((noreturn))
#define ARROW_NOINLINE __attribute__((noinline))
#define ARROW_PREFETCH(addr) __builtin_prefetch(addr)
#else
#define ARROW_NORETURN
#define ARROW_PREDICT_FALSE(x) (x)
#define ARROW_PREDICT_TRUE(x) (x)
#define ARROW_PREFETCH(addr)
#endif

namespace arrow
{
    namespace util
    {
        // cpp/src/arrow/util/string_builder.h
        namespace detail
        {

            class StringStreamWrapper {
              public:
                StringStreamWrapper() : sstream_(std::make_unique<std::ostringstream>()), ostream_(*sstream_) {}

                ~StringStreamWrapper() {}

                std::ostream& stream() { return ostream_; }

                std::string str() { return sstream_->str(); }

              protected:
                std::unique_ptr<std::ostringstream> sstream_;
                std::ostream& ostream_;
            };
        }  // namespace detail

        template <typename Head>
        void StringBuilderRecursive(std::ostream& stream, Head&& head) {
            stream << head;
        }

        template <typename Head, typename... Tail>
        void StringBuilderRecursive(std::ostream& stream, Head&& head, Tail&&... tail) {
            StringBuilderRecursive(stream, std::forward<Head>(head));
            StringBuilderRecursive(stream, std::forward<Tail>(tail)...);
        }

        template <typename... Args>
        std::string StringBuilder(Args&&... args) {
            detail::StringStreamWrapper ss;
            StringBuilderRecursive(ss.stream(), std::forward<Args>(args)...);
            return ss.str();
        }

        /// CRTP helper for declaring string representation. Defines operator<<
        template <typename T>
        class ToStringOstreamable {
          public:
            ~ToStringOstreamable() {
                static_assert(std::is_same<decltype(std::declval<const T>().ToString()), std::string>::value,
                              "ToStringOstreamable depends on the method T::ToString() const");
            }

          private:
            const T& cast() const { return static_cast<const T&>(*this); }

            friend inline std::ostream& operator<<(std::ostream& os, const ToStringOstreamable& t) {
                return os << t.cast().ToString();
            }
        };

        // cpp/src/arrow/util/compare.h
        /// CRTP helper for declaring equality comparison. Defines operator== and operator!=
        template <typename T>
        class EqualityComparable {
          public:
            ~EqualityComparable() {
                static_assert(
                  std::is_same<decltype(std::declval<const T>().Equals(std::declval<const T>())), bool>::value,
                  "EqualityComparable depends on the method T::Equals(const T&) const");
            }

            template <typename... Extra>
            bool Equals(const std::shared_ptr<T>& other, Extra&&... extra) const {
                if (other == nullptr) {
                    return false;
                }
                return cast().Equals(*other, std::forward<Extra>(extra)...);
            }

            struct PtrsEqual {
                bool operator()(const std::shared_ptr<T>& l, const std::shared_ptr<T>& r) const {
                    return l->Equals(*r);
                }
            };

            friend bool operator==(T const& a, T const& b) { return a.Equals(b); }

            friend bool operator!=(T const& a, T const& b) { return !(a == b); }

          private:
            const T& cast() const { return static_cast<const T&>(*this); }
        };

    }  // namespace util

    // arrow/cpp/src/arrow/status.h
    enum class StatusCode : char {
        OK = 0,
        OutOfMemory = 1,
        KeyError = 2,
        TypeError = 3,
        Invalid = 4,
        IOError = 5,
        CapacityError = 6,
        IndexError = 7,
        Cancelled = 8,
        UnknownError = 9,
        NotImplemented = 10,
        SerializationError = 11,
        RError = 13,
        // Gandiva range of errors
        CodeGenError = 40,
        ExpressionValidationError = 41,
        ExecutionError = 42,
        // Continue generic codes.
        AlreadyExists = 45
    };

    class Status : public util::ToStringOstreamable<Status>, public util::EqualityComparable<Status> {
      public:
        // Create a success status.
        constexpr Status() noexcept : state_(nullptr) {}

        Status(StatusCode code, const std::string& msg) {
            state_ = new State;
            state_->code = code;
            state_->msg = std::move(msg);
        }

        ~Status() noexcept {
            // ARROW-2400: On certain compilers, splitting off the slow path improves
            // performance significantly.
            if (ARROW_PREDICT_FALSE(state_ != NULL)) {
                DeleteState();
            }
        }

        /// Return a success status
        static Status OK() { return Status(); }

        template <typename... Args>
        static Status FromArgs(StatusCode code, Args&&... args) {
            return Status(code, util::StringBuilder(std::forward<Args>(args)...));
        }

        /// Return an error status for out-of-memory conditions
        template <typename... Args>
        static Status OutOfMemory(Args&&... args) {
            return Status::FromArgs(StatusCode::OutOfMemory, std::forward<Args>(args)...);
        }

        /// Return an error status for failed key lookups (e.g. column name in a table)
        template <typename... Args>
        static Status KeyError(Args&&... args) {
            return Status::FromArgs(StatusCode::KeyError, std::forward<Args>(args)...);
        }

        /// Return an error status for type errors (such as mismatching data types)
        template <typename... Args>
        static Status TypeError(Args&&... args) {
            return Status::FromArgs(StatusCode::TypeError, std::forward<Args>(args)...);
        }

        /// Return an error status when some IO-related operation failed
        template <typename... Args>
        static Status IOError(Args&&... args) {
            return Status::FromArgs(StatusCode::IOError, std::forward<Args>(args)...);
        }

        /// Return an error status when an index is out of bounds
        template <typename... Args>
        static Status IndexError(Args&&... args) {
            return Status::FromArgs(StatusCode::IndexError, std::forward<Args>(args)...);
        }

        /// Return true iff the status indicates success.
        constexpr bool ok() const { return (state_ == nullptr); }

        /// Return true iff the status indicates an out-of-memory error.
        constexpr bool IsOutOfMemory() const { return code() == StatusCode::OutOfMemory; }

        /// Return true iff the status indicates a key lookup error.
        constexpr bool IsKeyError() const { return code() == StatusCode::KeyError; }

        /// Return true iff the status indicates an out of bounds index.
        constexpr bool IsIndexError() const { return code() == StatusCode::IndexError; }

        /// Return true iff the status indicates a type error.
        constexpr bool IsTypeError() const { return code() == StatusCode::TypeError; }

        /// Return true iff the status indicates an IO-related failure.
        constexpr bool IsIOError() const { return code() == StatusCode::IOError; }

        /// \brief Return the StatusCode value attached to this status.
        constexpr StatusCode code() const { return ok() ? StatusCode::OK : state_->code; }

        /// \brief Return the specific error message attached to this status.
        const std::string& message() const {
            static const std::string no_message = "";
            return ok() ? no_message : state_->msg;
        }

        std::string CodeAsString(StatusCode code) const {
            const char* type;
            switch (code) {
                case StatusCode::OK:
                    type = "OK";
                    break;
                case StatusCode::OutOfMemory:
                    type = "Out of memory";
                    break;
                case StatusCode::KeyError:
                    type = "Key error";
                    break;
                case StatusCode::TypeError:
                    type = "Type error";
                    break;
                case StatusCode::Invalid:
                    type = "Invalid";
                    break;
                case StatusCode::Cancelled:
                    type = "Cancelled";
                    break;
                case StatusCode::IOError:
                    type = "IOError";
                    break;
                case StatusCode::CapacityError:
                    type = "Capacity error";
                    break;
                case StatusCode::IndexError:
                    type = "Index error";
                    break;
                case StatusCode::UnknownError:
                    type = "Unknown error";
                    break;
                case StatusCode::NotImplemented:
                    type = "NotImplemented";
                    break;
                case StatusCode::SerializationError:
                    type = "Serialization error";
                    break;
                case StatusCode::CodeGenError:
                    type = "CodeGenError in Gandiva";
                    break;
                case StatusCode::ExpressionValidationError:
                    type = "ExpressionValidationError";
                    break;
                case StatusCode::ExecutionError:
                    type = "ExecutionError in Gandiva";
                    break;
                default:
                    type = "Unknown";
                    break;
            }
            return std::string(type);
        }

        std::string CodeAsString() const {
            if (state_ == nullptr) {
                return "OK";
            }
            return CodeAsString(code());
        }

        std::string ToString() const {
            std::string result(CodeAsString());
            if (state_ == nullptr) {
                return result;
            }
            result += ": ";
            result += state_->msg;

            return result;
        }

        bool Equals(const Status& s) const {
            if (state_ == s.state_) {
                return true;
            }

            if (ok() || s.ok()) {
                return false;
            }

            return code() == s.code() && message() == s.message();
        }

      private:
        struct State {
            StatusCode code;
            std::string msg;
            // std::shared_ptr<StatusDetail> detail;
        };

        // OK status has a `NULL` state_.  Otherwise, `state_` points to
        // a `State` structure containing the error code and message(s)
        State* state_;

        void DeleteState() {
            delete state_;
            state_ = nullptr;
        }
    };
}  // namespace arrow
