import com.cycling74.max.*;

public class tc_editor extends MaxObject
{
	public void version() {
		post("tc_editor version 0.07 -  auto-order");
	}

	private static final String[] INLET_ASSIST = new String[]{
		"from jit.cellblock first outlet",
		"from jit.cellblock third outlet"
	};
	private static final String[] OUTLET_ASSIST = new String[]{
		"to jit.cellblock inlet"
	};

	private java.util.ArrayList contents;
	private int nrows;
	private int ncols;
	private int new_nrows;
	private int new_ncols;
	private int selected_row = 0;
	private int selected_col = 0;
	private boolean auto_order = true;
	private boolean verbose = false;

	private static final int IGNORE = 0;
	private static final int CHECK_SIZE = 1;
	private static final int GOBBLE = 2;
	private int dump_mode;		// How to handle incoming dumped data

	java.util.TreeSet all_start_times;

	private class cell {
		Atom[] command;		// Max data in the cell
		float duration;		// Time taken by command
		float start_time;	// In the context of this column, the time this command will start
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
	

	public tc_editor(Atom[] args)
	{
		declareInlets(new int[]{DataTypes.ALL, DataTypes.ALL});
		declareOutlets(new int[]{DataTypes.ALL});
		createInfoOutlet(false);

		setInletAssist(INLET_ASSIST);
		setOutletAssist(OUTLET_ASSIST);

		dump_mode = IGNORE;
		nrows = 10;
		ncols = 5;

		make_contents();
		
		/*		java.lang.Object o = ((java.util.ArrayList) contents.get(2)).get(2);
				post("Default array content: " + o); */

		all_start_times = new java.util.TreeSet();
		version();	
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

	 public void clear_contents() {
		for (int i = 0; i < ncols; ++i) {
			java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
			for (int j = 0; j < nrows; ++j) {
				col.set(j,null);
			}	
		}
	}

	public void print_contents() {
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

	private void store(int col, int row, Atom[] data) {
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
		verbose = (v != 0);
		post("verbose mode " + verbose);
	}


	public void autoorder(int a) {
		auto_order = (a != 0);
		post("automatic reordering mode " + auto_order);
	}


	public void bang()
	{
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
				outlet(0, "clear", new Atom[] { Atom.newAtom(selected_col),
									            Atom.newAtom(selected_row) });
			} else {
				outlet_set_cell(selected_col, i, c);
			}
		}

		// And now we can put the new value ("hold 0.") in the selected cell
		outlet(0, "set", new Atom[] { Atom.newAtom(selected_col),
									  Atom.newAtom(selected_row),
									  Atom.newAtom("hold"),
									  Atom.newAtom(0.)} );
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
		if (shouldBeClick.equals("click")) {
			// Do nothing
		} else {
			post("Didn't expect sync " + shouldBeClick);
		}
	}

	public void sync(String shouldBeSelect, int c, int r) {
		if (shouldBeSelect.equals("select")) {
			selected_row = r;
			selected_col = c;
			// post("you selected r " + r + ", c " + c);
		} else {
			post("Didn't expect sync " + shouldBeSelect);
		}
	}

	public void refresh_selection() {
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

		for (int i = 0; i<ncols; ++i) {
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
				
}


























