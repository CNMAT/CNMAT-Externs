package cnmat.sdif;
import com.cycling74.max.*;

/** Basic SDIF matrix providing storage for an SDIFMatrixHeader and the matrix data stored as an array of Atoms.  
This class can be subclassed to provide functionality for specific matrix types (e.g. see SDIFMatrix1TRC and SDIFMatrixtRES).
*/

public class SDIFMatrix{
	/** The header for this matrix.*/
	protected SDIFMatrixHeader header = null;

	/** The data contained in an interleaved single-dimensional row-major array of atoms. */
	protected Atom[] data = null;

	/** Basic constructor */
	public SDIFMatrix(){}

	/** Constructor.
		@param h Header.
	*/
	public SDIFMatrix(SDIFMatrixHeader h){
		header = h;
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix(SDIFMatrixHeader h, Atom[] d){
		header = h;
		data = d;
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix(SDIFMatrixHeader h, float[] d){
		header = h;
		data = Atom.newAtom(d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix(SDIFMatrixHeader h, int[] d){
		header = h;
		data = Atom.newAtom(d);
	}

	/** Set the internal data--must be interleaved.
		@param d Interleaved data.
	*/
	public void setData(Atom[] d){data = d;}

	/** Set the internal data--must be interleaved.  The data will be converted and stored internally in an array of Atoms.
		@param d Interleaved data.
	*/
	public void setData(float[] d){data = Atom.newAtom(d);}

	/** Set the internal data--must be interleaved.  The data will be converted and stored internally in an array of Atoms.
		@param d Interleaved data.
	*/
	public void setData(int[] d){data = Atom.newAtom(d);}

	/** Get the interleaved data in one big array
		@returns The data.
	*/
	public Atom[] data(){return data;}

	/** Set the matrix header
		@param h Matrix header.
	*/
	public void setHeader(SDIFMatrixHeader h){header = h;}

	/** Get the matrix header
		@returns The matrix header.
	*/
	public SDIFMatrixHeader header(){return header;}

	/** De-interleave the data and return it in a 2-dimensional array.
		@returns A 2-dimensional array representing the columns.  col[2][50] = column 2, element 50.
	*/
	public Atom[][] getColumns(){
		int[] cols = new int[header.columnCount()];
		for(int i = 0; i < cols.length; i++)
			cols[i] = i;
		return getColumns(cols);
	}

	/** Get an array of columns from the matrix.  
		Columns can be specified in any order and will be returned in the same order as specified. 
		@param c Columns to get.
		@returns A 2-dimensional array representing the columns.  col[2][50] = column 2, element 50.
	*/
	public Atom[][] getColumns(int[] c){
		int num_columns = header.columnCount();
		int col_len = data.length / num_columns;
		Atom[][] out = new Atom[c.length][col_len];
		for(int i = 0; i < c.length; i++){
			out[i] = getColumn(c[i]);
		}
		return out;
	}

	/** Get a single column from the matrix.
		@param c Column to get.
		@returns The requested column.
	*/
	public Atom[] getColumn(int c){
		int num_columns = header.columnCount();
		int col_len = data.length / num_columns;
		Atom[] out = new Atom[col_len];
		for(int i = 0; i < data.length; i++){
			out[i] = data[i * num_columns];
		}
		return out;
	}
}