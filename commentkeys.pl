##
# commentkeys.pl
#
# by Andy W. Schmeder <andy@a2hd.com> 2005
#
# run as "perl commentkeys.pl < test.c > test.info.c" 
#     or "cat test.c | perl commentkeys.pl > test.info.txt"
#

my @versions = ();
my $source = '';

while(<STDIN>) {
  $source .= $_
  }

# natural-order sort with '.' separator and '-' tags
sub version_sort {
    # workaround for 0.01, 0.02 case etc which is otherwise indistinguishable from 0.1, 0.2
    $a =~ s/\.0(\d)/\.0\.$1/;
    $b =~ s/\.0(\d)/\.0\.$1/;

    my @av = split /\./, $a;
    my @bv = split /\./, $b;

    my $i = 0;
    while ($i < @av && $i < @bv) {
	my ($anum, $atag) = split /-/, $av[$i];
	my ($bnum, $btag) = split /-/, $bv[$i];

	if(int($anum) < int($bnum)) {
	    return -1;
	} elsif(int($anum) > int($bnum)) {
	    return 1;
	} else {
	    if($atag && ! $btag) {  # no tag sorts higher
		return -1;
	    }
	    elsif($btag && ! $atag) {
		return 1;
	    } elsif($atag && $btag) {
		return $atag < $btag ? -1 : 1
		}
	}
	$i++;
    }

    return 0;
}

while($source =~ m~/\*((\n|.)*?)\*/~g) {
    my $block_comment = $1;
    while($block_comment =~ m~\s*\@\@+\s*\n?((\n|.)*?)\n?\s*\@\@+~mg) {
      my $info_block = $1;
	while($info_block =~ m~\s*([A-Za-z0-9_\. -]*)\s*?:\s*([^\n]*)\s*~mg) {
	    my ($key, $value) = ($1, $2);
	    $value =~ s~\\~\\\\~g;
	    $value =~ s~"~\\"~g;
	    if($key =~ /^VERSION(\s|-)*(.*)$/i) {
		push @versions, $2;
	    }
          $key =~ s/[^\x20-\x7e]//;    # remove any weird control characters that might have snuck in!
          $value =~ s/[^\x20-\x7e]//;
          print qq~$key: "$value"\n~;
	}
    }
}

@versions = reverse sort version_sort @versions;

print qq~VERSION: "$versions[0]"\n~;

