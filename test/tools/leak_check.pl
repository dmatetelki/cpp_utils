#!/usr/bin/env perl

use strict;
use warnings;

# usage:
#   leak_check.pl <log>


my $leaks  = 0;
my $errors = 0;

while (<>) {
    if (/definitely lost: ([0-9,]+) bytes in ([0-9]+) blocks/) { $leaks = $2; }
    if (/ERROR SUMMARY: (\d+) errors from (\d+) contexts/)  { $errors = $2;}
}

if ($leaks)  { print(sprintf("You have some leaks at %d places  :/\n", $leaks)); }
if ($errors) { print(sprintf("You have some memory corruptions at %d places  :/\n", $errors)); }

if (0 == $leaks && 0 == $errors) {
    print "Valgrind result are ok :)\n";
    exit(0);
}
exit(1);

