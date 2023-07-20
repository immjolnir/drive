#!/usr/bin/env bash
set -e

# 0. clear it
#rm -rf debug-build

# 1. Configure the full test build
#cmake -B debug-build -S . -DCMAKE_BUILD_TYPE=Debug -DCATCH_ENABLE_COVERAGE=ON --preset all-tests

# 2. Run the actual build
#cmake --build debug-build

#pushd debug-build

# 4. Run the tests using CTest
#ctest -j 4 --output-on-failure -C Debug

# Clear any prior counters; see http://ltp.sourceforge.net/coverage/lcov/readme.php for
# a little guide.
#lcov --zerocounters --directory $script_dir/build/debug

# 1. Collect coverage info
lcov -c -d . -o raw_coverage.info

# 4.1 List raw files, List contents of tracefile FILE
lcov -l raw_coverage.info

# 4.2 Remove files matching PATTERN from FILE
lcov --remove raw_coverage.info \
    '/usr/include/*' \
    '/usr/lib/*' \
    '/usr/local/cuda/*' \
    '*/test_*'  \
    -o coverage.info

# 4.3 List desired tracefile
lcov -l coverage.info

# 5. Create HTML output for coverage data found in INFOFILE
genhtml --highlight --legend --output-directory coverage_report coverage.info

echo "Please now open coverage_report/index.html in your favorite browser."
#popd
