#!/usr/bin/perl

use strict;
use warnings;

use Term::ANSIColor qw(:constants);
$Term::ANSIColor::AUTORESET = 1;

my $line = "";
my $left;


while ( <> ) {



  if (/Segmentation fault/) {
    print "\n" . YELLOW . "Segmentation fault." . RESET . "\n";
    $left = 3;
    next;
  }

  if (/^(Thread \d+)/) {
    print "\n" . BOLD BLUE . $1 . RESET . " $'";
    next;
  }

  if ($left) {
    if ( $left == 3 ) {
      $line = $_;
      chomp($line);
      $left--;
      next;
    }
    elsif ( $left == 2 ) {
      $line .=  $_;
      $left--;
    }
    elsif ( $left == 1 ) {
      print BOLD YELLOW . $_ . RESET . "\n";
      $left--;
      next;
    }
  } else {
    $line = $_;
  }

  if ( $line =~ /^(#\d+)\s+(\S+)\s+in\s+(.+)( \(.*)(at.*\/)(\S+):(\d+)/ )
  {
      print "\n" .
            BOLD . $1 .          # no
            RESET . " $2 in " .  # mem in
            GREEN . $3 .         # function
            RESET . " $4\n$5" .  # args, path
            BOLD CYAN . "$6" .   # filename
            RESET . ":" .
            YELLOW . $7 .        # line
            RESET . "\n";
  }
  elsif ( $line =~ /^(#\d+)\s+(\S+)\s+in\s+(.+)( \(.*)(from.*\/)(\S+)/ )
  {
      print "\n" .
            BOLD . $1 .          # no
            RESET . " $2 in " .  # mem in
            GREEN . $3 .         # function
            RESET . " $4\n$5" .  # args, path
            BLUE . "$6" .        # filename
            RESET . "\n";
    }

}