/*

tc_format_midifile
by John MacCallum, 2006-7

Copyright (c) 2006, 07.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby
granted, provided that the above copyright notice, this paragraph and
the following two paragraphs appear in all copies, modifications, and
distributions.

Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by John MacCallum, The Center for New Music and Audio Technologies,
University of California, Berkeley. 

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
NAME: tc_format_midifile
DESCRIPTION: Formats the OSC output of tempocurver to be written as a midifile
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006-7
SVN_REVISION: $LastChangedRevision: 622 $
VERSION 0.0: First port of the old js code.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

import com.cycling74.max.*;

import java.io.*;
import java.util.*;
import java.io.FileOutputStream;
import java.lang.Math.*;

import javax.xml.*;
import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.stream.*;
import javax.xml.validation.*;

import org.w3c.dom.*;

import com.sun.org.apache.xml.internal.serialize.OutputFormat;
import com.sun.org.apache.xml.internal.serialize.XMLSerializer;

import org.xml.sax.*;

public class tc_format_midifile extends MaxObject {
	
	private static final int BEAT_EVENT = 1;
	private static final int SUBDIV_EVENT = 2;
	private static final int MARKER_EVENT = 3;

	private TreeMap beatArray = new TreeMap();
	private TreeMap subdivArray = new TreeMap();
	private ArrayList markerArray = new ArrayList();
		
	private TreeSet beatStartTimes = new TreeSet();
	private TreeSet<Float> subdivTimes_in = new TreeSet<Float>();
	private TreeSet subdivTimes_out = new TreeSet();

	private int[][] beatPositions;
	private int[][][] subdivPositions;
	private int[][] markerPositions;
	private HashMap markerStrings = new HashMap();
	
	private int subdivsToOutput[];
	
	// This is the number of subdivs tempocurver puts out by default as /future_subdiv messages.
	private int numSubdivs; 
	
	private float lastTempo = 0;

	private Document musicxmlscore;
	private static final String MUSIC_XML_VERSION = "1.1";	

	private int numBars = 0;

	private int numStaves = 2;

	private int beatStaff = 0;
	private int markerStaff = 1;
	private int[] subdivStaves = null;

	public void version(){
		post("tc_format_midifile Version 0.0, by John MacCallum.\nCopyright (c) 2006-7 Regents of the University of California.  All rights reserved.");
	}

	public tc_format_midifile(Atom[] args){
	
		if(args.length == 0){
			numSubdivs = 8;
			subdivsToOutput = new int[]{numSubdivs};
		} else if(args.length == 1){
			numSubdivs = args[0].toInt();
			subdivsToOutput = new int[]{numSubdivs};
		} else {
			numSubdivs = args[0].toInt();
			subdivsToOutput = new int[args.length - 1];
			for(int i = 0; i < args.length - 1; i++)
				subdivsToOutput[i] = args[i + 1].toInt();
		}

		numStaves = subdivsToOutput.length + 2; // subdivisions separate staves for beats and markers.
		subdivStaves = new int[subdivsToOutput.length];
		for(int i = 0; i < subdivsToOutput.length; i++)
			subdivStaves[i] = i + 2;

		// there is a delay when these classes get instantiated for the first time.
		tcfm_beatEvent b = new tcfm_beatEvent();
		tcfm_subdivEvent s = new tcfm_subdivEvent();
		tcfm_markerEvent m = new tcfm_markerEvent();
		version();
	}	

	/*
	class tcfm_event{
		private int eventType;
		private int beatIndex;
		private int subdivIndex;
		private float startTime;
		private float currentTempo;
		private float targetTempo;
		private float currentPhase;
		private float targetPhase;
		private int markerType;
		private HashMap plan;
		private int rampDirection;
		private String OSCString;
		private float duration;

		//public tcfm_event(int et, int bi, int si, float st, float ct){
			//eventType = et; beatIndex = bi; subdivIndex = si;
			//startTime = st; currentTempo = ct;
		//}
		public void setEventType(int et){eventType = et;}
		public void setBeatIndex(int bi){beatIndex = bi;}
		public void setSubdivIndex(int si){subdivIndex = si;}
		public void setStartTime(float st){startTime = st;}
		public void setCurrentTempo(float ct){currentTempo = ct;}
		public void setTargetTempo(float tt){targetTempo = tt;}
		public void setCurrentPhase(float cp){currentPhase = cp;}
		public void setTargetPhase(float tp){targetPhase = tp;}
		public void setRampDirection(int rd){rampDirection = rd;}
		public void setOSCString(String s){OSCString = s;}
		public void setDuration(float d){duration = d;}

		public void setPlan(Atom ar[]){
			plan = new HashMap();
			plan.put("currentTempo", ar[1].getFloat());
			plan.put("waitTime", ar[2].getFloat());
			plan.put("planTime", ar[4].getFloat());
			plan.put("targetTempo", ar[5].getFloat());
			if(ar[1].getFloat() < ar[5].getFloat())
				rampDirection = 1;
			else if(ar[1].getFloat() < ar[5].getFloat())
				rampDirection = -1;
			else rampDirection = 0;
		}		

		public int getEventType(){return eventType;}
		public int getBeatIndex(){return beatIndex;}
		public int getSubdivIndex(){return subdivIndex;}
		public float getStartTime(){return startTime;}
		public float getCurrentTempo(){return currentTempo;}
		public float getTargetTempo(){return targetTempo;}
		public float getCurrentPhase(){return currentPhase;}
		public float getTargetPhase(){return targetPhase;}
		public int getRampDirection(){return rampDirection;}

		public HashMap getPlan(){return plan;}
		public String getOSCString(){return OSCString;}
		public float getDuration(){return duration;}
	}
	*/

	public void beat(Atom[] args){
		if(args[0].isString()){
			if((args[0].getString()).compareTo(new String("/done")) == 0)
				return;
		}
		int index = args[0].toInt();
		
		tcfm_beatEvent ev = new tcfm_beatEvent(index, args[1].toFloat(), args[2].toFloat());
		beatArray.put(index, ev);
	}
	
	public void subdiv(Atom[] args){
		int index_b = args[0].toInt();
		int index_s = args[1].toInt();
		tcfm_subdivEvent ev = new tcfm_subdivEvent(index_b, index_s, args[2].toFloat(), args[3].toFloat());
		int rampDirection;

		if(index_b == 0 && index_s == 0) ev.setRampDirection(0);
		else{
			if(args[3].toFloat() > lastTempo)
				ev.setRampDirection(1);
			else if(args[3].toFloat() < lastTempo)
				ev.setRampDirection(-1);
			else ev.setRampDirection(0);
		}
		lastTempo = args[3].toFloat();
		
		HashMap hm;
		if(subdivArray.containsKey(index_b))
			hm = (HashMap)subdivArray.get(index_b);
		else hm = new HashMap();
		
		hm.put(index_s, ev);
		subdivArray.put(index_b, hm);

		subdivTimes_in.add(args[2].toFloat());
	}

	public void marker(Atom[] args){
		tcfm_markerEvent ev = new tcfm_markerEvent(args);
		markerArray.add(ev);

	}
	
	public void bang(){
		if(beatArray.size() < 1){
			error("tc_format_midifile: Beat array is empty!");
			return;
		}
		initDocument();
		setupPartList();
		setupParts();
		makeBars();
		setupFirstBarAttributes();
		fillPositionArrays();
	      	makeBeats();
		makeSubdivs();
		makeMarkers();
	}
	
	public void makeBeats(){
        	Iterator subdiv_keys = (subdivArray.keySet()).iterator();
		int currentBar = 0;
		int posInBar = 0;

		while(subdiv_keys.hasNext()){
			int key = ((Integer)subdiv_keys.next()).intValue();
			int len = ((HashMap)subdivArray.get(key)).size();

			if(key >= 0) beatPositions[currentBar][posInBar] = 1;
			if(posInBar + len >= numSubdivs * 4) ++currentBar;
			posInBar = (posInBar + len) % (numSubdivs * 4);
		}
		cleanupArray(beatPositions);
		makeMusicXMLScore(beatPositions, beatStaff);
		for(int i = 0; i < numBars; i++){
			Float[] times = subdivTimes_in.toArray(new Float[0]);
			insertStringBeforeNote(formatTimeString(times[((numSubdivs * 4) * i)]), beatStaff, i, 0);
		}
	}
	
	public void makeSubdivs(){
		Iterator key = subdivArray.keySet().iterator();
		int currentBar = 0;
		int posInBar = 0;
		int beat = 0;
		while(key.hasNext()){
			int len = ((HashMap)subdivArray.get(key.next())).size();
			for(int subdiv = 0; subdiv < len; subdiv++){
				for(int i = 0; i < subdivsToOutput.length; i++){
					if(subdiv % (numSubdivs / subdivsToOutput[i]) == 0) 
						subdivPositions[i][currentBar][posInBar] = 1;
				}
				if(posInBar + 1 >= numSubdivs * 4) ++currentBar;
				posInBar = (posInBar + 1) % (numSubdivs * 4);
			}
			++beat;
		}
		for(int i = 0; i < subdivPositions.length; i++){
			cleanupArray(subdivPositions[i]);
			makeMusicXMLScore(subdivPositions[i], subdivStaves[i]);
		}
	}
	
	public void makeMarkers(){
		tcfm_markerEvent ev;
		int beatPos, subdivPos;

		for(int i = 0; i < markerArray.size(); i++){
			ev = (tcfm_markerEvent)markerArray.get(i);
			float time = (float)ev.getStartTime();
			subdivPos = ((SortedSet)subdivTimes_in.headSet(time)).size();
			int beatIndex = ((int)Math.floor(subdivPos / (numSubdivs * 4)));
			int subdivIndex = (subdivPos % (numSubdivs * 4));
			markerPositions[beatIndex][subdivIndex] = 1;
			String thisMarkerString = ev.getOSCString();
			if(thisMarkerString != null){
				if(markerStrings.containsKey(subdivPos)){
					markerStrings.put(subdivPos, (String)markerStrings.get(subdivPos) + "\n" + thisMarkerString);
				} else
					markerStrings.put(subdivPos, thisMarkerString);
			}			
		}

		makeMusicXMLScore(markerPositions, markerStaff);
		Iterator keys = markerStrings.keySet().iterator();
		while(keys.hasNext()){
			subdivPos = (Integer)keys.next();
			int bar = (int)Math.floor(subdivPos / (numSubdivs * 4));
			int subdiv = subdivPos % (numSubdivs * 4);
			insertStringBeforeNote((String)markerStrings.get(subdivPos), markerStaff, bar, subdiv);
		}
	}

	public void cleanupArray(int[][] ar){
		int counter = 0;
		for(int i = 0; i < ar.length; i++){
			for(int j = (numSubdivs * 4) - 1; j >= 0; j--){
				if(j % (numSubdivs / 2) == (numSubdivs / 2) - 1) counter = 0;
				if(ar[i][j] < 0) ++counter;
				else counter = 0;
				
				if(counter > 0 && (j & 1) == 0){
					ar[i][j] = -1 * counter;
					for(int k = 0; k < counter - 1; k++){
						ar[i][j + k + 1] = 0;
					}
				}
			}
		}
	}

	private void insertStringBeforeNote(String s, int p, int bar, int subdiv){
		NodeList parts = musicxmlscore.getElementsByTagName("part");
		Element part = (Element)(parts.item(p));
		NodeList bars = part.getElementsByTagName("measure");
		Element bar_element = (Element)bars.item(bar);
		NodeList notes = bar_element.getElementsByTagName("note");
		Node note_element = (Node)notes.item(subdiv);
		Element direction = musicxmlscore.createElement("direction");
		direction.setAttribute("placement", "above");
		Element direction_type = musicxmlscore.createElement("direction-type");
		Element words = musicxmlscore.createElement("words");
		words.appendChild(musicxmlscore.createTextNode(s));
		direction_type.appendChild(words);
		direction.appendChild(direction_type);
		bar_element.insertBefore((Node)direction, note_element);
	}

	private void makeMusicXMLScore(int[][] ar, int p){
		NodeList parts = musicxmlscore.getElementsByTagName("part");
		Element part = (Element)(parts.item(p));
		NodeList bars = part.getElementsByTagName("measure");
		for(int i = 0; i < ar.length; i++){
			Element bar = ((Element)bars.item(i));
			Element prevNote = null;

			for(int j = 0; j < ar[i].length; j++){
				if(ar[i][j] != 0){
					Element note = musicxmlscore.createElement("note");
					if(ar[i][j] < 0){
						note.appendChild(musicxmlscore.createElement("rest"));
					} else if(ar[i][j] > 0){
						note.appendChild(getPitch(71));
					}
					Element dur = musicxmlscore.createElement("duration");
					dur.appendChild(musicxmlscore.createTextNode("" + Math.abs(ar[i][j])));
					Element type = musicxmlscore.createElement("type");
					type.appendChild(musicxmlscore.createTextNode("" + getDuration(Math.abs(ar[i][j]))[0]));
					note.appendChild(dur);
					note.appendChild(type);
					/*
					Element beam = musicxmlscore.createElement("beam");
					beam.setAttribute("number", "" + 1);

					if(start[beat] == j)
						beam.appendChild(musicxmlscore.createTextNode("start"));
					else if(end[beat] == j)
						beam.appendChild(musicxmlscore.createTextNode("end"));
					else 
						beam.appendChild(musicxmlscore.createTextNode("continue"));
					note.appendChild(beam);
					*/
					bar.appendChild(note);
				}
			}
		}
	}

	public void clear(){
		musicxmlscore = null;
		beatArray.clear();
		subdivArray.clear();
		markerArray.clear();
		beatStartTimes.clear();
		subdivTimes_in.clear();
		subdivTimes_out.clear();
		beatPositions = null;
		subdivPositions = null;
		markerPositions = null;
		markerStrings.clear();
		lastTempo = 0;
		numBars = 0;
	}
	
	// Printing 
	public void print_beat_array(){
		int i;
		tcfm_beatEvent ev;
		post("size: " + beatArray.size() + "\n");
		for(i = 0; i < beatArray.size(); i++){
			ev = (tcfm_beatEvent)beatArray.get(i);
			post(" Beat Index: " + ev.getBeatIndex() + " Start Time: " + ev.getStartTime() + " Current Tempo: " + ev.getCurrentTempo());
		}
	}

	public void print_subdiv_array(){
		tcfm_subdivEvent ev;
		HashMap hm;
		post("size: " + subdivArray.size() + "\n");
		for(int i = 0; i < subdivArray.size(); i++){
			hm = (HashMap)subdivArray.get(i);
			for(int j = 0; j < hm.size(); j++){
				ev = (tcfm_subdivEvent)hm.get(j);
				post("Beat Index: " + ev.getBeatIndex() + " Subdiv Index: " + " " + ev.getSubdivIndex() + " Start Time: " + ev.getStartTime()
				     + " Current Tempo: " + ev.getCurrentTempo());
			}
		}
	}

	/*	
	public void print_marker_array(){
		tcfm_markerEvent ev;
				
		Set markerKeys = (Set)(markerArray.keySet());
		Iterator markerKeys_it = markerKeys.iterator();
		Collection markerValues;
		Iterator markerValues_it;
		String key;
		
		while(markerKeys_it.hasNext()){
			key = (String)markerKeys_it.next();
			markerValues = (Collection)(((HashMap)(markerArray.get(key))).values());
			markerValues_it = markerValues.iterator();
			while(markerValues_it.hasNext()){
				ev = (tcfm_markerEvent)markerValues_it.next();
				if(key.compareTo("plan") == 0){
					post(key + " Current Tempo: " + (ev.getPlan()).get("currentTempo") + " Wait Time: " + (ev.getPlan()).get("waitTime") + 
					     " Plan Time: " + (ev.getPlan()).get("planTime") + " Target Tempo: " + (ev.getPlan()).get("targetTempo"));
				} else
					post(key + " Time: " + ev.getStartTime() + (ev.getCurrentTempo() == 0 ? "" : " Current tempo: " + ev.getCurrentTempo()));
			}
		}
	}
	*/

	private void initDocument() {
		SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		
		String f = MaxSystem.locateFile("partwise.xsd");

		try{
			Source schemaFile = new StreamSource(new File(f));
			Schema schema = factory.newSchema(schemaFile);
			dbf.setSchema(schema);
			dbf.setValidating(true);
		} catch(SAXException e){
			error("musicxml: error reading DTD file(s)\n" + e);
			post("Parsing XML failed due to a:\n" + e.getClass().getName()
			     + "\n in Line number:" + ((SAXParseException) e).getLineNumber()
			     + "\n Coloumn Number:" + ((SAXParseException) e).getColumnNumber()
			     + "\n Xml schema file is:" + ((SAXParseException) e).getSystemId() + "\nSTACKTRACE: ");
			e.printStackTrace();
			return;
		}
		DocumentBuilder db;		
		try {
			db = dbf.newDocumentBuilder();
			DOMImplementation musicxmlscoreImp = db.getDOMImplementation();
			//DocumentType dt = musicxmlscoreImp.initDocumentType("score-partwise", "-//Recordare//DTD MusicXML 1.1 Partwise//EN", null);
			//musicxmlscore = musicxmlscoreImp.initDocument("http://www.musicxml.org/dtds/partwise.dtd", "score-partwise", dt);
			musicxmlscore = musicxmlscoreImp.createDocument("http://www.musicxml.org/dtds/partwise.dtd", "score-partwise", null);
		}catch(ParserConfigurationException pce) {
			error("Error while trying to instantiate DocumentBuilder\n" + pce);
			return;
		}
		
		// add in the first couple of elements.
		Element rootElement = musicxmlscore.getDocumentElement();
		rootElement.setAttribute("version", MUSIC_XML_VERSION);
		
	}

	private void setupPartList(){
		Element rootElement = musicxmlscore.getDocumentElement();
		Element part_list_element = musicxmlscore.createElement("part-list");
		Element part_id_element;
		Element part_name_element;
		Text part_name;
		// create parts for all of the subdivsToOutput plus the main line.
		for(int i = 0; i < numStaves; i++){
			part_id_element = musicxmlscore.createElement("score-part");
			part_id_element.setAttribute("id", "P" + i);
			
			part_name_element = musicxmlscore.createElement("part-name");
			part_name = musicxmlscore.createTextNode("Part " + i);
			part_name_element.appendChild(part_name);
			part_id_element.appendChild(part_name_element);
			
			part_list_element.appendChild(part_id_element);
		}
		rootElement.appendChild(part_list_element);
	}

	private void setupParts(){
		Element rootElement = musicxmlscore.getDocumentElement();
		Element part;
		for(int i = 0; i < numStaves; i++){
			part = musicxmlscore.createElement("part");
			part.setAttribute("id", "P" + i);
			rootElement.appendChild(part);
		}
	}

	private void setupFirstBarAttributes(){
		NodeList parts = musicxmlscore.getElementsByTagName("part");
		Element attributes = musicxmlscore.createElement("attributes");
		
		// divisions
		// hard coded 32nd notes for now
		Element divisions_element = musicxmlscore.createElement("divisions");
		Text divisions = musicxmlscore.createTextNode("" + 8);
		divisions_element.appendChild(divisions);
		attributes.appendChild(divisions_element);
		
		// time sig
		// hard coding 4/4 for now
		Element time = musicxmlscore.createElement("time");
		Element beats_element = musicxmlscore.createElement("beats");
		Text beats = musicxmlscore.createTextNode("4");
		beats_element.appendChild(beats);
		Element beat_type_element = musicxmlscore.createElement("beat-type");
		Text beat_type = musicxmlscore.createTextNode("4");
		beat_type_element.appendChild(beat_type);
		time.appendChild(beats_element);
		time.appendChild(beat_type_element);
		attributes.appendChild(time);
		
		// clef
		Element clef = musicxmlscore.createElement("clef");
		Element sign_element = musicxmlscore.createElement("sign");
		Text sign = musicxmlscore.createTextNode("G");
		sign_element.appendChild(sign);
		Element line_element = musicxmlscore.createElement("line");
		Text line = musicxmlscore.createTextNode("" + 2);
		line_element.appendChild(line);
		clef.appendChild(sign_element);
		clef.appendChild(line_element);
		attributes.appendChild(clef);
		
		for(int i = 0; i < parts.getLength(); i++){
			Element part = (Element)parts.item(i);
			NodeList bars = part.getElementsByTagName("measure");
			Node firstBar = (Node)bars.item(0);
			//firstBar.insertBefore(attributes.cloneNode(true), firstBar.getFirstChild());
			firstBar.appendChild(attributes.cloneNode(true));
		}
	}

	private void makeBars(){
		int totalNumSubdivs = 0;
		Iterator keys = ((Set)subdivArray.keySet()).iterator();
		while(keys.hasNext()){
			totalNumSubdivs += ((HashMap)subdivArray.get(keys.next())).size();
		}
		int totalNumBars = (int)Math.ceil((double)totalNumSubdivs / ((double)numSubdivs * 4.));

		for(int i = 0; i < totalNumBars; i++){
			++numBars;
			addBarToMusicXMLScore();
		}
	}

	private void fillPositionArrays(){
		beatPositions = new int[numBars][4 * numSubdivs];
		subdivPositions = new int[numStaves - 2][numBars][4 * numSubdivs];
		markerPositions = new int[numBars][4 * numSubdivs];
		for(int i = 0; i < numBars; i++){
			for(int j = 0; j < 4 * numSubdivs; j++){
				beatPositions[i][j] = -1;
			}
		}

		for(int i = 0; i < numBars; i++){
			for(int j = 0; j < 4 * numSubdivs; j++){
				markerPositions[i][j] = -1;
			}
		}

		for(int k = 0; k < numStaves - 2; k++){
			for(int i = 0; i < numBars; i++){
				for(int j = 0; j < 4 * numSubdivs; j++){
					subdivPositions[k][i][j] = -1;
				}
			}
		}
	}

   	private void addBarToMusicXMLScore(){
	    	NodeList parts = musicxmlscore.getElementsByTagName("part");
	    	Element part;
	    	Element bar;

		for(int i = 0; i < parts.getLength(); i++){
			part = (Element)parts.item(i);
			bar = musicxmlscore.createElement("measure");
			bar.setAttribute("number", "" + numBars);
			part.appendChild(bar);
		}
	}

	private String[] getDuration(int dur){
		float dec = ((float)dur) / ((float)numSubdivs);
		if(dec == .125) return new String[]{"32nd"};
		else if(dec == 0.25) return new String[]{"16th"};
		else if(dec == .375) return new String[]{"16th", "32nd"};
		else if(dec == .5) return new String[]{"eighth"};
		else if(dec == .625) return new String[]{"eighth", "32nd"};
		else if(dec == .75) return new String[]{"eighth", "16th"};
		else if(dec == .875) return new String[]{"eighth", "16th", "32nd"};
		else if(dec == 1.0) return new String[]{"quarter"};
		else return null;
	}
	
	private Element getPitch(float m){
		//Element note = musicxmlscore.createElement("note");
		Element pitch = musicxmlscore.createElement("pitch");
		Element step = musicxmlscore.createElement("step");
		Element accidental = musicxmlscore.createElement("alter");
		Element octave = musicxmlscore.createElement("octave");
			
		float midiPitch = m;
		float acc = 0;
		float[] accidentals = new float[]{0.f, 1.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f, 1.f, 0.f};
		String[] pitches = new String[]{"C", "C", "D", "D", "E", "F", "F", "G", "G", "A", "A", "B"};

		float decimal = midiPitch - (float)(Math.floor(midiPitch));
		if(decimal < .25){
			midiPitch = (float)Math.floor(midiPitch);
		} else if(decimal < .75 && decimal >= .25){
			midiPitch = (float)Math.floor(midiPitch) + .5f;
			acc = 0.5f;
		} else{
			midiPitch = (float)Math.floor(midiPitch) + 1;
			acc = -0.5f;
		}

		octave.appendChild(musicxmlscore.createTextNode("" + (int)(((float)Math.floor(midiPitch) / 12.f) - 1.f)));

		while(midiPitch > 11) midiPitch -= 12;

		step.appendChild(musicxmlscore.createTextNode(pitches[(int)midiPitch]));
		accidental.appendChild(musicxmlscore.createTextNode("" + (acc + accidentals[(int)midiPitch])));


		pitch.appendChild(step);
		if((acc + accidentals[(int)midiPitch]) != 0) pitch.appendChild(accidental);
		pitch.appendChild(octave);
		//note.appendChild(pitch);
		return pitch;
	}

	private String formatTimeString(float t){
		int minute = (int)Math.floor(t / 60);
		float second = t - (minute * 60);
		return minute + "\' " + second + "\"";
	}

	public void writeFile(String path){
		try{
			OutputFormat format = new OutputFormat(musicxmlscore);
			format.setIndenting(true);
			format.setDoctype("-//Recordare//DTD MusicXML 1.1 Partwise//EN",
					"http://www.musicxml.org/dtds/partwise.dtd");
			format.setEncoding("UTF-8");
			format.setVersion("1.0");
			format.setStandalone(false);
			format.setOmitXMLDeclaration(false);
			format.setOmitDocumentType(false);

			OutputStream outputstream = new FileOutputStream(new File(path));

			XMLSerializer serializer = new XMLSerializer(outputstream, format);
			//serializer.serialize(dom);                                                               
			serializer.setOutputFormat(format);
			serializer.asDOMSerializer();
			serializer.serialize(musicxmlscore);
			outputstream.flush();
			outputstream.close();

		} catch(IOException ie) {
			error("musicxml: error writing file.");
			ie.printStackTrace();
		}
	}

	public void dumpToMidifile(){
		Iterator it = (beatArray.keySet()).iterator();
		while(it.hasNext()){
			tcfm_beatEvent e = (tcfm_beatEvent)(beatArray.get(it.next()));
			outlet(0, new Atom[]{Atom.newAtom("/beat"), Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(e.getPitch())});
		}
		it = (subdivArray.keySet()).iterator();
		while(it.hasNext()){
			HashMap h = (HashMap)subdivArray.get(it.next());
			Iterator subdiv_it = (h.keySet()).iterator();
			while(subdiv_it.hasNext()){
				Integer subdivIndex = (Integer)subdiv_it.next();
				tcfm_subdivEvent e = (tcfm_subdivEvent)h.get(subdivIndex);
				for(int i = 0; i < subdivsToOutput.length; i++){
					if(subdivIndex % subdivsToOutput[i] == 0)
						outlet(0, new Atom[]{Atom.newAtom("/subdiv/" + subdivsToOutput[i]), Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(e.getPitch())});
				}
			}
		}
		for(int i = 0; i < markerArray.size(); i++){
			tcfm_markerEvent e = (tcfm_markerEvent)markerArray.get(i);
			outlet(0, new Atom[]{Atom.newAtom("/marker"), Atom.newAtom("setText"), Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(e.getOSCString())});
			outlet(0, new Atom[]{Atom.newAtom("/marker"), Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(71)});
		}
	}

	class tcfm_beatEvent{
		public int beatIndex;
		public float startTime;
		public float currentTempo;
		public float pitch = 71;
		public int rampDirection;

		public tcfm_beatEvent(int b, float s, float t){
			beatIndex = b;
			startTime = s;
			currentTempo = t;
		}
		public tcfm_beatEvent(int b, float s, float t, float p){
			beatIndex = b;
			startTime = s;
			currentTempo = t;
			pitch = p;
		}
		public tcfm_beatEvent(){}

		public float getBeatIndex(){ return beatIndex;}
		public float getStartTime(){ return startTime;}
		public float getCurrentTempo(){ return currentTempo;}
		public float getPitch(){ return pitch; }

		public void setPitch(float p){
			pitch = p;
		}

		public float getRampDirection(){return rampDirection;}
		public void setRampDirection(int d){rampDirection = d;}
	}

	class tcfm_subdivEvent extends tcfm_beatEvent{
		private int subdivIndex;
		public tcfm_subdivEvent(int b, int si, float s, float t){
			super(b, s, t);
			subdivIndex = si;
		}
		public tcfm_subdivEvent(int b, int si, float s, float t, float p){
			super(b, s, t, p);
			subdivIndex = si;
		}
		public tcfm_subdivEvent(){}

		public int getSubdivIndex(){return subdivIndex;}
		public void setSubdivIndex(int si){subdivIndex = si;}
	}

	class tcfm_markerEvent{
		private float startTime;
		private float currentTempo;
		private float targetTempo;
		private float currentPhase;
		private float targetPhase;
		private float duration;
		private float waitTime;
		private int rampDirection;
		private String markerString;
		private String OSCString = "";

		public tcfm_markerEvent(Atom[] args){
			startTime = args[0].toFloat();
			markerString = args[1].toString();

			if(markerString.compareTo("/plan") == 0){
				currentTempo = args[2].toFloat();
				waitTime = args[3].toFloat();
				duration = args[5].toFloat();
				targetTempo = args[6].toFloat();
				rampDirection = calcRampDirection(currentTempo, targetTempo);
			} else if(markerString.compareTo("/starting-ramp") == 0){
				currentTempo = args[3].toFloat();
				targetTempo = args[4].toFloat();
				rampDirection = calcRampDirection(currentTempo, targetTempo);
			} else if(markerString.compareTo("/jumped") == 0){
				currentTempo = args[2].toFloat();
				currentPhase = args[3].toFloat();
			} else if(markerString.compareTo("/made-it") == 0){
				currentTempo = args[2].toFloat();
			} else if(markerString.compareTo("/holding") == 0){
				currentTempo = args[2].toFloat();
				rampDirection = 0;
				duration = args[3].toFloat();
			} else if(markerString.compareTo("/finished-hold") == 0){

			}

			for(int i = 0; i < args.length; i++)
				OSCString = OSCString.concat(args[i].toString() + " ");
		}

		public tcfm_markerEvent(){};

		private int calcRampDirection(float ct, float tt){
			if(ct < tt) return 1;
			else if(ct > tt) return -1;
			else return 0;
		}
		public float getStartTime(){return startTime;}
		public float getCurrentTempo(){return currentTempo;}
		public float getTargetTempo(){return targetTempo;}
		public float getCurrentPhase(){return currentPhase;}
		public float getTargetPhase(){return targetPhase;}
		public float getDuration(){return duration;}
		public float getWaitTime(){return waitTime;}
		public int getRampDirection(){return rampDirection;}
		public String getMarkerString(){return markerString;}
		public String getOSCString(){return OSCString;}

	}
}