package cnmat.sdif;
import com.cycling74.max.*;

public class SDIF_buffer extends MaxObject{
	private native MatrixHeader n_getMatrixHeader(String name);
	private native int n_init();

	public SDIF_buffer(){
		// load native lib
		java.util.Properties p = System.getProperties();
		String dylibPath = null;
		//if(p.getProperty("os.name").compareTo("Mac OS X") == 0)
			dylibPath = MaxSystem.locateFile("libSDIFJava_test.dylib");
			//else dylibPath = MaxSystem.locateFile("libjavaobject.1.0.dll");
		try{System.load(dylibPath);}
		catch(Exception e){
			error("SDIFJava_test: couldn't load libSDIFJava_test--make sure it's in your searchpath.");
			return;
		}

		int r = n_init();
		if(r == 0){
			error("SDIFJava_test: there was a problem initializing the native library");
			return;
		}
	}

	public void getMatrixHeader(String name){
		MatrixHeader mh = (MatrixHeader)n_getMatrixHeader(name);
		post(mh.toString());
	}
}