#!/bin/bash

# Usage: ./run_test.sh <TEST_BINARY>

pre="\E[00;32m"
post="\E[00;00m"

echo -e "${pre}Reset coverage files${post}"
lcov --directory . -z
rm -f ./lcov.info

echo -e "${pre}Run tests${post}"
$1

echo -e "${pre}Capture coverage info${post}"
lcov --directory . --capture -o lcov.info

echo -e "${pre}Filtering coverage tracefile${post}"
lcov -r lcov.info "g++-v*" -o lcov.info
lcov -r lcov.info "/usr/include/cxxtest*" -o lcov.info
lcov -r lcov.info "$(PWD)/test/*" -o lcov.info

echo -e "${pre}Generating HTML${post}"
rm -rf ./cov
mkdir cov
genhtml -o ./cov lcov.info
