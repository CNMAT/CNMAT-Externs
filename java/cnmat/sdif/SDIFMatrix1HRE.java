package cnmat.sdif;
import com.cycling74.max.*;
/** Additional functionality for a 1HRE (harmonicity estimate) matrix.  See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a list and explanation of the standard SDIF types.
 */

public class SDIFMatrix1HRE extends SDIFMatrix{
	/** Mean delta Frequency column number */
	public final int MEAN_DELTA_FREQUENCY = 0;
	/** Harmonicity column number */
	public final int HARMONICITY = 1;
	/** Weighted harmonicity column number */
	public final int WEIGHTED_HARMONICITY = 2;

	/** Basic constructor */
	public SDIFMatrix1HRE(){}

	/** Constructor.
		@param h Header.
	*/
	public SDIFMatrix1HRE(SDIFMatrixHeader h){
		super(h);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1HRE(SDIFMatrixHeader h, Atom[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1HRE(SDIFMatrixHeader h, float[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1HRE(SDIFMatrixHeader h, int[] d){
		super(h, d);
	}
}