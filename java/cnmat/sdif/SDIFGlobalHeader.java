package cnmat.sdif;

/**
 * Global Header for an SDIF file.
 */
public class SDIFGlobalHeader extends SDIFHeader{
	/** SDIF global identifier.  This should always be 'S', 'D', 'I', 'F' */
	private char[] SDIF = new char[4];
	/** The size of the header frame not including SDIF and size */
	private int size;
	/** Version of the SDIF library used to encode the file */
	private int SDIFVersion;
	/** Version of the SDIF spec */
	private int SDIFStandardTypesVersion;

	/** Default constructor */
	public SDIFGlobalHeader(){}

	/** Constructor
	    @param sdifc SDIF identifier.  Should always be 'S', 'D', 'I', 'F'
	    @param s size Size of the header not including SDIF and size
	    @param v Version of the SDIF library used
	    @param sstv Version of the SDIF spec
	*/
	public SDIFGlobalHeader(char[] sdifc, int s, int v, int sstv){
		super();
		SDIF = sdifc;
		size = s;
		SDIFVersion = v;
		SDIFStandardTypesVersion = sstv;
	}

	/** Constructor
	    @param sdifc SDIF identifier.  Should always be 'S', 'D', 'I', 'F'
	    @param s size Size of the header not including SDIF and size
	    @param v Version of the SDIF library used
	    @param sstv Version of the SDIF spec
	*/
	public SDIFGlobalHeader(String sdifc, int s, int v, int sstv){
		this(sdifc.toCharArray(), s, v, sstv);
	}

	/** Sets the SDIF field of the header to 'S', 'D', 'I', 'F' */
	public void setSDIF(){SDIF = new char[]{'S', 'D', 'I', 'F'};}

	/** Sets the size of the header.  This should be the size not including SDIF and size.
	    @param s Size
	*/
	public void setSize(int s){size = s;}

	/** Sets the version of the library used to create the SDIF file
	    @param v Version of the library
	*/
	public void setSDIFVersion(int v){SDIFVersion = v;}

	/** Sets the version of the spec used to create the file
	    @param sstv Version of the spec
	*/
	public void setSDIFStandardTypesVersion(int sstv){SDIFStandardTypesVersion = sstv;}

	/** Get the SDIF field 
	    @returns The SDIF field.
	*/
	public char[] SDIF(){return SDIF;}

	/** Get the size of the header not including SDIF and size 
	    @returns The size of the header not including SDIF and size.
	*/
	public int size(){return size;}

	/** Get the version of the library used to create the file 
	    @returns The version of the library.
	*/
	public int SDIFVersion(){return SDIFVersion;}

	/** Get the version of the spec used to create the file 
	    @returns The version of the spec.
	*/
	public int SDIFStandardTypesVersion(){return SDIFStandardTypesVersion;}

	/** Returns a string representation of this header.  The string contains a list of the variable names and their values separated by an equals sign ("=") and terminated by a newline.
	    @returns A string representation of this header.
	*/
	public String toString(){
		return "SDIF = " + SDIF + "\nsize = " + size + "\nSDIFVersion = " + SDIFVersion + "\nSDIFStandardTypesVersion = " + SDIFStandardTypesVersion;
	}
}