##
# info2header.pl
#
# by Andy W. Schmeder <andy@a2hd.com> 2005
#
# input must be a series of lines in the format:
# KEY: "Value"
#
# Keys containing spaces will be ignored.
#
# run as "perl info2header.pl < test.info.txt > test.info.h" 
#     or "echo test.info.txt | perl info2header.pl > test.info.h"
#

my $source = '';

while(<STDIN>) {
  $source .= $_
}

while($source =~ m~^([A-Za-z0-9_\.-]*)\s*?:[ ]*([^\n]*)\s*~mg) {
    my ($key, $value) = ($1, $2);
    print qq~#ifndef $key\n~;
    print qq~#define $key $value\n~; 
    print qq~#endif\n~;
}
