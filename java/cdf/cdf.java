import com.cycling74.max.*;
import java.io.*;
import java.text.*;
import java.util.*;
import java.lang.reflect.*;
import gsfc.nssdc.cdf.*;
import gsfc.nssdc.cdf.util.*;

public class cdf extends MaxObject implements CDFConstants{
	CDF cdf = null;
	int OUT_BANG = 1;
	int OUT_OSC = 2;
	int OUT_EPOCH = 3;
	
	public cdf(){
		createInfoOutlet(false);
		declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL, DataTypes.ALL, DataTypes.ALL});
	}

	public void read(){
		String filepath;
		filepath = MaxSystem.openDialog();
		if(filepath == null) return;
		read(filepath);
	}

	public void read(String filepath){
		if(cdf != null)
			try{cdf.close();}
			catch(Exception e){
				error("cdf: Exception thrown trying to close " + cdf.getName() + " file.\n");
				e.printStackTrace();
			}
		try{
			cdf = CDF.open(filepath, READONLYoff);
		}catch(Exception e){
			error("cdf: Exception thrown trying to open cdf file.\n");
			e.printStackTrace();
		}
		//getGlobalAttributes();
		getVarNames();
	}

	/*
	public void getGlobalAttributes(){
		Vector<Attribute> atts = cdf.getAttributes();
		for(int i = 0; i < atts.size(); i++){
			Attribute a = atts.elementAt(i);
			post(a.getName());
			Vector<Entry> ents = a.getEntries();
			for(int j = 0; j < a.getNumEntries(); j++){
				Entry e = ents.elementAt(j);
				if(e != null)
					post("data type = " + e.getDataType());
			}
		}
	}
	*/

	public void getVarNames(){
		Vector<Variable> v = cdf.getVariables();
		outlet(OUT_OSC, new Atom[]{Atom.newAtom("/varname"), Atom.newAtom("clear")});
		for(int i = 0; i < v.size(); i++){
			outlet(OUT_OSC, new Atom[]{Atom.newAtom("/varname"), Atom.newAtom(v.elementAt(i).getName())});
		}
		outlet(OUT_OSC, new Atom[]{Atom.newAtom("/varname"), Atom.newAtom("done")});
	}

	public void getAllRecordsForVarName(String[] vars){
		for(int i = 0; i < vars.length; i++){
			if(do_getAllRecordsForVarName(vars[i])){
				error("cdf: there was an error getting data for " + vars[i]);
				return;
			}
		}
		outlet(OUT_BANG, "bang");
	}

	public void getAllRecordsForVarName(String var){
		if(do_getAllRecordsForVarName(var)){
			error("cdf: there was an error getting data for " + var);
		}else outlet(OUT_BANG, "bang");
	}

	private boolean do_getAllRecordsForVarName(String var){
		long varID = cdf.getVariableID(var);
		if(varID < 0){
			error("cdf: variable " + var + " doesn't exist.\n");
			return true;
		}
		//post("" + varID);
		Variable v = null;
		long n = 0L;
		CDFData d = null;
		try{
			v = cdf.getVariable(var);
			n = v.getNumWrittenRecords();
			d = v.getRecordsObject(0L, n);
		}
		catch(Exception e){
			error("cdf: error getting data for " + var);
			e.printStackTrace();
			return true;
		}

		// the cdf constants are long, not int, so use this crappy if statement 
		// rather than cast the constants to int to use switch.
		if(v.getDataType() == CDF_UINT4){
			if(d.getDimCounts()[0] > 0)
				outputData(var, (long[][])d.getData());
			else outputData(var, (long[])d.getData());

		} else if(v.getDataType() == CDF_DOUBLE){
			if(d.getDimCounts()[0] > 0)
				outputData(var, (double[][])d.getData());
			else outputData(var, (double[])d.getData());

		} else if(v.getDataType() == CDF_REAL4 || v.getDataType() == CDF_FLOAT){
			if(d.getDimCounts()[0] > 0)
				outputData(var, (float[][])d.getData());
			else outputData(var, (float[])d.getData());

		} else if(v.getDataType() == CDF_EPOCH){
			outputEpochData(var, (double[])d.getData());

		} else {
			error("cdf: data is type " + v.getDataType());
		}

		return false;
	}

	private void outputMinMax(String var, Atom min, Atom max){
		outlet(OUT_OSC, new Atom[]{Atom.newAtom("/" + var + "/min"), min});
	        outlet(OUT_OSC, new Atom[]{Atom.newAtom("/" + var + "/max"), max});
	}

	private void outputData(String var, long[] data){
		long min = data[0], max = data[0];
		for(int i = 0; i < data.length; i++){
			if(data[i] < min) min = data[i];
			if(data[i] > max) max = data[i];
			outlet(0, new Atom[]{Atom.newAtom(var), Atom.newAtom(data[i])});
		}
		outputMinMax(var, Atom.newAtom(min), Atom.newAtom(max));
	}

	private void outputData(String var, long[][] data){
		long min = data[0][0], max = data[0][0];
		for(int i = 0; i < data.length; i++){
			for(int j = 0; j < data[i].length; j++){
				if(data[i][j] < min) min = data[i][j];
				if(data[i][j] > max) max = data[i][j];
			}
			outlet(0, Atom.newAtom(var, Atom.newAtom(data[i])));
		}
		outputMinMax(var, Atom.newAtom(min), Atom.newAtom(max));
	}

	private void outputData(String var, double[] data){
		double min = data[0], max = data[0];
		for(int i = 0; i < data.length; i++){
			if(data[i] < min) min = data[i];
			if(data[i] > max) max = data[i];
			outlet(0, new Atom[]{Atom.newAtom(var), Atom.newAtom(data[i])});
		}
		outputMinMax(var, Atom.newAtom(min), Atom.newAtom(max));
	}

	private void outputData(String var, double[][] data){
		double min = data[0][0], max = data[0][0];
		for(int i = 0; i < data.length; i++){
			for(int j = 0; j < data[i].length; j++){
				if(data[i][j] < min) min = data[i][j];
				if(data[i][j] > max) max = data[i][j];
			}
			outlet(0, Atom.newAtom(var, Atom.newAtom(data[i])));
		}
		outputMinMax(var, Atom.newAtom(min), Atom.newAtom(max));
	}

	private void outputData(String var, float[] data){
		float min = data[0], max = data[0];
		for(int i = 0; i < data.length; i++){
			if(data[i] < min) min = data[i];
			if(data[i] > max) max = data[i];
			outlet(0, new Atom[]{Atom.newAtom(var), Atom.newAtom(data[i])});
		}
		outputMinMax(var, Atom.newAtom(min), Atom.newAtom(max));
	}

	private void outputData(String var, float[][] data){
		float min = data[0][0], max = data[0][0];
		for(int i = 0; i < data.length; i++){
			for(int j = 0; j < data[i].length; j++){
				if(data[i][j] < min) min = data[i][j];
				if(data[i][j] > max) max = data[i][j];
			}
			outlet(0, Atom.newAtom(var, Atom.newAtom(data[i])));
		}
		outputMinMax(var, Atom.newAtom(min), Atom.newAtom(max));
	}

	private void outputEpochData(String var, double[] data){
		double min = data[0], max = data[0];
		for(int i = 0; i < data.length; i++){
			if(data[i] < min) min = data[i];
			if(data[i] > max) max = data[i];
			outlet(0, Atom.newAtom(var, Atom.newAtom(Epoch.breakdown(data[i]))));
		}
		outlet(OUT_OSC, Atom.newAtom("/" + var + "/min", Atom.newAtom(Epoch.breakdown(min))));
		outlet(OUT_OSC, Atom.newAtom("/" + var + "/max", Atom.newAtom(Epoch.breakdown(max))));
	}

	public void epoch2string(double e){
		outlet(OUT_EPOCH, Epoch.encode(e));
	}

	public void epoch2string(long[] e){
		if(e.length < 7){
			error("cdf: epoch must be a list of 7 integers: year, month, day, hour, minute, second, msec.");
			return;
		}
		try{
			outlet(OUT_EPOCH, Epoch.encode(Epoch.compute(e[0], e[1], e[2], e[3], e[4], e[5], e[6])));
		} catch(Exception ee){
			error("cdf: couldn't convert epoch to string.");
			ee.printStackTrace();
		}
	}

	public void string2epoch(String e){
		try{
			outlet(OUT_EPOCH, Epoch.breakdown(Epoch.parse(e)));
		} catch(Exception ee){
			error("cdf: couldn't convert epoch to string.");
			ee.printStackTrace();
		}
	}
}