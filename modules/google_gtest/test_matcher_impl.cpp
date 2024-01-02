#include <array>
#include <tuple>

#include <gtest/gtest.h>

// #include <gmock/gmock.h>

namespace testing_reimpl {

namespace internal {

// A macro to disallow copy operator=
// This should be used in the private: declarations for a class.
#define GTEST_DISALLOW_ASSIGN_(type) type& operator=(type const&) = delete

// Implements ElementsAre.
template <typename MatcherTuple>
class ElementsAreMatcher {
  public:
    explicit ElementsAreMatcher(const MatcherTuple& args) : matchers_(args) {}

    // 返回一个 Matcher 对象
    template <typename Container>
    operator Matcher<Container>() const {
        GTEST_COMPILE_ASSERT_(!IsHashTable<GTEST_REMOVE_REFERENCE_AND_CONST_(Container)>::value ||
                                ::std::tuple_size<MatcherTuple>::value < 2,
                              use_UnorderedElementsAre_with_hash_tables);

        typedef GTEST_REMOVE_REFERENCE_AND_CONST_(Container) RawContainer;
        typedef typename internal::StlContainerView<RawContainer>::type View;
        typedef typename View::value_type Element;
        typedef ::std::vector<Matcher<const Element&>> MatcherVec;
        MatcherVec matchers;
        matchers.reserve(::std::tuple_size<MatcherTuple>::value);
        TransformTupleValues(CastAndAppendTransform<const Element&>(), matchers_, ::std::back_inserter(matchers));
        // 创建一个 ElementsAreMatcherImpl 的对象
        return Matcher<Container>(new ElementsAreMatcherImpl<const Container&>(matchers.begin(), matchers.end()));
    }

  private:
    const MatcherTuple matchers_;
};
}  // namespace internal

template <typename... Args>
internal::ElementsAreMatcher<std::tuple<typename std::decay<const Args&>::type...>> ElementsAre(
  const Args&... matchers) {
    return internal::ElementsAreMatcher<std::tuple<typename std::decay<const Args&>::type...>>(
      std::make_tuple(matchers...));
}

// gmock-machers.h
#define EXPECT_THAT(value, matcher) \
    EXPECT_PRED_FORMAT1(::testing::internal::MakePredicateFormatterFromMatcher(matcher), value)

}  // namespace testing_reimpl

TEST(matcher_impl, reimpl) {
    // construction uses aggregate initialization
    std::array<int, 3> arr{{1, 2, 3}};  // double-braces required in C++11 prior to
                                        // the CWG 1270 revision (not needed in C++11
                                        // after the revision and in C++14 and beyond)

    EXPECT_EQ(3, arr.size());
    // EXPECT_THAT(arr, gtest_reimpl::ElementsAre(1, 2, 3));

    auto matcher = testing_reimpl::internal::MakePredicateFormatterFromMatcher(testing_reimpl::ElementsAre(1, 2, 3));
}
