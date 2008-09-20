package cnmat.sdif;

public abstract class Header{
	public void sdifTypeCheck(char[] t) throws SDIFBadTypeException{
		if(t.length != 5) // should be 4, but the jstring we're passing from the native library is likely null-terminated.)
			throw new SDIFBadTypeException("FrameHeader: Bad SDIF type.  Type " + t + " " + (new String(t)) + " has " + t.length + " characters and is not a valid 4-char code");
	}
	public abstract String toString();
}