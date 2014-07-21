package cnmat.sdif;
import com.cycling74.max.*;
/** Additional functionality for a 1RES (resonance model) matrix.  See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a list and explanation of the standard SDIF types.
 */

public class SDIFMatrix1RES extends SDIFMatrix{
	/** Frequency column number */
	public final int FREQUENCY = 0;
	/** Amplitude column number */
	public final int AMPLITUDE = 1;
	/** Decayrate column number */
	public final int DECAYRATE = 2;
	/** Phase column number */
	public final int PHASE = 3;

	/** Basic constructor */
	public SDIFMatrix1RES(){}

	/** Constructor.
		@param h Header.
	*/
	public SDIFMatrix1RES(SDIFMatrixHeader h){
		super(h);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1RES(SDIFMatrixHeader h, Atom[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1RES(SDIFMatrixHeader h, float[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1RES(SDIFMatrixHeader h, int[] d){
		super(h, d);
	}
}