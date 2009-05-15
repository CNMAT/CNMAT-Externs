package cnmat.sdif;

/** Codes used for SDIF types. */
public class SDIFTypes{
	/** bytecode for float used by native SDIF library */
	public int SDIF_FLOAT32 = 0x0004;
	/** bytecode for double used by native SDIF library */
	public int SDIF_FLOAT64 = 0x0008;
	/** bytecode for short used by native SDIF library */
	public int SDIF_INT16 = 0x0102;
	/** bytecode for int used by native SDIF library */
	public int SDIF_INT32 = 0x0104;
	/** bytecode for long used by native SDIF library */
	public int SDIF_INT64 = 0x0108;
	/** bytecode for unsigned int used by native SDIF library */
	public int SDIF_UINT32 = 0x0204;
	/** bytecode for char (16 bit) used by native SDIF library */
	public int SDIF_UTF8 = 0x0301;
	/** bytecode for char used by native SDIF library */
	public int SDIF_BYTE = 0x0401;
	/** bytecode used when no byte is specified  */
    	public int SDIF_NO_TYPE = -1;

	private SDIFTypes(){}
}