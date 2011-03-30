#!/usr/bin/perl

use Term::ANSIColor qw(:constants);
$Term::ANSIColor::AUTORESET = 1;


while ( <> ) {

  if (/^(Thread \d+)/) {
     print "\n" . BOLD BLUE . $1 . RESET . " $'"
  }
  elsif ( /^(#\d+)\s+(\S+)\s+in\s+(.+)( \(.*)(at.*\/)(\S+):(\d+)/ )
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
  elsif ( /^(#\d+)\s+(\S+)\s+in\s+(.+)( \(.*)(from.*\/)(\S+)/ )
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