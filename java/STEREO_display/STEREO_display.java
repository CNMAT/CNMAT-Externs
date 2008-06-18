import com.cycling74.max.*;
import com.cycling74.msp.*;
import com.cycling74.jitter.*;
import gsfc.nssdc.cdf.*;
import gsfc.nssdc.cdf.util.*;
import java.util.Arrays;

public class STEREO_display extends MaxObject{
	JitterMatrix xdata, zdata;
	int width = 256;
	int mode = 0; // 0 = 3d sonogram-style, 1 = 2d cartesian plot.
	String buffer = "";
	int bufferSize = -1;
	double[] dateRange = null;
	double fillVal = 0.0;

	public STEREO_display(){
		init();
	}

	private void init(){
		declareInlets(new int[]{DataTypes.ALL, DataTypes.ALL});
		createInfoOutlet(false);
                declareOutlets(new int[]{DataTypes.ALL});
		declareAttribute("mode");
		declareAttribute("width");
		declareAttribute("buffer");
		declareAttribute("bufferSize");
		declareAttribute("fillVal");
	}

	public void jit_matrix(String s){
		int ilet = getInlet();
		switch(ilet){
		case 0:
			xdata = new JitterMatrix(s);
			break;
		case 1:
			zdata = new JitterMatrix(s);
			break;
		}
	}

	public void bang(){
		switch(mode){
		case 0:
			String dt = zdata.getType();
			JitterMatrix jm = new JitterMatrix(1, dt, width, zdata.getDim()[1]);
			double[] ar = processData_1(width);
			jm.copyArrayToMatrix(ar);
			outlet(0, "jit_matrix", jm.getName());
			if(buffer.compareTo("") == 0 || bufferSize < 1) break;
			fillBuffer();
			break;
		case 1:
			processData_2();
			if(buffer.compareTo("") == 0 || bufferSize < 1) break;
			fillBuffer();
			break;
		}
	}

	private void fillBuffer(){
		//java.io.File f = new java.io.File("/Users/johnmac/Workspace/STEREO/STEREO_max_new/" + buffer + ".txt");
		//java.io.FileOutputStream fos = null;
		//try{fos = new java.io.FileOutputStream(f);}
		//catch(Exception e){e.printStackTrace();}
		double[] ar = processData_1(bufferSize);
		MSPBuffer.setFrames(buffer, zdata.getDim()[1], bufferSize);
		float[] fill = new float[bufferSize];
		Arrays.fill(fill, (float)fillVal);
		MSPBuffer.poke(buffer, fill);
		for(int j = 0; j < zdata.getDim()[1]; j++){
			for(int i = 0; i < bufferSize; i++){
				MSPBuffer.poke(buffer, j + 1, i, (float)ar[i + (j * bufferSize)]);
				//String s = (j + 1) + " " + i + " " + ((float)ar[i + (j * bufferSize)]) + "\n";
				//try{fos.write(s.getBytes());}
				//catch(Exception e){e.printStackTrace();}
			}
		}
		//try{fos.close();}
		//catch(Exception e){e.printStackTrace();}
	}

	private double[] processData_1(int w){
		double[] xArray = makeXArray(w);
		int[] zdim = zdata.getDim();
		double[] out = new double[w * zdim[1]];
		Arrays.fill(out, fillVal);
		//post("xArray.length = " + xArray.length + " w = " + w);

		for(int j = 0; j < zdim[1]; j++){
			for(int i = 0; i < xArray.length; i++){
				double currentZ = zdata.getcellFloat(new int[]{i, j})[0];
				int outIndex = (j * w) + (int)Math.round(xArray[i]);
				if(outIndex >= 0 && outIndex < out.length){
					double currentOutVal = out[outIndex];
					if(currentOutVal < currentZ) out[outIndex] = currentZ;
				}
			}
		}
		return out;
	}

	private void processData_2(){
		double[] xArray = makeXArray(width);
		JitterMatrix jm = new JitterMatrix(2, "float32", xdata.getDim()[0], 1);
		float[] zArray = new float[zdata.getDim()[0]];
		zdata.copyMatrixToArray(zArray);

		for(int i = 0; i < zArray.length; i++){
			jm.setcell(new int[]{i, 0}, new float[]{(float)xArray[i], zArray[i]});
			//post(xArray[i] + " " + zArray[i]);
		}

		// these didn't work--either buggy or i'm stupid...
		//jm.copyArrayToVectorPlanar(0, 1, new int[]{0, 0}, xArray, xArray.length, 0);
		//jm.copyArrayToVectorPlanar(1, 1, new int[]{0, 0}, zArray, zArray.length, 0);
		outlet(0, "jit_matrix", jm.getName());
	}

	private double[] makeXArray(int w){
		// xdata is assumed to be sorted from low to high.
		double[] xdataArray = new double[xdata.getDim()[0]];
		for(int i = 0; i < xdata.getDim()[0]; i++){
			int[] e = xdata.getcellInt(new int[]{i, 0});
			try{xdataArray[i] = Epoch.compute(e[0], e[1], e[2], e[3], e[4], e[5], e[6]);}
			catch(Exception ex){
				error("plot_3d: exception thrown while trying to compute epochs");
				ex.printStackTrace();
				return null;
			}
		}
		double xmin, xmax, xdiff, xstep;
		int[] zdim = zdata.getDim();

		if(dateRange == null){
			xmin = xdataArray[0];
			xmax = xdataArray[xdataArray.length - 1];
		}else{
			xmin = dateRange[0];
			xmax = dateRange[1];
		}

		xdiff = xmax - xmin;
		xstep = ((double)w- 1.0) / xdiff;

		double[] out = new double[xdataArray.length];
		for(int i = 0; i < zdim[0]; i++){
			double currentX = xdataArray[i];
			out[i] = (int)Math.round((currentX - xmin) * xstep);
			//post(buffer + " currentX = " + currentX + " out[" + i + "] = " + out[i]);
		}
		return out;
	}

	public void dateRange(long[] r){
		if(dateRange == null) dateRange = new double[2];
		try{dateRange[0] = Epoch.compute(r[0], r[1], r[2], r[3], r[4], r[5], r[6]);}
		catch(Exception e){e.printStackTrace();}
		try{dateRange[1] = Epoch.compute(r[7], r[8], r[9], r[10], r[11], r[12], r[13]);}
		catch(Exception e){e.printStackTrace();}
	}

	public void dateRange_fullDay(long d){
		//this is horrible...
		String ds = String.valueOf(d);
		Long year = new Long(new String(new char[]{ds.charAt(0), ds.charAt(1), ds.charAt(2), ds.charAt(3)}));
		Long month = new Long(new String(new char[]{ds.charAt(4), ds.charAt(5)}));
		Long day = new Long(new String(new char[]{ds.charAt(6), ds.charAt(7)}));
		if(dateRange == null) dateRange = new double[2];
		try{dateRange[0] = Epoch.compute(year.longValue(), month.longValue(), day.longValue(), 0, 0, 0, 0);}
		catch(Exception e){e.printStackTrace();}
		try{dateRange[1] = Epoch.compute(year.longValue(), month.longValue(), day.longValue(), 23, 59, 59, 999);}
		catch(Exception e){e.printStackTrace();}
	}

	public void dateRange(double[] r){
		// don't know why i do this--it's not like max has doubles...
		dateRange = r;
	}

	public void tellmeeverything(){
		post("width = " + width);
		post("mode = " + mode);
		post("buffer = " + buffer);
		post("bufferSize = " + bufferSize);
	}
}