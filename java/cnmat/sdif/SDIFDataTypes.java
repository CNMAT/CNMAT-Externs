package cnmat.sdif;

public class SDIFDataTypes{
	public int SDIF_FLOAT32 = 0x0004;
	public int SDIF_FLOAT64 = 0x0008;
	public int SDIF_INT16 = 0x0102;
	public int SDIF_INT32 = 0x0104;
	public int SDIF_INT64 = 0x0108;
	public int SDIF_UINT32 = 0x0204;
	public int SDIF_UTF8 = 0x0301;
	public int SDIF_BYTE = 0x0401;
    	public int SDIF_NO_TYPE = -1;

	private SDIFDataTypes(){}
}