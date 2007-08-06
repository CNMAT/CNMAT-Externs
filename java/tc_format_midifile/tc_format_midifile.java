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
	private HashMap subdivArray = new HashMap();
	private HashMap markerArray = new HashMap();
		
	private TreeSet beatStartTimes = new TreeSet();
	private TreeSet subdivTimes_in = new TreeSet();
	private TreeSet subdivTimes_out = new TreeSet();
	
	private int subdivsToOutput[];
	
	// This is the number of subdivs tempocurver puts out by default as /future_subdiv messages.
	private int numSubdivs; 
	
	private float lastTempo = 0;

	private Document musicxmlscore;
	private static final String MUSIC_XML_VERSION = "1.1";	

	private int numBars = 0;
	
	private int posInBar = 0; // the position in the bar in subdivisions
	private int offsetInBeat = 0; // this is to keep track of where any syncopation

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
	
		markerArray.put("jumped", new HashMap());
		markerArray.put("made-it", new HashMap());
		markerArray.put("plan", new HashMap());
		markerArray.put("starting-ramp", new HashMap());
		markerArray.put("holding", new HashMap());
		markerArray.put("finished-hold", new HashMap());

	   	tcfm_event ev = new tcfm_event();
		version();
	}	
	
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

		/*public tcfm_event(int et, int bi, int si, float st, float ct){
			eventType = et; beatIndex = bi; subdivIndex = si;
			startTime = st; currentTempo = ct;
		}*/
		public void setEventType(int et){eventType = et;}
		public void setBeatIndex(int bi){beatIndex = bi;}
		public void setSubdivIndex(int si){subdivIndex = si;}
		public void setStartTime(float st){startTime = st;}
		public void setCurrentTempo(float ct){currentTempo = ct;}
		public void setTargetTempo(float tt){targetTempo = tt;}
		public void setCurrentPhase(float cp){currentPhase = cp;}
		public void setTargetPhase(float tp){targetPhase = tp;}
		public void setRampDirection(int rd){rampDirection = rd;}

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

	}

	public void beat(Atom[] args){
		if(args[0].isString()){
			if((args[0].getString()).compareTo(new String("/done")) == 0)
				return;
		}
		int index = args[0].toInt();
		
		tcfm_event ev = new tcfm_event();
		ev.setEventType(BEAT_EVENT);
		ev.setBeatIndex(index);
		ev.setStartTime(args[1].toFloat());
		ev.setCurrentTempo(args[2].toFloat());
		beatArray.put(index, ev);
	}
	
	public void subdiv(Atom[] args){
		int index_b = args[0].toInt();
		int index_s = args[1].toInt();
		
		tcfm_event ev = new tcfm_event();
		ev.setEventType(SUBDIV_EVENT);
		ev.setBeatIndex(index_b);
		ev.setSubdivIndex(index_s);
		ev.setStartTime(args[2].toFloat());
		subdivTimes_in.add(args[2].toFloat());
		ev.setCurrentTempo(args[3].toFloat());
		
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
	}

	public void marker(Atom[] args){
		tcfm_event ev = new tcfm_event();
		ev.setEventType(MARKER_EVENT);
		ev.setStartTime(args[0].getFloat());
		String marker_type = args[1].getString();
		HashMap hm;
			
		if(marker_type.compareTo("/plan") == 0){
			Atom ar[] = new Atom[args.length - 2];
			for(int i = 0; i < args.length - 2; i++)
				ar[i] = args[i + 2];
				
			ev.setPlan(ar);
			hm = (HashMap)(markerArray.get("plan"));
			hm.put(ev.getStartTime(), ev);
		} 
		else if(marker_type.compareTo("/starting-ramp") == 0){
			ev.setCurrentTempo(args[3].toFloat());
			ev.setTargetTempo(args[4].toFloat());
			if(args[3].toFloat() < args[4].toFloat()) ev.setRampDirection(1);
			else if(args[3].toFloat() > args[4].toFloat()) ev.setRampDirection(-1);
			else ev.setRampDirection(0); // this shouldn't happen
			
			hm = (HashMap)(markerArray.get("starting-ramp"));
			hm.put(ev.getStartTime(), ev);
		} 
		else if(marker_type.compareTo("/jumped") == 0){
			ev.setCurrentTempo(args[2].toFloat());
			ev.setCurrentPhase(args[3].toFloat());
			hm = (HashMap)(markerArray.get("jumped"));
			hm.put(ev.getStartTime(), ev);
		} 
		else if(marker_type.compareTo("/made-it") == 0){
			ev.setCurrentTempo(args[2].toFloat());
			hm = (HashMap)(markerArray.get("made-it"));
			hm.put(ev.getStartTime(), ev);
		} 
		else if(marker_type.compareTo("/holding") == 0){
			ev.setCurrentTempo(args[2].toFloat());
			ev.setRampDirection(0);
			hm = (HashMap)(markerArray.get("holding"));
			hm.put(ev.getStartTime(), ev);
		} 
		else if(marker_type.compareTo("/finished-hold") == 0){
			hm = (HashMap)(markerArray.get("finished-hold"));
			hm.put(ev.getStartTime(), ev);
		}
	}
	
	public void bang(){
		initDocument();
		setupPartList();
		setupParts();
		setupFirstBarAttributes();
		dumpBeats();
		dumpSubdivs();
		dumpMarkers();
		makeTimeMap();
	}
	
	public void dumpBeats(){
		if(beatArray.size() < 1){
			error("tc_format_midifile: Beat array is empty!");
			return;
		}
		
		NodeList parts = musicxmlscore.getElementsByTagName("part");
		Element part_element = (Element)parts.item(0);
		NodeList bars = (NodeList)part_element.getElementsByTagName("measure");
		Element bar_element = (Element)bars.item(0);
		Element duration_element = musicxmlscore.createElement("duration");
		Element type_element = musicxmlscore.createElement("type");
		Element note_element;
		
		// hard-coded for now.  this should be open to the user.
		Element pitch_element = musicxmlscore.createElement("pitch");
		Element step_element = musicxmlscore.createElement("step");
		Element octave_element = musicxmlscore.createElement("octave");
		step_element.appendChild(musicxmlscore.createTextNode("B"));
		octave_element.appendChild(musicxmlscore.createTextNode("4"));
		pitch_element.appendChild(step_element);
		pitch_element.appendChild(octave_element);
		
		int dur_subdivs;

		tcfm_event thisBeat;
		HashMap theseSubdivs;
		int startTime, duration;
		int prevTime = 0;
		int prevDur = 0;
		
		for(int i = 0; i < beatArray.size() - 1; i++){
			//thisBeat = (tcfm_event)beatArray.get(i);
			theseSubdivs = (HashMap)subdivArray.get(i);
			dur_subdivs = theseSubdivs.size();
			
			/////////////////////////
			// for musicXML output
			/////////////////////////
			post("dur_subdivs = " + dur_subdivs + " offsetInBeat = " + offsetInBeat + " posInBar = " + posInBar + " numBars = " + numBars);
			if(dur_subdivs > numSubdivs - offsetInBeat){
				note_element = musicxmlscore.createElement("note");
				note_element.appendChild(pitch_element);
				note_element.appendChild(musicxmlscore.createElement("duration"));
				duration_element.appendChild(musicxmlscore.createTextNode("" + (numSubdivs - offsetInBeat)));
				note_element.appendChild(musicxmlscore.createElement("type"));
				duration_element.appendChild(musicxmlscore.createTextNode(getNoteType(numSubdivs - offsetInBeat)));
				dur_subdivs -= (numSubdivs - offsetInBeat);
				offsetInBeat = (offsetInBeat + (numSubdivs - offsetInBeat)) % numSubdivs;
				posInBar += (numSubdivs - offsetInBeat);
				if(posInBar >= 4 * numSubdivs){
					addBarToMusicXMLScore();
					bars = (NodeList)part_element.getElementsByTagName("measure");
					bar_element = (Element)bars.item(numBars);
				}
				post("dur_subdivs = " + dur_subdivs + " offsetInBeat = " + offsetInBeat + " posInBar = " + posInBar + " numBars = " + numBars);
			}
			while(dur_subdivs > numSubdivs){
				note_element = musicxmlscore.createElement("note");
				note_element.appendChild(musicxmlscore.createElement("rest"));
				note_element.appendChild(musicxmlscore.createElement("duration"));
				duration_element.appendChild(musicxmlscore.createTextNode("" + (numSubdivs - offsetInBeat)));
				dur_subdivs -= (numSubdivs - offsetInBeat);
				offsetInBeat = (offsetInBeat + (numSubdivs - offsetInBeat)) % numSubdivs;
				posInBar += (numSubdivs - offsetInBeat);
				if(posInBar >= 4 * numSubdivs){
					addBarToMusicXMLScore();
					bars = (NodeList)part_element.getElementsByTagName("measure");
					bar_element = (Element)bars.item(numBars);
				}
				post("dur_subdivs = " + dur_subdivs + " offsetInBeat = " + offsetInBeat + " posInBar = " + posInBar + " numBars = " + numBars);
			}
			post("dur_subdivs = " + dur_subdivs + " offsetInBeat = " + offsetInBeat + " posInBar = " + posInBar + " numBars = " + numBars);
			
			/////////////////////////
			// for midi output
			/////////////////////////
		    		
			startTime = prevTime + prevDur;
			duration = (int)(theseSubdivs.size() * (1000.f / numSubdivs));
			
			outlet(0, new Atom[]{
							Atom.newAtom("/beat"),
							Atom.newAtom(startTime), 
							Atom.newAtom(71),
							Atom.newAtom(duration)});
			beatStartTimes.add(startTime);
			prevTime = startTime;
			prevDur = duration;
		}
	}
	
	public void dumpSubdivs(){
		int n;
		int startTime = 0;
		int duration = 0;
		int thisStartTime;
		Iterator startTimes = beatStartTimes.iterator();
		int i = 0;
		int theSubdiv, mod, pitch;
		HashMap thisSubdivArray;
		tcfm_event ev;
		while(startTimes.hasNext()){
			thisSubdivArray = (HashMap)(subdivArray.get(i));
			n = thisSubdivArray.size();
			startTime = (Integer)(startTimes.next());
			//duration = (int)(1000.f / numSubdivs);
			for(int j = 0; j < n; j++){
				ev = (tcfm_event)thisSubdivArray.get(j);
				if(ev.getRampDirection() < 0) pitch = 64;
				else if(ev.getRampDirection() > 0) pitch = 77;
				else pitch = 71;
				for(int k = 0; k < subdivsToOutput.length; k++){
					theSubdiv = subdivsToOutput[k];
					mod = numSubdivs / theSubdiv;
					if(j % mod == 0){
						thisStartTime = startTime + (int)((1000.f / theSubdiv) * (j / mod));
						duration = (int)(1000.f / theSubdiv);
						outlet(0, new Atom[]{
									Atom.newAtom("/subdiv/" + theSubdiv),
									Atom.newAtom(thisStartTime), 
									Atom.newAtom(pitch),
									Atom.newAtom(duration)});
						
						if(theSubdiv == numSubdivs)
							subdivTimes_out.add(thisStartTime);
					}
				}
			}
			i++;
		}
	}
	
	public void dumpMarkers(){
		Set markerType = (Set)markerArray.keySet();
		Iterator markerType_it = markerType.iterator();
		String markerType_string;
		HashMap markers;
		Set markerTimes;
		Iterator markerTimes_it;
		tcfm_event ev;
		float time;
		int beatPos, subdivPos;
		while(markerType_it.hasNext()){
			markerType_string = (String)markerType_it.next();
			markers = (HashMap)markerArray.get(markerType_string);
			markerTimes = (Set)markers.keySet();
			markerTimes_it = markerTimes.iterator();
			while(markerTimes_it.hasNext()){
				ev = (tcfm_event)(markers.get((Float)(markerTimes_it.next())));
				time = (float)ev.getStartTime();
				subdivPos = 1 + ((SortedSet)subdivTimes_in.headSet(time)).size();
				post("type = " + markerType_string + " time = " + time + " subdivPos = " + subdivPos);
			}
		}
		//SortedSet subdivTimes_in.headSet(
	}
	
	public void makeTimeMap(){
		Set keys = (Set)beatArray.keySet();
		Iterator it = keys.iterator();
		Iterator it2 = beatStartTimes.iterator();
		int i = 0;
		Float time;
		Integer scoretime;
		tcfm_event ev;
		while(it.hasNext() && it2.hasNext()){
			ev = (tcfm_event)beatArray.get(it.next());
			time = ev.getStartTime();
			scoretime = (Integer)it2.next();
			if(i % 4 == 0){
				outlet(0, new Atom[]{Atom.newAtom("/timemap"), Atom.newAtom("setText"), Atom.newAtom(scoretime), Atom.newAtom(time)});
			}
			++i;
		}
	}

    private void makeMusicXMLScore(){
    	
    }
	
	public void clear(){
		beatArray.clear();
		subdivArray.clear();
		beatStartTimes.clear();
		
		Set keys = (Set)markerArray.keySet();
		Iterator it = keys.iterator();
		while(it.hasNext())
			((HashMap)(markerArray.get((String)(it.next())))).clear();
	}
	
	// Printing 
	public void print_beat_array(){
		int i;
		tcfm_event ev;
		post("size: " + beatArray.size() + "\n");
		for(i = 0; i < beatArray.size(); i++){
			ev = (tcfm_event)beatArray.get(i);
			post(" Beat Index: " + ev.getBeatIndex() + " Start Time: " + ev.getStartTime() + " Current Tempo: " + ev.getCurrentTempo());
		}
	}

	public void print_subdiv_array(){
		tcfm_event ev;
		HashMap hm;
		post("size: " + subdivArray.size() + "\n");
		for(int i = 0; i < subdivArray.size(); i++){
			hm = (HashMap)subdivArray.get(i);
			for(int j = 0; j < hm.size(); j++){
				ev = (tcfm_event)hm.get(j);
				post("Beat Index: " + ev.getBeatIndex() + " Subdiv Index: " + " " + ev.getSubdivIndex() + " Start Time: " + ev.getStartTime()
					+ " Current Tempo: " + ev.getCurrentTempo());
			}
		}
	}
	
	public void print_marker_array(){
		tcfm_event ev;
				
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
				ev = (tcfm_event)markerValues_it.next();
				if(key.compareTo("plan") == 0){
					post(key + " Current Tempo: " + (ev.getPlan()).get("currentTempo") + " Wait Time: " + (ev.getPlan()).get("waitTime") + 
							" Plan Time: " + (ev.getPlan()).get("planTime") + " Target Tempo: " + (ev.getPlan()).get("targetTempo"));
				} else
					post(key + " Time: " + ev.getStartTime() + (ev.getCurrentTempo() == 0 ? "" : " Current tempo: " + ev.getCurrentTempo()));
			}
		}
	}

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
		for(int i = 0; i < subdivsToOutput.length; i++){
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
		for(int i = 0; i < subdivsToOutput.length; i++){
			part = musicxmlscore.createElement("part");
			part.setAttribute("id", "P" + i);
			rootElement.appendChild(part);
		}
	}

	private void setupFirstBarAttributes(){
		NodeList parts = musicxmlscore.getElementsByTagName("part");
		Element part;
		NodeList bars;
		Element firstBar;
		
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
			part = (Element)parts.item(i);
			//bars = part.getElementsByTagName("measure");
			//firstBar = (Element)bars.item(0);
			firstBar = musicxmlscore.createElement("measure");
			firstBar.setAttribute("number", "" + (++numBars));
			firstBar.appendChild(attributes);
			part.appendChild(firstBar);
		}
	}

   	private void addBarToMusicXMLScore(){
	    NodeList parts = musicxmlscore.getElementsByTagName("part");
	    Element part;
	    Element bar;
		for(int i = 0; i < parts.getLength(); i++){
			part = (Element)parts.item(i);
			//bars = part.getElementsByTagName("measure");
			//bar = (Element)bars.item(0);
			bar = musicxmlscore.createElement("measure");
			bar.setAttribute("number", "" + (++numBars));
			part.appendChild(bar);
		}
	}
	
	private String getNoteType(int dur){
		switch(dur){
		}
		return null;
	}
	
	private void writeFile(String path){
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
}