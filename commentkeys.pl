##
# commentkeys.pl
#
# by Andy W. Schmeder <andy@a2hd.com> 2005
#
# run as "perl commentkeys.pl < test.c > test.info.c" 
#     or "echo test.c | perl commentkeys.pl > test.info.txt"
#

my $source = '';

while(<STDIN>) {
  $source .= $_
}

while($source =~ m~/\*((\n|.)*?)\*/~g) {
    my $block_comment = $1;
    while($block_comment =~ m~\s*\@\@+\s*\n?((\n|.)*?)\n?\s*\@\@+~mg) {
        my $info_block = $1;
	while($info_block =~ m~\s*\*?\s*([A-Za-z0-9_\.-]*)\s*?:[ ]*([^\n]*)\s*~mg) {
	    my ($key, $value) = ($1, $2);
	    $value =~ s~\\~\\\\~g;
	    $value =~ s~"~\\"~g;
	    print qq~#define $key "$value"\n~; 
	}
    }
}
