The lexer and parser contained in this folder don't work the way a normal flex/bison lexer/parser would.
Since we are working with Max, our input is partially tokenized by Max's lexer and handed to us
in the form of an atom array.  The problem is that, given input like this:

[o.if (/foo < 3.14159)]

we will end up with an array of atoms that looks like this:

(/foo
<
3.14159)

when what we want is

(
/foo
<
3.14159
)

Worse still are situations like:

[o.expr abs(atan(/foo - 3.14159))]

resulting in

abs(atan(/foo
-
3.14159))

or even worse:

[o.expr abs(atan(/foo-3.14159))]

etc...

So, the lexer here is meant to operate only on symbols found in the atom array to finish analyzing the
input.  The result is an atom array that is potentially longer than the original one that contains
separate atoms as in the example above.  The lexer that bison calls is much simpler and just returns
a code for each atom.

For user code in a Max object, in order to parse an atom array as in the examples above, one would do
something like

int argclex = 0;
t_atom *argvlex = NULL;
omax_scanner_scan_atom_array(argc, argv, &argclex, &argvlex);
int counter = 0;
omax_parse(argclex, argvlex, &counter);
...
osc_mem_free(argvlex);
