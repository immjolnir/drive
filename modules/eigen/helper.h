// verifyIsApprox is a wrapper to test_isApprox that outputs the relative difference magnitude if the test fails.
template <typename Type1, typename Type2>
inline bool verifyIsApprox(const Type1& a, const Type2& b) {
    bool ret = test_isApprox(a, b);
    if (!ret) {
        std::cerr << "Difference too large wrt tolerance " << get_test_precision(a)
                  << ", relative error is: " << test_relative_error(a, b) << std::endl;
    }
    return ret;
}