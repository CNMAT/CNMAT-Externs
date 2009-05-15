import com.cycling74.max.*;
import com.cycling74.jitter.*;
import java.io.*;
import java.text.*;
import java.util.*;
import java.lang.reflect.*;
import gsfc.nssdc.cdf.*;
import gsfc.nssdc.cdf.util.*;

public class cdf extends MaxObject implements CDFConstants{
	CDF cdf = null;
	int OUT_BANG = 1;
	int OUT_INFO = 2;
	public static boolean didcopyright = false;
	
	public cdf(){
		if(!didcopyright){
			post("CDF: version 1.0 by John MacCallum, Copyright (c) 2008 Regents of the University of California. All rights reserved.");
			didcopyright = true;
		}
		createInfoOutlet(false);
		declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL, DataTypes.ALL});
		/*
		String[] st = MaxSystem.getSearchPath();
		for(int i = 0; i < st.length; i++){
			post(st[i] + "\n");
		}
		post("**************************************************");
		post("default path: " + MaxSystem.getDefaultPath());
		post("lib = " + MaxSystem.locateFile("libcdfNativeLibrary.jnilib"));
		String libpath = MaxSystem.locateFile("libcdfNativeLibrary.jnilib");
		System.load(libpath);
		post(System.getProperty("java.library.path") + "\n");
		*/
	}

	public void read(){
		String filepath;
		filepath = MaxSystem.openDialog();
		if(filepath == null) return;
		read(filepath);
	}

	public void read(String fn){
		//String filepath = MaxSystem.locateFile(fn);
		java.net.URI uri = null;
		try{
			uri = new java.net.URI(fn);
		}
		catch(Exception e){
			e.printStackTrace();
			return;
		}
		String filepath = "";
		if(!uri.isAbsolute()){
			//filepath = MaxSystem.getDefaultPath() + fn;
			filepath = MaxSystem.locateFile(fn);
		}
		else{
			filepath = fn;
		}
		post("filepath = " + filepath);

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
		outlet(OUT_INFO, new Atom[]{Atom.newAtom("/varname"), Atom.newAtom("clear")});
		for(int i = 0; i < v.size(); i++){
			outlet(OUT_INFO, new Atom[]{Atom.newAtom("/varname"), Atom.newAtom(v.elementAt(i).getName())});
		}
		outlet(OUT_INFO, new Atom[]{Atom.newAtom("/varname"), Atom.newAtom("done")});
	}

	public void getAllRecordsForVarName(final String[] vars){
		Thread t = new Thread(){
				public void run(){
					for(int i = 0; i < vars.length; i++){
						Variable v = getVariable(vars[i]);
						if(do_getRecordsForVarName(v, vars[i], 0, getNumRecords(v) - 1)){
							error("cdf: there was an error getting data for " + vars[i]);
							return;
						}
					}
					//outlet(OUT_BANG, "bang");
				}
			};
		t.start();
	}

	public void getAllRecordsForVarName(String var){
		Variable v = getVariable(var);
		if(do_getRecordsForVarName(v, var, 0, getNumRecords(v) - 1)){
			error("cdf: there was an error getting data for " + var);
		}//else outlet(OUT_BANG, "bang");
	}

	public void getRecordsForVarName_from(String var, long start){
		Variable v = getVariable(var);
		if(do_getRecordsForVarName(v, var, start, getNumRecords(v) - 1)){
			error("cdf: there was an error getting data for " + var);
		}//else outlet(OUT_BANG, "bang");
	}

	private boolean do_getRecordsForVarName(Variable v, String var, long start, long end){
		long n = 0L;
		CDFData d = null;
		try{
			n = v.getNumWrittenRecords();
			d = v.getRecordsObject(0L, n);
		}
		catch(Exception e){
			error("cdf: error getting data for " + var);
			e.printStackTrace();
			return true;
		}

		outlet(OUT_INFO, new Atom[]{Atom.newAtom("/num-records/" + var), Atom.newAtom(n)});

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

	private void outputData(String var, long[] data){
		JitterMatrix jm = new JitterMatrix(1, "long", data.length, 1);
		for(int i = 0; i < data.length; i++){
			jm.setcell(new int[]{i, 0}, 0, data[i]);
		}
		outlet(0, var, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(jm.getName())});
	}

	private void outputData(String var, long[][] data){
		//we assume that this 2d data will always have a number of dimensions consistent with the first array.
		//this is safe for the SSL project for which this object was written, but perhaps not for other uses.
		JitterMatrix jm = new JitterMatrix(1, "long", data.length, data[0].length);
		for(int i = 0; i < data.length; i++){
			for(int j = 0; j < data[i].length; j++){
				jm.setcell(new int[]{i, j}, 0, data[i][j]);
			}
		}
		outlet(0, var, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(jm.getName())});
	}

	private void outputData(String var, double[] data){
		JitterMatrix jm = new JitterMatrix(1, "float64", data.length, 1);
		for(int i = 0; i < data.length; i++){
			jm.setcell(new int[]{i, 0}, 0, data[i]);
		}
		outlet(0, var, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(jm.getName())});
	}

	private void outputData(String var, double[][] data){
		JitterMatrix jm = new JitterMatrix(1, "float64", data.length, data[0].length);
		for(int i = 0; i < data.length; i++){
			for(int j = 0; j < data[i].length; j++){
				jm.setcell(new int[]{i, j}, 0, data[i][j]);
			}
		}
		outlet(0, var, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(jm.getName())});
	}

	private void outputData(String var, float[] data){
		JitterMatrix jm = new JitterMatrix(1, "float32", data.length, 1);
		for(int i = 0; i < data.length; i++){
			jm.setcell(new int[]{i, 0}, 0, data[i]);
		}
		outlet(0, var, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(jm.getName())});
	}

	private void outputData(String var, float[][] data){
		JitterMatrix jm = new JitterMatrix(1, "float32", data.length, data[0].length);
		for(int i = 0; i < data.length; i++){
			for(int j = 0; j < data[i].length; j++){
				jm.setcell(new int[]{i, j}, 0, data[i][j]);
			}
		}
		outlet(0, var, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(jm.getName())});
	}

	private void outputEpochData(String var, double[] data){
		JitterMatrix jm = new JitterMatrix(7, "long", data.length, 1);
		for(int i = 0; i < data.length; i++){
			double d = data[i];
			long[] e = Epoch.breakdown(d);
			int[] ee = new int[]{(int)e[0], (int)e[1], (int)e[2], (int)e[3], (int)e[4], (int)e[5], (int)e[6]};
			jm.setcell(new int[]{i, 0}, ee);
			//post(i + " " + ee[0] + " " + ee[1] + " " + ee[2] + " " + ee[3] + " " + ee[4] + " " + ee[5] + " " + ee[6]);
		}
		outlet(0, var, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(jm.getName())});
	}

	public void epoch2string(double e){
		outlet(OUT_INFO, "/epoch", Epoch.encode(e));
	}

	public void epoch2string(long[] e){
		if(e.length < 7){
			error("cdf: epoch must be a list of 7 integers: year, month, day, hour, minute, second, msec.");
			return;
		}
		try{
			outlet(OUT_INFO, "/epoch", Epoch.encode(Epoch.compute(e[0], e[1], e[2], e[3], e[4], e[5], e[6])));
		} catch(Exception ee){
			error("cdf: couldn't convert epoch to string.");
			ee.printStackTrace();
		}
	}

	public void string2epoch(String e){
		try{
			outlet(OUT_INFO, "/epoch", Epoch.breakdown(Epoch.parse(e)));
		} catch(Exception ee){
			error("cdf: couldn't convert epoch to string.");
			ee.printStackTrace();
		}
	}


	public Variable getVariable(String var){
		long varID = cdf.getVariableID(var);
		if(varID < 0){
			error("cdf: variable " + var + " doesn't exist.\n");
			return null;
		}
		//post("" + varID);
		Variable v = null;
		try{v = cdf.getVariable(var);}
		catch(Exception e){
			error("cdf: error getting data for " + var);
			e.printStackTrace();
			return null;
		}
		return v;
	}

	private long getNumRecords(Variable v){
		long n = 0L;
		try{n = v.getNumWrittenRecords();}
		catch(Exception e){
			e.printStackTrace();
			return -1;
		}
		return n;
	}
}