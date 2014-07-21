package cnmat.sdif;
/** Frame header.  Provides general information about an SDIF frame.  Standard frame types include

See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a list and explanation of the standard SDIF types.
 */
public class SDIFFrameHeader extends SDIFHeader{
	private char[] frameType = new char[4];
	private int size;
	private float time;
	private int streamID;
	private int matrixCount;

	/** Default constructor */
	public SDIFFrameHeader(){}
	
	/** Constructor
	    @param ft Frame type.  See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a list of standard frame types and their descriptions.
	    @param s The size of the Frame Header not including the frame type and size
	    @param t The time of the frame.
	    @param sid Stream ID.
	    @param mc The number of Matrices contained in the frame.
	*/
	public SDIFFrameHeader(char[] ft, int s, float t, int sid, int mc) throws SDIFBadTypeException{
		super();
		sdifTypeCheck(ft);
		frameType = ft;
		size = s;
		time = t;
		streamID = sid;
		matrixCount = mc;
	}

	/** Constructor
	    @param ft 4-char code for the frame type.  See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a list of standard frame types and their descriptions.
	    @param s The size of the Frame Header not including the frame type and size
	    @param t The time of the frame.
	    @param sid Stream ID.
	    @param mc The number of Matrices contained in the frame.
	*/
	public SDIFFrameHeader(String ft, int s, float t, int sid, int mc) throws SDIFBadTypeException{
		this(ft.toCharArray(), s, t, sid, mc);
	}

	/** Set the frame type of the SDIF frame.  If the frame type is not exactly 4 characters long, an SDIFBadTypeException is thrown.
	    @param t 4-char type code.
	    @throws SDIFBadTypeException
	*/
	public void setFrameType(String t) throws SDIFBadTypeException{
		setFrameType(t.toCharArray());
	}

	/** Set the frame type of the SDIF frame.  If the frame type is not exactly 4 characters long, an SDIFBadTypeException is thrown.
	    @param t 4-char type code.
	    @throws SDIFBadTypeException
	*/
	public void setFrameType(char[] t) throws SDIFBadTypeException{
		sdifTypeCheck(t);
		frameType = t;
	}

	/** Sets the size of the frame header not including the frame type and size
	    @param s Size.
	*/
	public void setSize(int s){size = s;}

	/** Sets the time of the frame
	    @param t Time.
	*/
	public void setTime(float t){time = t;}

	/** Sets the stream ID of the frame
	    @param sid Stream ID.
	*/
	public void setStreamID(int sid){streamID = sid;}

	/** Sets the number of matrices in this frame.
	    @param c Matrix count.
	*/
	public void setMatrixCount(int c){matrixCount = c;}

	/** Get the frame type.
	    See <a href="http://sdif.sourceforge.net/standard/types-main.html">http://sdif.sourceforge.net/standard/types-main.html</a> for a complete list of valid frame types.
	    @returns The frame type as a 4-char code (e.g. 1TRC, 1RES, etc.)
	*/
	public char[] frameType(){return frameType;}

	/** Get the size of the frame header not including the frame type and size
	    @returns The size of the header.
	*/
	public int size(){return size;}

	/** Get the time of this frame
	    @returns The time of this frame.
	*/
	public float time(){return time;}

	/** Get the stream ID
	    @returns the stream ID of this frame.
	*/
	public int streamID(){return streamID;}

	/** Get the matrix count for this frame.
	    @returns The number of matrices in this frame.
	*/
	public int matrixCount(){return matrixCount;}

	/** Returns a string representation of this header with the variable names and their values separated by an equals ("=") sign and terminated with newlines.
	    @returns A string representation of this header.
	*/
	public String toString(){
		return "frameType = " + frameType + "\nsize = " + size + "\ntime = " + time + "\nstreamID = " + streamID + "\nmatrixCount = " + matrixCount;
	}
}