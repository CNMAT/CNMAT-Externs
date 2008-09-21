package cnmat.sdif;

/** Matrix header.  Provides basic information about an SDIF matrix. */
public class SDIFMatrixHeader extends SDIFHeader{
	private char[] matrixType = new char[4];
	private int matrixDataType;
	private int rowCount;
	private int columnCount;

	/** Default constructor */
	public SDIFMatrixHeader(){}

	/** Constructor
	    @param t 4 char type (e.g. 1TRC, 1RES, etc.)
	    @param dt Data type (use SDIFDataTypes to get the proper type)
	    @param r Row count.
	    @param c Column count.
	*/
	public SDIFMatrixHeader(String t, int dt, int r, int c) throws SDIFBadTypeException{
		this(t.toCharArray(), dt, r, c);
	}

	/** Constructor
	    @param t 4 char type (e.g. 1TRC, 1RES, etc.)
	    @param dt Data type (use SDIFDataTypes to get the proper type)
	    @param r Row count.
	    @param c Column count.
	*/
	public SDIFMatrixHeader(char[] t, int dt, int r, int c) throws SDIFBadTypeException{
		super();
		sdifTypeCheck(t);
		matrixType = t;
		matrixDataType = dt;
		rowCount = r;
		columnCount = c;
	}

	/** Set the matrix type as a 4-char code.
	    @param t Matrix type, a 4-char code such as 1RES or 1TRC.
	*/
	public void setMatrixType(char[] t) throws SDIFBadTypeException{
		sdifTypeCheck(t);
		matrixType = t;
	}

	/** Set the matrix type as a 4-char code.
	    @param t Matrix type, a 4-char code such as 1RES or 1TRC.
	*/
	public void setMatrixType(String t) throws SDIFBadTypeException{
		setMatrixType(t.toCharArray());
	}

	/** Set the matrix data type.  Use SDIFDataTypes to get a valid type.
	    @param t Data type.
	*/
	public void setMatrixDataType(int t){matrixDataType = t;}

	/** Set the row count for this matrix
@param r Row count.
	*/
	public void setRowCount(int r){rowCount = r;}

	/** Set the column count for this matrix.
@param c Column count.
	*/
	public void setColumnCount(int c){columnCount = c;}

	/** Get the 4-char code for the type of this matrix.
@returns 4-char type code.
	*/
	public char[] matrixType(){return matrixType;}

	/** Get the data type for this matrix
@returns Data type.
	*/
	public int matrixDataType(){return matrixDataType;}

	/** Get the row count for this matrix
@returns Row count
	*/
	public int rowCount(){return rowCount;}

	/** Get the column count for this matrix
@returns Column count
	*/
	public int columnCount(){return columnCount;}

	/** Returns a string representation of this header with the variable names and their values separated by an equals ("=") sign and terminated with newlines.
	    @returns A string representation of this header.
	*/
	public String toString(){
		String mt = new String(matrixType);
		return "matrixType = " + mt + " matrixDataType = " + matrixDataType + " rowCount = " + rowCount + " columnCount = " + columnCount;
	}
}