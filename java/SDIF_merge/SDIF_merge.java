import com.cycling74.max.*;
import java.util.LinkedHashMap;
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.InputStream;
import java.io.File;
import java.util.regex.*;


public class SDIF_merge extends MaxObject{
	private ArrayList<String> filenames = new ArrayList<String>();
	private ArrayList<Integer> streamIDs = new ArrayList<Integer>();
	private ArrayList<String> frameTypes = new ArrayList<String>();
	private ArrayList<Float> startTimes = new ArrayList<Float>();
	private LinkedHashMap<String, String> files = new LinkedHashMap<String, String>();
	private Cell selection = new Cell();
	private String outputFile;
	private String merge_sdif_path = null, change_streamID_path = null, streamlist_path = null;
	private boolean verbose = false;
	private boolean displayFullPath = true;
	private MaxQelem q = new MaxQelem(this, "sendClear");

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

		//////////////////////////////////////////////////
		// find merge-sdif
		merge_sdif_path = MaxSystem.locateFile("merge-sdif");
		if(merge_sdif_path == null){
			// we couldn't find merge-sdif in the max searchpath, so 
			// look for it in the user's path env var
			String s = System.getenv("PATH");
			// /usr/local/bin is a good candidate, but it is probably not one
			// of the paths that we'll get with System.getenv("PATH") since
			// that won't includ all paths in .bashrc (or whatever) unless Max
			// was launched from the terminal.
			s = "/usr/local/bin:" + s;
			String[] paths = s.split(":");
			for(String p : paths){
				merge_sdif_path = findFile(p, "merge-sdif");
				if(merge_sdif_path != null){
					break;
				}
			}
			if(merge_sdif_path == null){
				bail("SDIF_merge: couldn't locate command-line program ``merge-sdif'' necessary for operation.  Make sure it is either in the same directory as the SDIF_merge .class files, or somewhere in your search path.");
			}
		}
		//////////////////////////////////////////////////
		// find change-streamID
		change_streamID_path = MaxSystem.locateFile("change-streamID");
		if(change_streamID_path == null){
			String s = System.getenv("PATH");
			s = "/usr/local/bin:" + s;
			String[] paths = s.split(":");
			for(String p : paths){
				change_streamID_path = findFile(p, "change-streamID");
				if(change_streamID_path != null){
					break;
				}
			}
			if(change_streamID_path == null){
				bail("SDIF_merge: couldn't locate command-line program ``change-streamID'' necessary for operation.  Make sure it is either in the same directory as the SDIF_merge .class files, or somewhere in your search path.");
			}
		}
		//////////////////////////////////////////////////
		// find streamlist
		streamlist_path = MaxSystem.locateFile("streamlist");
		if(streamlist_path == null){
			// we couldn't find streamlist in the max searchpath, so 
			// look for it in the user's path env var
			String s = System.getenv("PATH");
			s = "/usr/local/bin:" + s;
			String[] paths = s.split(":");
			for(String p : paths){
				streamlist_path = findFile(p, "streamlist");
				if(streamlist_path != null){
					break;
				}
			}
			if(streamlist_path == null){
				bail("SDIF_merge: couldn't locate command-line program ``streamlist'' necessary for operation.  Make sure it is either in the same directory as the SDIF_merge .class files, or somewhere in your search path.");
			}
		}
		q.set();
		/*
		post(merge_sdif_path);
		post(change_streamID_path);
		post(streamlist_path);
		*/
	}

	private String findFile(String dir, String filename){
		File fp = new File(dir);
		if(!fp.isDirectory()){
			if(dir.equals(filename)){
				return dir;
			}else{
				return null;
			}
		}
		String[] filelist = fp.list();
		for(String f : filelist){
			fp = new File(f);
			if(fp.isDirectory()){
				String ff = findFile(dir + File.separator + f, filename);
				if(ff != null){
					return ff;
				}
			}else{
				if(f.equals(filename)){
					return dir + File.separator + f;
				}
			}
		}
		return null;
	}

	public void add(String filename){
		File fp = new File(filename);
		if(fp.isDirectory()){
			addDir(filename);
			return;
		}
		String[] tmp = filename.split("\\.");
		if(!tmp[tmp.length - 1].equals("sdif")){
			return;
		}
		String absFilename = MaxSystem.locateFile(filename);
		if(absFilename == null){
			error("SDIF_merge: Couldn't locate " + filename + ". Make sure it's in your searchpath.");
			return;
		}
		files.put(absFilename, "");
		scanFiles();
	}

	public void addDir(String dirname){
		File fp = new File(dirname);
		for(String filename : fp.list()){
			add(filename);
		}
	}

	private void scanFiles(){
		String[] command = new String[2];
		command[0] = streamlist_path;
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

	/*
	public void outputFile(String filename){
		outputFile = filename;
	}
	*/

	public void write(){
		write(MaxSystem.saveAsDialog("", ""));
	}

	public void write(String filename){
		if(filename == null){
			return;
		}
		if(files.size() < 2){
			error("SDIF_merge: you must specify more than one file to merge");
	        }
		String[] command = new String[files.size() + 2];
		command[0] = merge_sdif_path;
		int i = 1;
		for(String key : files.keySet()){
			command[i] = key;
			i++;
		}
		command[command.length - 1] = filename;
		if(verbose){
			String s = "";
			for(String st : command){
				s += st + " ";
			}
			post(s);
		}

		String[] ret = launchProc(command);
		for(String st : ret){
			post(st);
		}
	}

	private String[] changeStreamID(String filename, Integer from, Integer to){
		String[] command = new String[4];
		command[0] = change_streamID_path;
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
			String filename = filenames.get(r);
			if(!displayFullPath){
				String[] tmp = filename.split(File.separator);
				filename = tmp[tmp.length - 1];
			}
			outlet(0, new Atom[]{Atom.newAtom("set"), Atom.newAtom(0), Atom.newAtom(r), Atom.newAtom(filename)});
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
		if(files == null || row >= filenames.size()){
			return;
		}
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
				//post("rename " + selection.val.toString() + " to " + args[0].toString());
				File ofp = new File(selection.val.toString());
				File nfp = new File(args[0].toString());
				boolean ret;
				try{ret = ofp.renameTo(nfp);}
				catch(Exception e){
					e.printStackTrace();
					return;
				}
				if(!ret){
					error("SDIF_merge: couldn't rename file");
					return;
				}
				/*
				int index;
				while((index = filenames.indexOf(selection.val.toString())) >= 0){
					filenames.set(index, args[0].toString());
				}
				*/
				files.remove(selection.val.toString());
				files.put(args[0].toString(), "");
				selection.val = args[0];
				outputInfo();
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

	public void verbose(boolean v){
		verbose = v;
	}

	public void displayFullPath(boolean b){
		displayFullPath = b;
		outputInfo();
	}
}

