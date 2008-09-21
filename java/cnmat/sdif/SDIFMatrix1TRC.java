package cnmat.sdif;
import com.cycling74.max.*;
/** Additional functionality for a 1TRC (sinusoidal tracks) matrix.  See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a list and explanation of the standard SDIF types.
 */

public class SDIFMatrix1TRC extends SDIFMatrix{
	/** Index column number */
	public final int INDEX = 0;
	/** Frequency column number */
	public final int FREQUENCY = 1;
	/** Amplitude column number */
	public final int AMPLITUDE = 2;
	/** Phase column number */
	public final int PHASE = 3;

	/** Basic constructor */
	public SDIFMatrix1TRC(){}

	/** Constructor.
		@param h Header.
	*/
	public SDIFMatrix1TRC(SDIFMatrixHeader h){
		super(h);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1TRC(SDIFMatrixHeader h, Atom[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1TRC(SDIFMatrixHeader h, float[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1TRC(SDIFMatrixHeader h, int[] d){
		super(h, d);
	}
}