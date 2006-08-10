/*
Copyright (c) 2006.
The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

*/

import com.cycling74.max.*;

public class tc_editor extends MaxObject
{
	public void version() {
		post("tc_editor version 0.11  - fixes row 0 bug");
	}

	private static final String[] INLET_ASSIST = new String[]{
		"from jit.cellblock first and third outlets"
	};
	private static final String[] OUTLET_ASSIST = new String[]{
		"to jit.cellblock inlet",
		"OSC Messages"
	};

	private java.util.ArrayList contents;
	private int nrows;
	private int ncols;
	private int new_nrows;
	private int new_ncols;
	private int selected_row = 0;
	private int selected_col = 0;

	private boolean last_command_was_input = false;
	private int last_import_col;
	private int last_import_row;



	// user-settable modes
	private boolean auto_order = true;
	private boolean verbose = false;
	private int first_data_row = 1;


	private int dump_mode;		// How to handle incoming dumped data
		private static final int IGNORE = 0;
		private static final int CHECK_SIZE = 1;
		private static final int GOBBLE = 2;




	java.util.TreeSet all_start_times;
	
	public tc_editor(Atom[] args)
	{
		declareInlets(new int[]{DataTypes.ALL});
		declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL});
		createInfoOutlet(false);

		setInletAssist(INLET_ASSIST);
		setOutletAssist(OUTLET_ASSIST);

		dump_mode = IGNORE;
		nrows = 10;
		ncols = 5;

		make_contents();
		all_start_times = new java.util.TreeSet();
		version();	
	}
    
	private class cell {
		public Atom[] command;		// Max data in the cell
		public float duration;		// Time taken by command
		public float start_time;	// In the context of this column, the time this command will start
		public cell (Atom[] args) {
			command = args;
			
			duration = 0;
			start_time = -1;

			if (args.length == 0) {
				post("Ack! cell has no data");
			} else if (!args[0].isString()) {
				// Lists of numbers are implicitly ramp commands
				if (args.length < 3) {
					// Implied jump
					duration = 0;
				} else if (args.length == 3) {
					if (args[2].isString()) {
						post("Ack! Raw list's third element must be a number");
					} else {
						duration = args[2].toFloat();
					}
				} else {
					post("Ack!  Too many elements in list");
				}
			} else if (args[0].getString().equalsIgnoreCase("jump")) {
				duration = 0;
			} else if (args[0].getString().equalsIgnoreCase("hold")) {
				if (args.length == 1) {
					post("Ack! Need an argument to HOLD");
				} else if (args[1].isString()) {
					post("Ack! Argument to HOLD must be a number");
				} else {
					duration = args[1].toFloat();
				}
			} else if (args[0].getString().equalsIgnoreCase("ramp")) {
				if (args.length < 4) {
					// Implied jump
					duration = 0;
				} else if (args.length == 4) {
					if (args[3].isString()) {
						post("Ack! JUMP's third argument must be a number");
					} else {
						duration = args[3].toFloat();
					}
				} else {
					post("Ack!  Too many arguments to JUMP!");
				}
			} else {
				// post("Ack! Unrecognized tempocurver command: " + args[0].getString());
			}
		}
	}

	private void make_contents() {
		// Make an ArrayList of ArrayLists to mirror what's in the jit.cellblock
		// Put a "null" in each element.
		contents = new java.util.ArrayList(ncols);
		 for (int i = 0; i < ncols; ++i) {
			 contents.add(make_column());
		 }
	 }

	private void resize_contents() {
		if (contents.size() < ncols) {
			add_cols(ncols);
		}
		add_rows(nrows);
	}

	 private java.util.ArrayList make_column() {
		 java.util.ArrayList col = new java.util.ArrayList(nrows);
		 for (int j = 0; j < nrows; ++j) {
			 col.add(null);
		 }	
		 return col;
	 }	

	 private void add_cols(int larger_ncols) {
		 contents.ensureCapacity(larger_ncols);
		 for (int i = contents.size(); i < larger_ncols; ++i) {
			 contents.add(make_column());
		 }
		 ncols = larger_ncols;
	 }

	 private void add_rows(int larger_nrows) {
		 for (int i = 0; i<ncols; ++i) {
			 java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
			 col.ensureCapacity(larger_nrows);
			 for (int j = col.size(); j < larger_nrows; ++j) {
				 col.add(null);
			 }
		 }
		 nrows = larger_nrows;
	 }

	 private void clear_contents() {
		for (int i = 0; i < ncols; ++i) {
			clear_column(i, 0);	
		}
	}

	public void clearall() {
		clear_contents();
		outlet(0, "clear", new Atom[] { Atom.newAtom("all")});
	}

	private void clear_column(int i, int rowsToKeep) {
		java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
		if (col != null) {
			for (int j = rowsToKeep; j < col.size(); ++j) {
				col.set(j,null);
			}
		}
	}

	public void print_contents() {
		last_command_was_input = false;
		post("Printing contents: " + ncols + " columns, " + nrows + "rows.");
		for (int i = 0; i < ncols; ++i) {
			java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
			for (int j = 0; j < nrows; ++j) {
				cell c =  (cell) col.get(j);
				post("[" + i + "," + j + "] " +
					 ((c==null) ? "[null]" : Atom.toDebugString(c.command)));
			}	
		}
	}

	public int highest_row_in_use() {
		int answer = -1;
		// Skip column 0, since it's just times that this object writes
		for (int i = 1; i<ncols; ++i) {
			java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
			for (int j = answer+1; j < col.size(); ++j) {
				cell c =  (cell) col.get(j);
				if (c != null) {
					if (j > answer) answer = j;
				}
			}
		}

		if (verbose) post("Highest row in use is " + answer);
		return answer;
	}


	private void store(int col, int row, Atom[] data) {
		if (col < 0 || row < 0) {
			post("Ack! column number ("+col+") and row number ("+row+") must be nonnegative!");
			return;
		}

		if (row>nrows) {
			post("Ack!  Trying to store row " + row + " but data only has " + nrows);
			return;
		}
		if (col > ncols) {
			post("Ack!  Trying to store column " + col + " but data only has " + ncols);
			return;
		}

		Object o = null;
		if  (data.length > 0) {
			o =  new cell(data);
		}

	    ((java.util.ArrayList) contents.get(col)).set(row, o);
	}


	public void verbose(int v) {
		last_command_was_input = false;
		verbose = (v != 0);
		post("verbose mode " + verbose);
	}


	public void autoorder(int a) {
		last_command_was_input = false;
		auto_order = (a != 0);
		post("automatic reordering mode " + auto_order);
	}

	public void labelrow(int l) {
		last_command_was_input = false;
		if (l == 0) {
			first_data_row = 0;
		} else {
			first_data_row = 1;
		}
		post("labelrow " + first_data_row);
	}

	public void bang()
	{
		last_command_was_input = false;
		post("bung!");
	}

	public void dumpout(Atom[] args) {
		// post("dumpout called with " + Atom.toDebugString(args));

		if (args.length < 3) {
			post("Ack: too few arguments to dumpout");
			return;
		}

		if (!args[0].isInt() || !args[1].isInt()) {
			post("Ack: first two args to dumpout must be ints");
			return;
		}

		int col = args[0].getInt();
		int row = args[1].getInt();

		Atom[] data = Atom.removeFirst(args,2);


		// post("dumpout " + col + "," + row);

		switch (dump_mode) {
			case IGNORE: // do nothing
				break;
			case CHECK_SIZE:
				// post("Checking size");
				if (row >= new_nrows) {
					// post("more rows");
				    new_nrows = row+1;
				}
				if (col >= new_ncols) {
					// post("more columns");
					new_ncols = col+1;
				}
				break;
			case GOBBLE:
				store(col, row, data);
				break;

			default:
				post("Unrecognized dump mode " + dump_mode);
		}
	}

				

	public void take_dump() {
		clear_contents();
		// post("Before:  size (r=" + nrows + ", c=" + ncols + ")");

		dump_mode = CHECK_SIZE;
		new_nrows = 0;
		new_ncols = 0;
		outlet(0, "dump");
		// Now dumpout() will be called many times

		// post("After dump, new_nrows=" + new_nrows +", new_ncols=" + new_ncols);

		/* Check nrows first, cause it's more efficient to call add_rows() before
		   add_cols() than the reverse. */
		if (new_nrows > nrows) {
			add_rows(new_nrows);
		}
		if (new_ncols > ncols) {
			add_cols(new_ncols);
		}

		// post("After:  size (r=" + nrows + ", c=" + ncols + ")");


		dump_mode = GOBBLE;
		outlet(0, "dump");
		// Now dumpout() will be called many times
	}	

	public void insert() {
		last_command_was_input = false;

		// First refresh our knowledge of the contents of the cellblock
		take_dump();

		if (selected_col < 0 || selected_col >= ncols) {
			post("Ack!  selected column " + selected_col + " is illegal!");
			return;
		}

		// Get the one column we care about
		java.util.ArrayList col = (java.util.ArrayList) contents.get(selected_col);

		// post("Inserting:  size (r=" + nrows + ", c=" + ncols + ")");



		// Move everything below the selected cell down by one (to the next higher row)
		for (int i = selected_row+1; i < nrows; ++i) {
			cell c = (cell) col.get(i-1);

			if (c==null) {
				// Clear the ith row of this column
				outlet_clear_cell(selected_col, selected_row);
			} else {
				outlet_set_cell(selected_col, i, c);
			}
		}

		// And now we can put the new value ("hold 0.") in the selected cell
		outlet(0, "set", new Atom[] { Atom.newAtom(selected_col),
									  Atom.newAtom(selected_row),
									  Atom.newAtom("hold"),
									  Atom.newAtom(0.)} );

		// This may have changed the order
		if (auto_order) order();
	}
	
	private void outlet_clear_cell(int col, int row) {
		outlet(0, "clear", new Atom[] { Atom.newAtom(col),
							            Atom.newAtom(row) });
	}

	private void outlet_set_cell(int col, int row, cell c) {
		// Construct the Atom[] array to set a cell to an (arbitrary-sized) value
		// We need the user's arbitrary stored command,
		//  plus the new row and column to store it in
		// It's fun to concatenate arrays in Java!

		Atom[] command =  c.command;
		Atom[] toOutput = new Atom[2+command.length];
		toOutput[0] = Atom.newAtom(col);
		toOutput[1] = Atom.newAtom(row);
		for (int j = 0; j < command.length; ++j) {
			toOutput[j+2] = command[j];
		}
		// post("Gonna output " + Atom.toDebugString(toOutput));
		outlet(0, "set", toOutput);
	}

	private void refresh_cellblock() {
		if (verbose) post("Writing everythign into the jit.cellblock");

		dump_mode = IGNORE;
		
		outlet(0, "rows", new Atom[] { Atom.newAtom(highest_row_in_use()+1) });


		for (int i = 1; i<ncols; ++i) { // col 0 is for the times this method write
			java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
			for (int j=0; j<col.size(); ++j) {
				cell c =  (cell) col.get(j);
				if (c==null) {
					outlet_clear_cell(i, j);
				} else {
					outlet_set_cell(i, j, c);
				}
			}
		}
	}


/*
	public void sync(Atom[] args) {
		if (args.length == 0) {
			post("didn't expect empty sync message");
		} else {
			if (args[0].isString()) {
				post("Don't know how to sync that " + args[0].getString());
			} else {
				post("Didn't expect sync message with numeric first argument");
			}
		}
	}
*/


	public void sync(String shouldBeClick, int a, int b, int c, int d) {
		last_command_was_input = false;

		if (shouldBeClick.equals("click")) {
			// Do nothing
		} else {
			post("Didn't expect sync " + shouldBeClick);
		}
	}

	public void sync(String shouldBeSelect, int c, int r) {
		last_command_was_input = false;
		if (shouldBeSelect.equals("select")) {
			selected_row = r;
			selected_col = c;
			// post("you selected r " + r + ", c " + c);
		} else {
			post("Didn't expect sync " + shouldBeSelect);
		}
	}

	public void refresh_selection() {
		last_command_was_input = false;
		outlet(0, "select", new Atom[] {Atom.newAtom(selected_col), Atom.newAtom(selected_row)});
	}
    
	public void inlet(int i)
	{
	}
    
	public void inlet(float f)
	{
	}
    
    
	public void list(Atom[] list) {
		// jit.cellblock apparently gets to output a list whose first element is "dumpout".  Grr.
		if (list.length > 0 && list[0].isString() && list[0].getString().equals("dumpout")) {
			dumpout(Atom.removeFirst(list,1));
		} else {
			if (auto_order) {
				// Look for messages that say a cell has a new value
				// They look like [int] [int] [something else]
				if (list.length >= 3 &&  list[0].isInt() && list[1].isInt()) {
				    last_command_was_input = false;
					if (verbose) post("about to auto_order...");
					order();
				}
			}
		}
	}


	public void order() {
		// First refresh our knowledge of the contents of the cellblock
		take_dump();

		// Find the start time of each command
		calculate_start_times();

		if (verbose) {
			post("All the start times: ");
			java.util.Iterator iter = all_start_times.iterator();
			while (iter.hasNext()) {
				float thisTime = ((java.lang.Float) iter.next()).floatValue();
				post("   " + thisTime);
			}
		}

		// Get rid of all the stored "null" values 
		// All the vertical spacing is going to be recomputed in this method anyway. 
		for (int i = 0; i<ncols; ++i) {
			java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
			while (col.remove(null)) {
				// keep going until it returns false
			}
		}

		// Now clear the entire cellblock
		outlet(0, "clear", new Atom[] { Atom.newAtom("all")});

		/* Here's the tricky part.  We iterate through the sorted set of times,
		   keeping track of our current index in each of the columns. */
		int indices[] = new int[ncols];
		java.util.Arrays.fill(indices, 0);
		
		java.util.Iterator iter = all_start_times.iterator();
		int outputRow = 0;

		while (iter.hasNext()) {
			float thisTime = ((java.lang.Float) iter.next()).floatValue();

			// Since "jump" commands (and unrecognized commands) take zero time,
			// there may need to be multiple output rows with the same time.
			boolean more_this_time = true;
			while (more_this_time) {
				// Put the absolute time in the leftmost column
				outlet(0, "set", new Atom[] { Atom.newAtom(0),
											  Atom.newAtom(outputRow),
											  Atom.newAtom(thisTime) });

				// So far we haven't seen any columns with more entries for this time.
				more_this_time = false;  

				for (int i = 1; i<ncols; ++i) { // col 0 is for the times this method write
					java.util.ArrayList col = (java.util.ArrayList) contents.get(i);

					if (indices[i] < col.size()) {
						cell c =  (cell) col.get(indices[i]);
						float next_time_in_column = c.start_time;
						if (next_time_in_column < thisTime) {
							post("Internal error!  next_time_in_column < thisTime");
							return;
						} else if (next_time_in_column == thisTime) {
							// This command belongs in the current row, since it has the right time
							outlet_set_cell(i, outputRow, c);
							(indices[i])++;

							// Is the next command in this column at the same start time?
							if (indices[i] < col.size() &&
								((cell) col.get(indices[i])).start_time == thisTime) {
								more_this_time = true;
							}
						} else {
							// This column should just have a blank in this row, so do nothing
						}
					}
				}

				outputRow++;
				outlet(0, "rows", new Atom[] { Atom.newAtom(outputRow+1) });
			}
		}

		// Now that we're done, restore the size of contents
		resize_contents();

		// This shouldn't be necessary, but it might help with the textedit box to display
		// the selected cell after doing a reorder.  Of course it should really be the same
		// *content* that used to be selected, not just the same row and column.
		// refresh_selection();  gives stack overflow
	}

    public void calculate_start_times() {
		all_start_times.clear();

		for (int i = 1; i<ncols; ++i) {
			java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
			float t = 0.0f;
			for (int j = 0; j < nrows; ++j) {
				cell c =  (cell) col.get(j);
				if (c != null) {
					c.start_time = t;
					all_start_times.add(new java.lang.Float(t));
					t += c.duration;
				}
			}
			// xxx t is now the time that the last command ends...
		}
	}

	public void outputall() {
		last_command_was_input = false;

		take_dump();
		for (int i = 1; i<ncols; ++i) {
			output(i);
		}
	}

	public void output(int column) {
		last_command_was_input = false;

		take_dump();
		do_output(column);
	}

	private void do_output(int column) {
		if (column < 0 || column >= ncols) {
			post("Can't output column " + column + "; they go from 0 to " + (ncols-1));
			return;
		}

		java.util.ArrayList col = (java.util.ArrayList) contents.get(column);
		if (col == null) {
			post("Error: column " + column + " is null!");
			return;
		}
	
		// clear the message box
		outlet(1, ("/"+column+"/msg"), new Atom[] {Atom.newAtom("set")});

		// output all commands in this column to the message box and the tempocurver object
		// row 0 is reserved for title (voice name or whatever)

		for (int i = first_data_row; i < col.size(); ++i) {
			cell c =  (cell) col.get(i);
			if (c != null) {
				
				outlet(1, ("/"+column+"/tc"), c.command);

				Atom[] toOutput = new Atom[c.command.length + 1];
				toOutput[0] = Atom.newAtom("append");
				for (int j=0; j<c.command.length; ++j) {
					toOutput[j+1] = c.command[j];
				}

				outlet(1, ("/"+column+"/msg"), toOutput);
				outlet(1, ("/"+column+"/msg"), new Atom[]{Atom.newAtom("append"),
													  Atom.newAtom(",") });
			}
		}
	}

	

	public void input(Atom [] column_plus_command) {
		if (column_plus_command.length < 2) {
			post("input: needs column number followed by a command to add to the column");
			return;
		}

		if (!column_plus_command[0].isInt()) {
			post("input: column number must be an integer (you gave " + 
				 Atom.toDebugString(column_plus_command) + ")");
			return;
		}

		int column = column_plus_command[0].getInt();
		Atom [] command = Atom.removeFirst(column_plus_command);


		if (verbose) {
			post("import " + column + 
				  (last_command_was_input ? " another" : " first"));
		}

		if (column < 1 || column >= contents.size()) {
			post("Can't import to column " + column + "; they go from 1 to " + (ncols-1));
			return;
		}

		java.util.ArrayList col = (java.util.ArrayList) contents.get(column);
		if (col == null) {
			post("Internal error!  Column " + column + " is null!");
			return;
		}

		if (verbose) {
		    post("Last inputted column was " + last_import_col + " now inputting " + column);
		}

		if (column != last_import_col) {
			last_command_was_input = false;				
			last_import_col = column;
		}

		if (!last_command_was_input) {
			clear_column(column, first_data_row);
			last_import_row =  first_data_row - 1;
			// post("** reset last_import_row  to " + last_import_row );
		}
		last_command_was_input = true;


		last_import_row++;
		
		// post("** Gonna write command to row " + last_import_row );
		col.set(last_import_row, new cell(command));

		refresh_cellblock();
		if (auto_order) order();
		
	}
}












