import com.cycling74.max.*;
import java.util.LinkedHashMap;
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.util.regex.*;

public class SDIF_merge extends MaxObject{
	ArrayList<String> filenames = new ArrayList<String>();
	ArrayList<Integer> streamIDs = new ArrayList<Integer>();
	ArrayList<String> frameTypes = new ArrayList<String>();
	ArrayList<Float> startTimes = new ArrayList<Float>();
	LinkedHashMap<String, String> files = new LinkedHashMap<String, String>();
	Cell selection = new Cell();
	String outputFile;


	public class Cell{
		public int row, column;
		public Atom val;
		public Cell(int c, int r){
			row = r;
			column = c;
			val = null;
		}
		public Cell(){
			this(0, 0);
		}
		public void set(int c, int r){
			row = r;
			column = c;
			val = null;
		}
		public void set(int c, int r, Atom v){
			row = r;
			column = c;
			val = v;
		}
	}

	public SDIF_merge(){
		declareIO(4, 2);
		createInfoOutlet(false);
		outputFile = null;
		//sendClear(); // doesn't work from constructor!!
	}

	public void add(String filename){
		String absFilename = MaxSystem.locateFile(filename);
		if(absFilename == null){
			error("SDIF_merge: Couldn't locate " + filename + ". Make sure it's in your searchpath.");
			return;
		}
		files.put(absFilename, "");
		scanFiles();
	}

	private void scanFiles(){
		String[] command = new String[2];
		command[0] = "/usr/local/bin/streamlist";
		filenames.clear();
		streamIDs.clear();
		frameTypes.clear();
		startTimes.clear();
		for(String key : files.keySet()){
			command[1] = key;
			String[] ret = launchProc(command);
			for(String st : ret){
				if(!st.startsWith("List of streams")){
					addInfoToArrays(key, st);
					//post(key + " " + st);
				}
			}
		}
		outputInfo();
	}

	private void addInfoToArrays(String filename, String streamList){
		Pattern pattern = Pattern.compile("\\s<stream id=\"(-?\\d+)\"/>\\s+frame type ([0-9A-Z]{4}), starts at time (.*)$");
		Matcher matcher = pattern.matcher(streamList);
		if(!matcher.find()){
			error("SDIF_merge: " + streamList + " does not appear to be a valid stream list");
		}

		filenames.add(filename);
		streamIDs.add(new Integer(matcher.group(1)));
		frameTypes.add(matcher.group(2));
		startTimes.add(new Float(matcher.group(3)));
	}

	public void outputFile(String filename){
		outputFile = filename;
	}

	public void bang(){
		if(outputFile == null){
			error("SDIF_merge: you must specify an output file by calling outputFile");
			return;
		}
		if(files.size() < 2){
			error("SDIF_merge: you must specify more than one file to merge");
	        }
		String[] command = new String[files.size() + 2];
		command[0] = "/usr/local/bin/merge-sdif";
		int i = 1;
		for(String key : files.keySet()){
			command[i] = key;
			i++;
		}
		command[command.length - 1] = outputFile;

		String[] ret = launchProc(command);
		for(String st : ret){
			post(st);
		}
	}

	private String[] changeStreamID(String filename, Integer from, Integer to){
		String[] command = new String[4];
		command[0] = "/usr/local/bin/change-streamID";
		command[1] = filename;
		command[2] = from.toString();
		command[3] = to.toString();
		post(command[0] + " " + command[1] + " " + command[2] + " " + command[3]);
		return launchProc(command);
	}

	private String[] launchProc(String[] command){
		Process p;
		final ArrayList<String> ret = new ArrayList<String>();
		try{p = Runtime.getRuntime().exec(command);
			final BufferedReader stdout, stderr;
			InputStream is = p.getInputStream();
			stdout = new BufferedReader(new InputStreamReader(is));
			is = p.getErrorStream();
			stderr = new BufferedReader(new InputStreamReader(is));

			Thread stdoutThread = new Thread(){
					public void run(){
						try{
							String line = null;
							while((line = stdout.readLine()) != null){
								ret.add(line);
								//post(line + "\n");
							}
							stdout.close();
						}catch(Exception e){
							e.printStackTrace();
						}
					}
				};
			stdoutThread.start();
			Thread stderrThread = new Thread(){
					public void run(){
						try{
							String line = null;
							while((line = stderr.readLine()) != null){
								error("SDIF_merge: " + line + "\n");
							}
							stderr.close();
						}catch(Exception e){
							e.printStackTrace();
						}
					}
				};
			stderrThread.start();

			int exitVal = p.waitFor();
			//post("exit code %d" + exitVal);
		
		}catch(Exception e){
			e.printStackTrace();
			return null;
		}
		return ret.toArray(new String[0]);

	}

	private void outputInfo(){
		int row, col;
		row = 0;
		sendClear();
		for(int r = 0; r < filenames.size(); r++){
			outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(0), Atom.newAtom(r), Atom.newAtom(filenames.get(r))});
			outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(1), Atom.newAtom(r), Atom.newAtom(streamIDs.get(r))});
			outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(2), Atom.newAtom(r), Atom.newAtom(frameTypes.get(r))});
			outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(3), Atom.newAtom(r), Atom.newAtom(startTimes.get(r))});
		}
	}

	public void clear(){
		filenames.clear();
		streamIDs.clear();
		frameTypes.clear();
		startTimes.clear();
		files.clear();
		outputFile = null;
		sendClear();
	}

	private void sendClear(){
		outlet(0, new Atom[]{Atom.newAtom("clear"), Atom.newAtom("all")});
	}

	public void sync(Atom[] args){
		if(args[1].toInt() > 1){
			return;
		}
		int col = args[1].toInt();
		int row = args[2].toInt();
		Atom val = null;
		ArrayList l = getArrayListForCol(col);
		switch(col){
		case 0:
			val = Atom.newAtom((String)l.get(row));
			break;
		case 1:
			val = Atom.newAtom((Integer)l.get(row));
			break;
		}
		selection.set(col, row, val);
	}

	public void set(Atom[] args){
		if(!selection.val.equals(args[0])){
			switch(selection.column){
			case 0:
				// rename file
				post("rename " + selection.val.toString() + " to " + args[0].toString());
				break;
			case 1:
				// change stream id
				changeStreamID(filenames.get(selection.row), selection.val.toInt(), args[0].toInt());
				break;
			case 2:
				// throw error
				break;
			case 3:
				// throw error
				break;
			}
			selection.val = args[0];
			scanFiles();
		}
	}

	public void anything(String msg, Atom[] args){
		switch(getInlet()){
		case 0:

			break;
		case 1:

			break;
		case 2:
			// sync
			break;
		case 3:

			break;
		}
	}

	private ArrayList getArrayListForCol(int col){
		switch(col){
		case 0:
			return filenames;
		case 1:
			return streamIDs;
		case 2:
			return frameTypes;
		case 3:
			return startTimes;
		default:
			return null;
		}
	}
}

