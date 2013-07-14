#!/usr/bin/perl

use warnings;
use strict;

# Usage:
#   cov_check.pl <expected coverage percentage> <index.html to analyze>


my $min_cov = shift;

while (<>) {
    if ( my ($cov) = /<td class="headerCovTableEntry\S+">(\d*\.\d*) %<\/td>/ ) {
        if ($cov < $min_cov) {
            print "The coverage: $cov% is NOT enough, it should be at least $min_cov%\n";
            exit(1);
        }
        else {
            print "The coverage: $cov% exceeds the expected $min_cov%, OK :)\n";
            exit(0)
        }
    }
}
