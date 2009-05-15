import com.cycling74.max.*;
import java.util.LinkedHashMap;
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.util.regex.*;

public class SDIF_merge extends MaxObject{
	LinkedHashMap<String, ArrayList<SDIF_stream>> tab = new LinkedHashMap<String, ArrayList<SDIF_stream>>();
	ArrayList<String> rows = new ArrayList<String>();
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
		}
	}

	public class SDIF_stream{
		private int streamID;
		private String frameType;
		private float startTime;
		public SDIF_stream(String streamList){
			Pattern pattern = Pattern.compile("\\s<stream id=\"(-?\\d+)\"/>\\s+frame type ([0-9A-Z]{4}), starts at time (.*)$");
			Matcher matcher = pattern.matcher(streamList);
			if(!matcher.find()){
				error("SDIF_merge: " + streamList + " does not appear to be a valid stream list");
			}

			streamID = Integer.parseInt(matcher.group(1));
			frameType = matcher.group(2);
			startTime = Float.parseFloat(matcher.group(3));
			/*
			post("count = " + matcher.groupCount());
			for(int i = 1; i <= matcher.groupCount(); i++){
				post("i = " + i + " " + matcher.group(i));
			}
			*/
		}
		public int getStreamID(){return streamID;}
		public void setStreamID(int sid){streamID = sid;}
		public String getFrameType(){return frameType;}
		public void setFrameType(String ft){frameType = ft;}
		public float getStartTime(){return startTime;}
		public void setStartTime(float st){startTime = st;}
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
		//list.put(absFilename, 0);
		String[] command = new String[2];
		command[0] = "/usr/local/bin/streamlist";
		command[1] = absFilename;
		String[] ret = launchProc(command);
		ArrayList<SDIF_stream> streams = new ArrayList<SDIF_stream>();
		for(String st : ret){
			//post(st);
			if(!st.startsWith("List of streams")){
				SDIF_stream stream = new SDIF_stream(st);
				streams.add(stream);
			}
		}
		tab.put(absFilename, streams);
		outputInfo();
	}

	public void outputFile(String filename){
		outputFile = filename;
	}

	public void bang(){
		if(outputFile == null){
			error("SDIF_merge: you must specify an output file by calling outputFile");
			return;
		}
		if(tab.size() < 2){
			error("SDIF_merge: you must specify more than one file to merge");
	        }
		String[] command = new String[tab.size() + 2];
		command[0] = "/usr/local/bin/merge-sdif";
		int i = 1;
		for(String key : tab.keySet()){
			command[i] = key;
			i++;
		}
		command[command.length - 1] = outputFile;

		String[] ret = launchProc(command);
		for(String st : ret){
			post(st);
		}
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
		rows.clear();
		for(String filename : tab.keySet()){
			ArrayList<SDIF_stream> streams = tab.get(filename);
			for(SDIF_stream s : streams){
				rows.add(filename);
				outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(0), Atom.newAtom(row), Atom.newAtom(filename)});
				outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(1), Atom.newAtom(row), Atom.newAtom(s.getStreamID())});
				outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(2), Atom.newAtom(row), Atom.newAtom(s.getFrameType())});
				outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(3), Atom.newAtom(row), Atom.newAtom(s.getStartTime())});
				row++;
			}
			/*
			post(s.getStreamID() + "");
			post(s.getFrameType() + "");
			post(s.getStartTime() + "");
			*/
		}
	}

	public void clear(){
		tab.clear();
		outputFile = null;
		sendClear();
	}

	private void sendClear(){
		outlet(0, new Atom[]{Atom.newAtom("clear"), Atom.newAtom("all")});
	}

	public void sync(Atom[] args){
		int col = args[1].toInt();
		int row = args[2].toInt();
		selection.set(col, row);
		String filename = rows.get(row);
		switch(col){
		case 0:
			selection.val = Atom.newAtom(filename);
			break;
		case 1:
			break;
		}
	}

	public void set(Atom[] args){
		boolean diff = false;
		for(int i = 0; i < args.length; i++){
			if(!args[i].equals(selection.val[i])){
				diff = true;
				break;
			}
		}
		post("in set");
		if(diff){
			post("different");
			switch(selection.column){
			case 0:
				// rename file
				break;
			case 1:
				// change stream id
				post("gonna change stream ID from " + Atom.toOneString(selection.val) + " to " + Atom.toOneString(args));
				break;
			case 2:
				// throw error
				break;
			case 3:
				// throw error
				break;
			}
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
}

