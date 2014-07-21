import com.cycling74.max.*;
import com.cycling74.jitter.*;
import gsfc.nssdc.cdf.*;
import gsfc.nssdc.cdf.util.*;

public class STEREO_display extends MaxObject{
	JitterMatrix xdata, zdata;
	int width = 256;

	public STEREO_display(){
		init();
	}

	public STEREO_display(int w){
		init();
		width = w;
	}

	private void init(){
		declareInlets(new int[]{DataTypes.ALL, DataTypes.ALL});
		createInfoOutlet(false);
                declareOutlets(new int[]{DataTypes.ALL});
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
		processData();
	}

	private double makeXArray(){
		// xdata is assumed to be sorted from low to high.
		double[] xdataArray = new double[xdata.getDim()[0]];
		for(int i = 0; i < xdata.getDim()[0]; i++){
			int[] e = xdata.getcellInt(new int[]{i, 0});
			try{xdataArray[i] = Epoch.compute(e[0], e[1], e[2], e[3], e[4], e[5], e[6]);}
			catch(Exception ex){
				error("plot_3d: exception thrown while trying to compute epochs");
				ex.printStackTrace();
				return;
			}
		}
		double xmin, xmax, xdiff, xstep;
		int[] zdim = zdata.getDim();
		JitterMatrix jm = new JitterMatrix(1, zdata.getType(), width, zdim[1]);
		jm.setall(Atom.newAtom(0.0));

		xmin = xdataArray[0];
		xmax = xdataArray[xdataArray.length - 1];
		xdiff = xmax - xmin;
		xstep = ((double)width - 1.0) / xdiff;


		for(int i = 0; i < zdim[0]; i++){
			double currentX = xdataArray[i];
			//post("xscaled = " + ((currentX - xmin) * xstep));
			int xscaled = (int)Math.round((currentX - xmin) * xstep);
		
			for(int j = 0; j < zdim[1]; j++){
				double currentZ = zdata.getcellFloat(new int[]{i, j})[0];
				double currentOutVal = jm.getcellFloat(new int[]{xscaled, j})[0];
				post("currentZ = " + currentZ + " currentOutVal = " + currentOutVal);
				if(currentOutVal < currentZ) jm.setcell(new int[]{xscaled, j}, 0, currentZ);
			}
		}
		outlet(0, "jit_matrix", jm.getName());
	}
}