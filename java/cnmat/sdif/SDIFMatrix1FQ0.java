package cnmat.sdif;
import com.cycling74.max.*;
/** Additional functionality for a 1FPQ (fundamental estimate) matrix.  See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a list and explanation of the standard SDIF types.
 */

public class SDIFMatrix1FQ0 extends SDIFMatrix{
	/** Frequency column number */
	public final int FREQUENCY = 0;
	/** Confidence column number */
	public final int CONFIDENCE = 1;
	/** Score column number */
	public final int SCORE = 2;
	/** Real amplitude column number */
	public final int REAL_AMPLITUDE = 3;

	/** Basic constructor */
	public SDIFMatrix1FQ0(){}

	/** Constructor.
		@param h Header.
	*/
	public SDIFMatrix1FQ0(SDIFMatrixHeader h){
		super(h);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1FQ0(SDIFMatrixHeader h, Atom[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1FQ0(SDIFMatrixHeader h, float[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1FQ0(SDIFMatrixHeader h, int[] d){
		super(h, d);
	}
}