#!/bin/bash

# Usage:
#   ./run_test.sh <TEST_BINARY>

pre="\E[00;32m"
post="\E[00;00m"

echo -e "${pre}Reset coverage files${post}"
lcov --directory . -z
rm -f ./lcov.info

echo -e "${pre}Run tests${post}"
valgrind --log-file=leak.log --leak-check=full --show-reachable=yes --show-below-main=no  \
--track-origins=yes --num-callers=30 --malloc-fill=0xaa --free-fill=0xdd \
--suppressions=valgrind.supp  $1


echo -e "${pre}Capture coverage info${post}"
lcov --directory ../build --capture -o lcov.info


echo -e "${pre}Filtering coverage tracefile${post}"
lcov -r lcov.info "g++-v*" -o lcov.info
lcov -r lcov.info "/usr/include/cxxtest*" -o lcov.info

echo -e "${pre}Generating HTML${post}"
rm -rf ./cov
mkdir cov
genhtml --frames --legend -o ./cov lcov.info

echo -e "${pre}Checking the coverage results${post}"
./cov_check.pl 90 cov/index.html

echo -e "${pre}Checking leak results${post}"
./leak_check.pl leak.log
