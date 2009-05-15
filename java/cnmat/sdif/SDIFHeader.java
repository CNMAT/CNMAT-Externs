package cnmat.sdif;

/** Abstract parent class of the SDIF*Header classes.  */
public abstract class SDIFHeader{
	/** Verify that the type being used is valid.  At this time, the only thing it checks is that the code isn't too long, 
		but in the future, it could do a more rigorous test.
		@param t Type code to be verified
		@throws SDIFBadTypeException
	*/
	public void sdifTypeCheck(char[] t) throws SDIFBadTypeException{
		if(t.length != 5) // should be 4, but the jstring we're passing from the native library is likely null-terminated.)
			throw new SDIFBadTypeException("FrameHeader: Bad SDIF type.  Type " + t + " " + (new String(t)) + " has " + t.length + " characters and is not a valid 4-char code");
	}

	/** Return a string representation of this header.  All subclasses must implement this.
		@returns String representation of the header.
	*/
	public abstract String toString();
}