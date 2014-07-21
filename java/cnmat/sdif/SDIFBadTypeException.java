package cnmat.sdif;

/** Thrown when a bad type code is used.  Mainly this will occur if the type code is too long. */
public class SDIFBadTypeException extends java.lang.Throwable{

	/** Constructor. */
	public SDIFBadTypeException(){
		super();
	}

	/** Constructor with message about exception.
		@param message A message describing the reason the exception was thrown.
	*/
	public SDIFBadTypeException(String message){
		super(message);
	}
}