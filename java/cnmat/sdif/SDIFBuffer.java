package cnmat.sdif;

public class SDIFBuffer{
	private native SDIFMatrixHeader n_getMatrixHeader(String name);
	private native int n_init();

	public SDIFBuffer(){
		// load native lib
		java.util.Properties p = System.getProperties();
		String dylibPath = null;
		//if(p.getProperty("os.name").compareTo("Mac OS X") == 0)
		//dylibPath = MaxSystem.locateFile("libSDIF_buffer_native.dylib");
			//else dylibPath = MaxSystem.locateFile("libjavaobject.1.0.dll");
		try{System.load(dylibPath);}
		catch(Exception e){
			//error("mxj SDIF_buffer: couldn't load libSDIF_buffer_native.dylib--make sure it's in your searchpath.");
			return;
		}

		int r = n_init();
		if(r == 0){
			//error("mxj SDIF_buffer: there was a problem initializing the native library");
			return;
		}
	}

	public void getMatrixHeader(String name){
		SDIFMatrixHeader mh = (SDIFMatrixHeader)n_getMatrixHeader(name);
		//post(mh.toString());
	}
}