package cnmat.sdif;
import com.cycling74.max.*;
/** Additional functionality for a 1PIC (picked spectral peaks) matrix.  See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a list and explanation of the standard SDIF types.
 */

public class SDIFMatrix1PIC extends SDIFMatrix{
	/** Frequency column number */
	public final int FREQUENCY = 0;
	/** Amplitude column number */
	public final int AMPLITUDE = 1;
	/** Phase column number */
	public final int PHASE = 2;
	/** Confidence column number */
	public final int CONFIDENCE = 3;

	/** Basic constructor */
	public SDIFMatrix1PIC(){}

	/** Constructor.
		@param h Header.
	*/
	public SDIFMatrix1PIC(SDIFMatrixHeader h){
		super(h);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1PIC(SDIFMatrixHeader h, Atom[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1PIC(SDIFMatrixHeader h, float[] d){
		super(h, d);
	}

	/** Constructor.
		@param h Header.
		@param d Interleaved data.
	*/
	public SDIFMatrix1PIC(SDIFMatrixHeader h, int[] d){
		super(h, d);
	}
}