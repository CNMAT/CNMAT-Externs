package cnmat.sdif;
import com.cycling74.max.MaxSystem;

public class SDIFBuffer{
	private static native SDIFMatrix n_getMatrix(String name, char[] type, double time, int direction);
	private static native SDIFMatrixHeader n_getMatrixHeader(String name);
	private static native int n_init();

	public SDIFBuffer(){}

	public static void init(){
		String dylib = MaxSystem.locateFile("libSDIFBuffer_native.dylib");
		System.load(dylib);
		int r = n_init();
                if(r == 0){
                        System.out.println("cnmat.sdif.SDIF: couldn't load native library");
                        return;
                }
	}

	public static SDIFMatrix getMatrix(String name, char[] type, double time, int direction){
		return n_getMatrix(name, type, time, direction);
	}

	public static SDIFMatrixHeader getMatrixHeader(String name){
		return n_getMatrixHeader(name);
	}
}