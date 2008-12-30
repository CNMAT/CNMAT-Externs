#!/usr/bin/perl
=pod
    cmmjl_commonsymbols.pl

    Parses cmmjl_commonsymbols.c and creates an approprate header file based on the contents of the cmmjl_common_strings[] array.

=cut
use strict;

my $dir = shift;
if(!$dir){
    $dir = ".";
}elsif(rindex($dir, "/") == length($dir) - 1){
    chop($dir);
}

my $fin = "$dir/cmmjl_commonsymbols.c";
my $fout = "$dir/cmmjl_commonsymbols.h";

my @symbols;

open(FIN, "<", $fin) or die("Couldn't open $fin for reading\n");
open(FOUT, ">", $fout) or die("Couldn't open $fout for writing\n");

print(FOUT "/** \@file $fout\n");
print(FOUT "\tA set of commonly used predefined t_symbols.\n\n");
print(FOUT "\tWARNING: THIS FILE IS GENERATED AUTOMATICALLY BY cmmjl_commonsymbols.pl.\n\tDO NOT EDIT.\n\n");
print(FOUT "*/\n\n");

print(FOUT "#ifndef __CMMJL_COMMONSYMBOLS_H__\n");
print(FOUT "#define __CMMJL_COMMONSYMBOLS_H__\n\n");

print(FOUT "#include \"ext.h\"\n\n");

print(FOUT "typedef struct _cmmjl_common_symols_table{\n");
foreach(<FIN>) {
    if($_ =~ /^\s*\"(.*)\",?$/){
	push(@symbols, $1);
	print(FOUT "\tt_symbol *s_$1;\n");
    }
}

print(FOUT "} t_cmmjl_common_symbols_table;\n\n");

foreach(@symbols){
    print(FOUT "#define _$_ (_cmmjl_common_symbols->s_$_)\n");
}

print(FOUT "\n");

print(FOUT "/** The common symbols structure.  This shouldn't be accessed directly, but rather by using the symbols defined above. */\n");
print(FOUT "extern t_cmmjl_common_symbols_table *_cmmjl_common_symbols;\n\n");

print(FOUT "/** Initialize the symbol table by calling this. */\n");
print(FOUT "void cmmjl_common_symbols_init(void);\n\n");

print(FOUT "#endif\n");
