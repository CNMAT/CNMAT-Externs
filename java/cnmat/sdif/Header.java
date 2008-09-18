package cnmat.sdif;

public abstract class Header{
	public void sdifTypeCheck(char[] t) throws SDIFBadTypeException{
		if(t.length != 4)
			throw new SDIFBadTypeException("FrameHeader: Bad SDIF type.  Types must be a 4-char code");
	}
	public abstract String toString();
}