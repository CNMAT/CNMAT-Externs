import com.cycling74.max.*;

public class tc_editor extends MaxObject
{

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

	private static final int IGNORE = 0;
	private static final int CHECK_SIZE = 1;
	private static final int GOBBLE = 2;
	private int dump_mode;		// How to handle incoming dumped data

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
						duration = args[2].getFloat();
					}
				} else {
					post("Ack!  Too many elements in list");
				}
			} else if (args[0].getString.equalsIgnoreCase("jump")) {
				duration = 0;
			} else if (args[0].getString.equalsIgnoreCase("hold")) {
				if (args.length == 1) {
					post("Ack! Need an argument to HOLD");
				} else if (args[1].isString()) {
					post("Ack! Argument to HOLD must be a number");
				} else {
					duration = args[1].getFloat();
				}
			} else if (args[0].getString.equalsIgnoreCase("ramp")) {
				if (args.length < 4) {
					// Implied jump
					duration = 0;
				} else if (args.length == 4) {
					if (args[3].isString()) {
						post("Ack! JUMP's third argument must be a number");
					} else {
						duration = args[3].getFloat();
					}
				} else {
					post("Ack!  Too many arguments to JUMP!");
				}
			} else {
				post("Ack! Unrecognized tempocurver command: " + args[0].getString());
			}
		}
	}
	

	public tc_editor(Atom[] args)
	{
		declareInlets(new int[]{DataTypes.ALL, DataTypes.ALL});
		declareOutlets(new int[]{DataTypes.ALL});
		createInfoInlet(false);

		setInletAssist(INLET_ASSIST);
		setOutletAssist(OUTLET_ASSIST);

		dump_mode = IGNORE;
		nrows = 10;
		ncols = 5;

		make_contents();
		java.lang.Object o = ((java.util.ArrayList) contents.get(2)).get(2);
		post("Default array content: " + o);

	}
    
	private void make_contents() {
		// Make an ArrayList of ArrayLists to mirror what's in the jit.cellblock
		// Put a "null" in each element.
		contents = new java.util.ArrayList(ncols);
		for (int i = 0; i < ncols; ++i) {
			contents.add(make_column());
		}
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
		for (int i = ncols; i < larger_ncols; ++i) {
			contents.add(make_column());
		}
		ncols = larger_ncols;
	}

	private void add_rows(int larger_nrows) {
		for (int i = 0; i<ncols; ++i) {
			java.util.ArrayList col = (java.util.ArrayList) contents.get(i);
			col.ensureCapacity(larger_nrows);
			for (int j = nrows; j < larger_nrows; ++j) {
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
			o =  new cell(data)
		}

	    ((java.util.ArrayList) contents.get(col)).set(row, o);
	}



	public void bang()
	{
		post("bung!");
	}

	public void dumpout(int col, int row, Atom[] data) {
		switch (dump_mode) {
			case IGNORE: // do nothing
				break;
			case CHECK_SIZE:
				/* Check nrows first, cause it's more efficient to call add_rows() before
				   add_cols() than the reverse. */
				if (row > nrows) {
					add_rows(row);
				}
				if (col > ncols) {
					add_cols(col);
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

	
	
	}	

	public void insert() {
		take_dump();
	}
		


	public void sync() {
		post("Don't know how to sync that");
	}

	public void sync(String shouldBeSelect, int r, int c) {
		if (shouldBeSelect.equals("select")) {
			post("Sync r " + r + ", c " + c);
		} else if (shouldBeSelect.equals("click")) {
			// Do nothing
		} else {
			post("Didn't expect sync " + shouldBeSelect);
		}
	}
    
	public void inlet(int i)
	{
	}
    
	public void inlet(float f)
	{
	}
    
    
	public void list(Atom[] list)
	{
	}
    
}










