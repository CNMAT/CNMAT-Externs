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
	private TreeMap<Integer, TreeMap> beatArray = new TreeMap<Integer, TreeMap>();
	private TreeMap<Integer, TreeMap> subdivArray = new TreeMap<Integer, TreeMap>();
	private TreeMap<Float, Integer[]> subdivStartTimes = new TreeMap<Float, Integer[]>();
	private TreeMap<Integer, TreeMap> markerArray = new TreeMap<Integer, TreeMap>();
	private ArrayList<tcfm_markerEvent> markerArray_tmp = new ArrayList<tcfm_markerEvent>();

	Integer inputPos_subdiv = -1;
	Integer inputPos_beat = -1;
	Integer inputPos_bar = -1;

	private int subdivsToOutput[];
	
	// This is the number of subdivs tempocurver puts out by default as /future_subdiv messages.
	private int numSubdivs; 
	private int numBeatsInBar = 4;
	
	private float lastTempo = -1;

	private Document musicXMLScore;
	private Document editedMusicXMLScore;
	private static final String MUSIC_XML_VERSION = "1.1";	

	private int numBars = 0;

	private int numStaves = 2;

	private int beatStaff = 0;
	private int mainSubdivStaff = 1;
	private int markerStaff = 2;
	private int[] subdivStaves = null;

	private ThreadGroup threadGroup = new ThreadGroup("threadGroup");
	private volatile boolean threadShouldContinue = true;

	private int bangOutlet;

	private float defPitch_subdivRampUp = 77.f;
	private float defPitch_subdivHold = 71.f;
	private float defPitch_subdivRampDown = 64.f;

	public void version(){
		post("tc_format_midifile Version 0.0, by John MacCallum.\nCopyright (c) 2006-7 Regents of the University of California.  All rights reserved.");
	}

	public tc_format_midifile(Atom[] args){
		String f = MaxSystem.locateFile("partwise.xsd");
		if(f == null){
			bail("tc_format_midifile: can't find musicxml schema partwise.xsd.  bailing out.");
		}
	
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

		numStaves = subdivsToOutput.length + 3; // subdivisions separate staves for beats and markers.
		subdivStaves = new int[subdivsToOutput.length];
		for(int i = 0; i < subdivsToOutput.length; i++)
			subdivStaves[i] = i + 3;

		createInfoOutlet(false);
		int[] outlets = new int[numStaves + 1];
		bangOutlet = numStaves;
		for(int i = 0; i < numStaves; i++)
			outlets[i] = DataTypes.LIST;

		outlets[bangOutlet] = DataTypes.MESSAGE;
		declareOutlets(outlets);

		String outletAssist[] = new String[numStaves + 1];
		outletAssist[beatStaff] = "beats";
		outletAssist[mainSubdivStaff] = "main subdivision staff";
		outletAssist[markerStaff] = "markers";
		for(int i = 0; i < subdivStaves.length; i++)
			outletAssist[subdivStaves[i]] = "subdiv " + subdivsToOutput[i];
		outletAssist[bangOutlet] = "bang when done processing.";
		setOutletAssist(outletAssist);

		// there is a delay when these classes get instantiated for the first time.
		tcfm_event e = new tcfm_event();
		tcfm_beatEvent b = new tcfm_beatEvent();
		tcfm_subdivEvent s = new tcfm_subdivEvent();
		tcfm_markerEvent m = new tcfm_markerEvent();

		incrementInputPos();

		version();
	}	

	private void incrementInputPos(){
		inputPos_subdiv = (++inputPos_subdiv) % numSubdivs;
		if(inputPos_subdiv == 0){
			inputPos_beat = (++inputPos_beat) % numBeatsInBar;
			if(inputPos_beat == 0){
				inputPos_bar = ++inputPos_bar;
				addBar();
				addBeat();
			} else {
				addBeat();
			}
		}

		//post("inputPos_subdiv = " + inputPos_subdiv + " inputPos_beat = " + inputPos_beat + " inputPos_bar = " + inputPos_bar);
	}

	private void addBar(){
		TreeMap<Integer, TreeMap> b = new TreeMap<Integer, TreeMap>();
		TreeMap<Integer, TreeMap> s = new TreeMap<Integer, TreeMap>();
		TreeMap<Integer, TreeMap> m = new TreeMap<Integer, TreeMap>();

		beatArray.put(inputPos_bar, b);
		subdivArray.put(inputPos_bar, s);
		markerArray.put(inputPos_bar, m);
	}

	private void addBeat(){
		TreeMap<Integer, tcfm_beatEvent> b = new TreeMap<Integer, tcfm_beatEvent>();
		TreeMap<Integer, tcfm_subdivEvent> s = new TreeMap<Integer, tcfm_subdivEvent>();
		TreeMap<Integer, tcfm_markerEvent> m = new TreeMap<Integer, tcfm_markerEvent>();

		for(int i = 0; i < numSubdivs; i++){
			b.put(i, null);
			s.put(i, null);
			m.put(i, null);
		}

		(beatArray.get(inputPos_bar)).put(inputPos_beat, b);
		(subdivArray.get(inputPos_bar)).put(inputPos_beat, s);
		(markerArray.get(inputPos_bar)).put(inputPos_beat, m);
	}

	private TreeMap getBeat(TreeMap<Integer, TreeMap> m, Integer barNum, Integer beatNum){
		TreeMap<Integer, TreeMap> bar = m.get(barNum);
		TreeMap<Integer, TreeMap> beat = bar.get(beatNum);
		return beat;
	}

	public void beat(Atom[] args){
		if(args[0].isString()){
			if((args[0].getString()).compareTo(new String("/done")) == 0)
				return;
		}

		tcfm_beatEvent ev = new tcfm_beatEvent(args);
		ev.setPitch(81);
		TreeMap beat = getBeat(beatArray, inputPos_bar, inputPos_beat);
		beat.put(inputPos_subdiv, ev);
	}
	
	public void subdiv(Atom[] args){
		int index_b = args[0].toInt();
		int index_s = args[1].toInt();
		tcfm_subdivEvent ev = new tcfm_subdivEvent(args);
		int rampDirection;

		if(lastTempo < 0) ev.setRampDirection(0);
		else{
			if(args[3].toFloat() > lastTempo){
				ev.setRampDirection(1);
				ev.setPitch(defPitch_subdivRampUp);
			} else if(args[3].toFloat() < lastTempo){
				ev.setRampDirection(-1);
				ev.setPitch(defPitch_subdivRampDown);
			} else{
				ev.setRampDirection(0);
				ev.setPitch(defPitch_subdivHold);
			}
		}
		lastTempo = args[3].toFloat();

		if(inputPos_subdiv == 0 && inputPos_beat == 0){
			ev.setString("" + ev.getStartTime());
			ev.setHasString(true);
		}
		TreeMap beat = getBeat(subdivArray, inputPos_bar, inputPos_beat);
		beat.put(inputPos_subdiv, ev);
		subdivStartTimes.put(ev.getStartTime(), new Integer[]{inputPos_bar, inputPos_beat, inputPos_subdiv});
		incrementInputPos();
	}

	public void marker(Atom[] args){
		// since the markers come out first, we can't place them in the
		// bars since we don't know the times of anything yet.  So, just
		// put them in an arraylist for now.
		tcfm_markerEvent ev = new tcfm_markerEvent(args);
		ev.setString(ev.getOSCString());
		ev.setHasString(true);
		markerArray_tmp.add(ev);
	}
	
	private ArrayList<Element> formatMusicXMLBeat(TreeMap m){
		boolean[] mask = new boolean[m.size()];
		Iterator it = m.keySet().iterator();
		int pos = 0;
		while(it.hasNext()){
			if(m.get(it.next()) == null)
				mask[pos] = false;
			else mask[pos] = true;
			++pos;
		}

		return formatMusicXMLBeat(m, mask);
	}

	private ArrayList<Element> formatMusicXMLBeat(TreeMap m, boolean[] mask){
		/* l:
		 * 	this is to prevent ties across the middle of a beat.
		 *	for example, if numSubdivs = 8, we don't want an eighth-note
		 *	rest to begin on the 4th 32nd note.
		 */
		int l = 0;
		if(numSubdivs < 3) l = numSubdivs;
		else if(((float)numSubdivs / 3.f) == Math.round((float)numSubdivs / 3.f)) l = 3;
		else if(((numSubdivs / 4) & 1) == 0) l = 4;
		else error("tc_format_midifile: the number of subdivs you have chosen isn't supported.");
		// 5 is a special case that we'll deal with when someone complains that this piece of crap doesn't handle 5's right.
		// who uses 5's anyway...

		ArrayList a = new ArrayList();
		int restLength = 0;
		for(int i = 0; i < numSubdivs / l; i++){
			restLength = 0;
			for(int j = 0; j < l; j++){
				tcfm_event e = (tcfm_event)m.get(j + (i * l));
				//if(e != null){
				if(mask[j + (i * l)]){
					if(restLength > 0){
						if((((j - 1) - restLength) & 1) == 1){
							a.add(makeMusicXMLRest(1));
							--restLength;
						}
						while(restLength >= 3){
							a.add(makeMusicXMLRest(4));
							restLength -= 4;
						}
						if(restLength == 3 || restLength == 2){
							a.add(makeMusicXMLRest(2));
							restLength -= 2;
						}
						if(restLength == 1){
							a.add(makeMusicXMLRest(1));
							restLength -= 1;
						}
					}
					if(e.hasString()){
						Element direction = musicXMLScore.createElement("direction");
						direction.setAttribute("placement", "above");
						Element direction_type = musicXMLScore.createElement("direction-type");
						Element words = musicXMLScore.createElement("words");
						words.appendChild(musicXMLScore.createTextNode(e.getString()));
						direction_type.appendChild(words);
						direction.appendChild(direction_type);
						a.add(direction);
					}
					a.add(makeMusicXMLNote(e.pitch, 1));
				} else {
					++restLength;
				}
			}
			if(restLength > 0){
				if((restLength & 1) == 1){
					a.add(makeMusicXMLRest(1));
					--restLength;
				}
				while(restLength >= 3){
					a.add(makeMusicXMLRest(4));
					restLength -= 4;
				}
				if(restLength == 3 || restLength == 2){
					a.add(makeMusicXMLRest(2));
					restLength -= 2;
				}
				if(restLength == 1){
					a.add(makeMusicXMLRest(1));
					restLength -= 1;
				}
			}
		}
		return a;
	}

	private Element makeMusicXMLNote(float p, int d){
		return makeMusicXMLNote(p, d, null);
	}

	private Element makeMusicXMLNote(float p, int d, String s){
		Element note = musicXMLScore.createElement("note");
		note.appendChild(getPitch(p));
		Element dur = musicXMLScore.createElement("duration");
		dur.appendChild(musicXMLScore.createTextNode("" + d));
		Element type = musicXMLScore.createElement("type");
		type.appendChild(musicXMLScore.createTextNode("" + getDuration(d)[0]));

		note.appendChild(dur);
		note.appendChild(type);

		return note;
	}

	private Element makeMusicXMLRest(int d){
		if(d > 4) error("tc_format_midifile: something bad has happened in makeMusicXMLRest");
		Element note = musicXMLScore.createElement("note");
		note.appendChild(musicXMLScore.createElement("rest"));
		Element dur = musicXMLScore.createElement("duration");
		dur.appendChild(musicXMLScore.createTextNode("" + d));
		Element type = musicXMLScore.createElement("type");
		type.appendChild(musicXMLScore.createTextNode("" + getDuration(d)[0]));

		note.appendChild(dur);
		note.appendChild(type);
		return note;
	}

	public void test(){
		long time = System.currentTimeMillis();
		for(int i = 0; i < 10000; i++){
			outlet(0, new Atom[]{Atom.newAtom(Math.random()), Atom.newAtom(Math.random())});
		}
		post("" + (System.currentTimeMillis() - time));
	}

	public void bang(){
		if(beatArray.size() < 2){
			error("tc_format_midifile: Beat array is empty!");
			return;
		}

		initDocument();
		setupPartList();
		setupParts();
		makeBars();
		setupFirstBarAttributes();
		//write("/Users/johnmac/Development/cnmat/trunk/max/java/tc_format_midifile/foiegras.xml");
	      	makeBeats();
		//write("/Users/johnmac/Development/cnmat/trunk/max/java/tc_format_midifile/boudin.xml");
		makeSubdivs();
		//write("/Users/johnmac/Development/cnmat/trunk/max/java/tc_format_midifile/lapinAvecSauceMoutard.xml");
		makeMainSubdivStaff();
		//write("/Users/johnmac/Development/cnmat/trunk/max/java/tc_format_midifile/porchetta.xml");
		makeMarkers();
		//write("/Users/johnmac/Development/cnmat/trunk/max/java/tc_format_midifile/rognonDeVeau.xml");
		bangWhenThreadsExit(threadGroup);
		dumpToMidifile();
	}

	private void bangWhenThreadsExit(ThreadGroup tg){
		long time = System.currentTimeMillis();
		while(threadGroup.activeCount() > 0){
			if(System.currentTimeMillis() - time > 300000) break;
		}
		if(threadGroup.activeCount() > 0){
			error("tc_format_midifile: threads have been running for more than 5 min and may be hung.  You should restart Max.");
		} else{
			outlet(bangOutlet, "bang");
		}
	}

	public void stop(){
		post("stopping threads...");
		threadShouldContinue = false;
		long time = System.currentTimeMillis();
		while(threadGroup.activeCount() > 0){
			if(System.currentTimeMillis() - time > 15000) break;
		}
		if(threadGroup.activeCount() > 0){
			error("tc_format_midifile: couldn't stop all threads.  You should quit max and restart.");
		} else{
			outlet(bangOutlet, "bang");
			threadShouldContinue = true;
		}
	}

	public void makeBeats(){
		Thread t = new Thread(threadGroup, "makeBeats"){
				public void run(){
					long startTime = System.currentTimeMillis();

					Iterator<Integer> bars = beatArray.keySet().iterator();
					NodeList partList = musicXMLScore.getElementsByTagName("part");
					Element part = (Element)partList.item(beatStaff);
					NodeList barList = part.getElementsByTagName("measure");

					while(bars.hasNext()){
						if(!threadShouldContinue) return;
						Integer currentBar = bars.next();
						TreeMap<Integer, TreeMap> bar = beatArray.get(currentBar);
						Iterator<Integer> beats = bar.keySet().iterator();
						Element barElement = (Element)barList.item(currentBar);
						while(beats.hasNext()){
							if(!threadShouldContinue) return;
							Integer currentBeat = beats.next();
							TreeMap<Integer, tcfm_beatEvent> beat = bar.get(currentBeat);
							ArrayList<Element> a = formatMusicXMLBeat(beat);
							for(int i = 0; i < a.size(); i++){
								if(!threadShouldContinue) return;
								Element el = a.get(i);
								if(el != null){
									barElement.appendChild(el);
								}
								else error("bar = " + currentBar + " i = " + i);
							}
						}
					}
					post("beats done in " + (System.currentTimeMillis() - startTime) + " milliseconds");
				} // run
			}; // thread
		t.start();
	}
	
	public void makeSubdivs(){
		Thread t = new Thread(threadGroup, "makeSubdivs"){
				public void run(){
					long startTime = System.currentTimeMillis();

					for(int i = 0; i < subdivStaves.length; i++){
						if(!threadShouldContinue) return;

						Iterator<Integer> bars = subdivArray.keySet().iterator();
						NodeList partList = musicXMLScore.getElementsByTagName("part");
						Element part = (Element)partList.item(subdivStaves[i]);
						NodeList barList = part.getElementsByTagName("measure");

						while(bars.hasNext()){
							if(!threadShouldContinue) return;

							Integer currentBar = bars.next();
							TreeMap<Integer, TreeMap> bar = subdivArray.get(currentBar);
							Iterator<Integer> beats = bar.keySet().iterator();
							Element barElement = (Element)barList.item(currentBar);


							while(beats.hasNext()){
								if(!threadShouldContinue) return;

								Integer currentBeat = beats.next();
								TreeMap<Integer, tcfm_subdivEvent> beat = bar.get(currentBeat);
								boolean[] mask = new boolean[beat.size()];
								Iterator<Integer> it = beat.keySet().iterator();
								int pos = 0;
								while(it.hasNext()){
									if(!threadShouldContinue) return;

									Integer key = it.next();
									tcfm_subdivEvent ev = beat.get(key);

									if(ev != null){
										int index = ev.getSubdivIndex();
										if(index % (numSubdivs / subdivsToOutput[i]) != 0)
											mask[pos] = false;
										else mask[pos] = true;
									} else mask[pos] = false;
									++pos;
								}

								ArrayList<Element> a = formatMusicXMLBeat(beat, mask);
								for(int j = 0; j < a.size(); j++){
									if(!threadShouldContinue) return;

									Element el = a.get(j);
									if(el != null){
										barElement.appendChild(el);
									}
									else error("bar = " + currentBar + " i = " + j);
								}
							}
						}
					}
					post("subdivs done in " + (System.currentTimeMillis() - startTime) + " milliseconds");
				} // run
			}; // thread
		t.start();
	}

	private void makeMainSubdivStaff(){
		Thread t = new Thread(threadGroup, "makeMainSubdivStaff"){
				public void run(){
					long startTime = System.currentTimeMillis();

					Iterator<Integer> bars = subdivArray.keySet().iterator();
					NodeList partList = musicXMLScore.getElementsByTagName("part");
					Element part = (Element)partList.item(mainSubdivStaff);
					NodeList barList = part.getElementsByTagName("measure");

					int sd = 0;
					while(bars.hasNext()){
						if(!threadShouldContinue) return;

						Integer currentBar = bars.next();
						TreeMap<Integer, TreeMap> bar = subdivArray.get(currentBar);
						Iterator<Integer> beats = bar.keySet().iterator();
						Element barElement = (Element)barList.item(currentBar);

						while(beats.hasNext()){
							if(!threadShouldContinue) return;

							Integer currentBeat = beats.next();
							TreeMap<Integer, tcfm_subdivEvent> beat = bar.get(currentBeat);
							Iterator<Integer> it = beat.keySet().iterator();
							boolean[] mask = new boolean[beat.size()];
							int pos = 0;
							while(it.hasNext()){
								if(!threadShouldContinue) return;

								Integer key = it.next();
								tcfm_subdivEvent ev = beat.get(key);

								if(ev != null){
									if(ev.getSubdivIndex() == 0) sd = getSubdivsForTempo(ev.getCurrentTempo());
									int index = ev.getSubdivIndex();
									if(index % (numSubdivs / sd) != 0)
										mask[pos] = false;
									else mask[pos] = true;
								} else mask[pos] = false;
								++pos;
							}

							ArrayList<Element> a = formatMusicXMLBeat(beat, mask);
							for(int j = 0; j < a.size(); j++){
								if(!threadShouldContinue) return;

								Element el = a.get(j);
								if(el != null)
									barElement.appendChild(el);
								else error("bar = " + currentBar + " i = " + j);
							}
						}
					}
					post("main subdivs done in " + (System.currentTimeMillis() - startTime) + " milliseconds");
				} // run
			}; // thread
		t.start();
	}
	
	private int getSubdivsForTempo(float t){
		int s;
		if(t <= 0.12) s = 32;
		else if(t <= 0.25) s = 16;
		else if(t <= 0.55) s = 8;
		else if(t <= 1.9) s = 4;
		else if(t <= 2.8) s = 2;
		else s = 1;
		if(s > numSubdivs) return numSubdivs;
		else return s;
	}

	private ArrayList findClosestEvent(TreeMap<Float, Integer[]> m, Float key){
		// returns the closest:
		// 	time
		//	corresponding beat (or subdiv) index
		//	object at that key from the TreeMap m.
		ArrayList out = new ArrayList();
		SortedMap<Float, Integer[]> headMap = (m.headMap(key));
		SortedMap<Float, Integer[]> tailMap = (m.tailMap(key));
		if(headMap.size() == 0){
			out.add(tailMap.firstKey());
			out.add(0);
			out.add(m.get(tailMap.firstKey()));
			return out;
		} else if(tailMap.size() == 0){
			out.add(m.lastKey());
			out.add(m.size() - 1);
			out.add(m.get(m.lastKey()));
			return out;
		} else{
			if(Math.abs(headMap.lastKey() - key) < Math.abs(tailMap.firstKey() - key)){
				out.add(headMap.lastKey());
				out.add(headMap.size() - 1);
				out.add(m.get(headMap.lastKey()));
				return out;
			} else {
				out.add(tailMap.firstKey());
				out.add(headMap.size());
				out.add(m.get(tailMap.firstKey()));
				return out;
			}
		}
	}

	public void makeMarkers(){
		Thread t = new Thread(threadGroup, "makeMarkers"){
				public void run(){
					long startTime = System.currentTimeMillis();

					for(int i = 0; i < markerArray_tmp.size(); i++){
						if(!threadShouldContinue) return;

						tcfm_markerEvent m = markerArray_tmp.get(i);
						tcfm_markerEvent m2;
						float time = m.getStartTime();
						ArrayList closestSubdiv = findClosestEvent(subdivStartTimes, time);
						Integer[] indexes = (Integer[])closestSubdiv.get(2);
						TreeMap<Integer, TreeMap> bar = markerArray.get(indexes[0]);
						TreeMap<Integer, tcfm_markerEvent> beat = bar.get(indexes[1]);
						m2 = beat.get(indexes[2]);
						if(m2 == null) beat.put(indexes[2], m);
						else{
							m2.setString(m2.getString() + " " + m.getString());
						}
					}

					Iterator<Integer> bars = markerArray.keySet().iterator();
					NodeList partList = musicXMLScore.getElementsByTagName("part");
					Element part = (Element)partList.item(markerStaff);
					NodeList barList = part.getElementsByTagName("measure");

					while(bars.hasNext()){
						if(!threadShouldContinue) return;

						Integer currentBar = bars.next();
						TreeMap<Integer, TreeMap> bar = markerArray.get(currentBar);
						Iterator<Integer> beats = bar.keySet().iterator();
						Element barElement = (Element)barList.item(currentBar);
						while(beats.hasNext()){
							if(!threadShouldContinue) return;

							Integer currentBeat = beats.next();
							TreeMap<Integer, tcfm_markerEvent> beat = bar.get(currentBeat);
							ArrayList<Element> a = formatMusicXMLBeat(beat);
							for(int i = 0; i < a.size(); i++){
								if(!threadShouldContinue) return;

								Element el = a.get(i);
								if(el != null){
									barElement.appendChild(el);
								}
								else error("bar = " + currentBar + " i = " + i);
							}
						}
					}
					post("markers done in " + (System.currentTimeMillis() - startTime) + " milliseconds");
				} // run
			}; // thread
		t.start();
	}

	private void makeMusicXMLScore(int[][] ar, int p){
		NodeList parts = musicXMLScore.getElementsByTagName("part");
		Element part = (Element)(parts.item(p));
		NodeList bars = part.getElementsByTagName("measure");
		for(int i = 0; i < ar.length; i++){
			Element bar = ((Element)bars.item(i));
			Element prevNote = null;

			for(int j = 0; j < ar[i].length; j++){
				if(ar[i][j] != 0){
					Element note = musicXMLScore.createElement("note");
					if(ar[i][j] < 0){
						note.appendChild(musicXMLScore.createElement("rest"));
					} else if(ar[i][j] > 0){
						note.appendChild(getPitch(71));
					}
					Element dur = musicXMLScore.createElement("duration");
					dur.appendChild(musicXMLScore.createTextNode("" + Math.abs(ar[i][j])));
					Element type = musicXMLScore.createElement("type");
					type.appendChild(musicXMLScore.createTextNode("" + getDuration(Math.abs(ar[i][j]))[0]));
					note.appendChild(dur);
					note.appendChild(type);
					bar.appendChild(note);
				}
			}
		}
	}

	public void clear(){
		musicXMLScore = null;
		beatArray.clear();
		subdivArray.clear();
		subdivStartTimes.clear();
		markerArray.clear();
		markerArray_tmp.clear();
		inputPos_subdiv = inputPos_beat = inputPos_bar = -1;
		lastTempo = -1;
		numBars = 0;

		incrementInputPos();
	}
	
	// Printing 
	public void printArray(TreeMap<Integer, TreeMap> m){
		Iterator<Integer> bars = m.keySet().iterator();
		while(bars.hasNext()){
			Integer barNum = bars.next();
			TreeMap<Integer, TreeMap> bar = m.get(barNum);
			Iterator<Integer> beats = bar.keySet().iterator();
			post("bar " + (barNum + 1) + ":");
			while(beats.hasNext()){
				Integer beatNum = beats.next();
				TreeMap beat = bar.get(beatNum);
				Iterator<Integer> subdivs = beat.keySet().iterator();
				post("	beat " + (beatNum + 1) + ":");
				while(subdivs.hasNext()){
					Integer subdivNum = subdivs.next();
					tcfm_event e = (tcfm_event)beat.get(subdivNum);
					if(e != null)
						post("		" + (subdivNum + 1) + ": " + ((tcfm_event)beat.get(subdivNum)).getOSCString());
					else post("		" + (subdivNum + 1) + ":");
				}
			}
		}
	}

	public void print_beat_array(){
		printArray(beatArray);
	}

	public void print_subdiv_array(){
		printArray(subdivArray);
	}

	public void print_marker_array(){
		printArray(markerArray);
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
			DOMImplementation musicXMLScoreImp = db.getDOMImplementation();
			//DocumentType dt = musicXMLScoreImp.initDocumentType("score-partwise", "-//Recordare//DTD MusicXML 1.1 Partwise//EN", null);
			//musicXMLScore = musicXMLScoreImp.initDocument("http://www.musicxml.org/dtds/partwise.dtd", "score-partwise", dt);
			musicXMLScore = musicXMLScoreImp.createDocument("http://www.musicxml.org/dtds/partwise.dtd", "score-partwise", null);
		}catch(ParserConfigurationException pce) {
			error("Error while trying to instantiate DocumentBuilder\n" + pce);
			return;
		}
		
		// add in the first couple of elements.
		Element rootElement = musicXMLScore.getDocumentElement();
		rootElement.setAttribute("version", MUSIC_XML_VERSION);
		
	}

	private void setupPartList(){
		Element rootElement = musicXMLScore.getDocumentElement();
		Element part_list_element = musicXMLScore.createElement("part-list");
		Element part_id_element;
		Element part_name_element;
		Text part_name;
		// create parts for all of the subdivsToOutput plus the main line.
		for(int i = 0; i < numStaves; i++){
			part_id_element = musicXMLScore.createElement("score-part");
			part_id_element.setAttribute("id", "P" + i);
			
			part_name_element = musicXMLScore.createElement("part-name");
			part_name = musicXMLScore.createTextNode("Part " + i);
			part_name_element.appendChild(part_name);
			part_id_element.appendChild(part_name_element);
			
			part_list_element.appendChild(part_id_element);
		}
		rootElement.appendChild(part_list_element);
	}

	private void setupParts(){
		Element rootElement = musicXMLScore.getDocumentElement();
		Element part;
		for(int i = 0; i < numStaves; i++){
			part = musicXMLScore.createElement("part");
			part.setAttribute("id", "P" + i);
			rootElement.appendChild(part);
		}
	}

	private void setupFirstBarAttributes(){
		NodeList parts = musicXMLScore.getElementsByTagName("part");
		Element attributes = musicXMLScore.createElement("attributes");
		
		// divisions
		// hard coded 32nd notes for now
		Element divisions_element = musicXMLScore.createElement("divisions");
		Text divisions = musicXMLScore.createTextNode("" + 8);
		divisions_element.appendChild(divisions);
		attributes.appendChild(divisions_element);
		
		// time sig
		// hard coding 4/4 for now
		Element time = musicXMLScore.createElement("time");
		Element beats_element = musicXMLScore.createElement("beats");
		Text beats = musicXMLScore.createTextNode("4");
		beats_element.appendChild(beats);
		Element beat_type_element = musicXMLScore.createElement("beat-type");
		Text beat_type = musicXMLScore.createTextNode("4");
		beat_type_element.appendChild(beat_type);
		time.appendChild(beats_element);
		time.appendChild(beat_type_element);
		attributes.appendChild(time);
		
		// clef
		Element clef = musicXMLScore.createElement("clef");
		Element sign_element = musicXMLScore.createElement("sign");
		Text sign = musicXMLScore.createTextNode("G");
		sign_element.appendChild(sign);
		Element line_element = musicXMLScore.createElement("line");
		Text line = musicXMLScore.createTextNode("" + 2);
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
		numBars = beatArray.size();
		for(int i = 0; i < numBars; i++){
			addBarToMusicXMLScore(i + 1);
		}
	}

   	private void addBarToMusicXMLScore(int n){
	    	NodeList parts = musicXMLScore.getElementsByTagName("part");
	    	Element part;
	    	Element bar;

		for(int i = 0; i < parts.getLength(); i++){
			part = (Element)parts.item(i);
			bar = musicXMLScore.createElement("measure");
			bar.setAttribute("number", "" + n);
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
		//Element note = musicXMLScore.createElement("note");
		Element pitch = musicXMLScore.createElement("pitch");
		Element step = musicXMLScore.createElement("step");
		Element accidental = musicXMLScore.createElement("alter");
		Element octave = musicXMLScore.createElement("octave");
			
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

		octave.appendChild(musicXMLScore.createTextNode("" + (int)(((float)Math.floor(midiPitch) / 12.f) - 1.f)));

		while(midiPitch > 11) midiPitch -= 12;

		step.appendChild(musicXMLScore.createTextNode(pitches[(int)midiPitch]));
		accidental.appendChild(musicXMLScore.createTextNode("" + (acc + accidentals[(int)midiPitch])));


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

	public void write(){
		String filepath;
		filepath = MaxSystem.saveAsDialog(new String(""), new String(""));
		if(filepath == null) return;
		write(filepath);
	}

	public void write(String path){
		if(musicXMLScore == null) return;
		try{
			OutputFormat format = new OutputFormat(musicXMLScore);
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
			serializer.serialize(musicXMLScore);
			outputstream.flush();
			outputstream.close();

		} catch(IOException ie) {
			error("musicxml: error writing file.");
			ie.printStackTrace();
			return;
		}
		post("tc_format_midifile: successfully wrote file " + path);
	}
	/*
	public void loadEditedScore(String path){
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		try{
			DocumentBuilder builder = factory.newDocumentBuilder();
			editedMusicXMLScore = builder.parse(new File(path));
		}catch(Exception e){
			e.printStackTrace();
		}
	}

	public void dumpEditedScoreToMidifile(){
		NodeList parts_ed = editedMusicXMLScore.getElementsByTagName("part");
		Element mainSubdivPart_ed = (Element)parts_ed.item(mainSubdivStaff);
		NodeList bars_ed = mainSubdivPart_ed.getElementsByTagName("measure");
		Iterator keys = (subdivArray.keySet()).iterator();

		for(int i = 0; i < bars_ed.getLength(); i++){
			Element bar_ed = (Element)bars_ed.item(i);
			NodeList notes_ed = bar_ed.getElementsByTagName("note");
			HashMap hm = (HashMap)subdivArray.get(keys.next());
			for(int j = 0; j < notes_ed.getLength(); j++){
				Element note_ed = (Element)notes_ed.item(j);
				if(note_ed.getElementsByTagName("rest").getLength() == 0){
					post("hm = " + hm);
					tcfm_subdivEvent ev = (tcfm_subdivEvent)hm.get(j + 1);
					outlet(mainSubdivStaff, new Atom[]{Atom.newAtom(ev.getStartTime()), Atom.newAtom(ev.getPitch()), Atom.newAtom(127), Atom.newAtom(0), Atom.newAtom((1.f / numSubdivs) * 1000.f)});
				}
			}
		}
		dumpBeatsToMidifile();
	}
	*/

	private void outputEvent(int outlet, tcfm_event e){
		outlet(outlet, new Atom[]{Atom.newAtom(e.getStartTime()), Atom.newAtom(e.getPitch())});
		if(e.hasString())
			outlet(outlet, "setText", new Atom[]{Atom.newAtom(e.getStartTime()), Atom.newAtom(e.getString())});
	}

	public void dumpToMidifile(){
		dumpBeatsToMidifile();
		dumpMainSubdivsToMidifile();
		dumpSubdivsToMidifile();
		dumpMarkersToMidifile();
	}

	public void dumpBeatsToMidifile(){
		long time = System.currentTimeMillis();
		Iterator bars = (beatArray.keySet()).iterator();
		while(bars.hasNext()){
			int barNum = (Integer)bars.next();
			TreeMap bar = (TreeMap)beatArray.get(barNum);
			Iterator beats = bar.keySet().iterator();
			while(beats.hasNext()){
				int beatNum = (Integer)beats.next();
				TreeMap beat = (TreeMap)bar.get(beatNum);
				Iterator subdivs = beat.keySet().iterator();
				while(subdivs.hasNext()){
					int subdiv = (Integer)subdivs.next();
					tcfm_beatEvent e = (tcfm_beatEvent)(beat.get(subdiv));
					if(!(e == null))
						outlet(beatStaff, new Atom[]{Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(e.getPitch())});
				}
			}
		}
		post("dumpBeatsToMidifile done in " + (System.currentTimeMillis() - time) + " milliseconds");
	}

	public void dumpMainSubdivsToMidifile(){
		long time = System.currentTimeMillis();
		Iterator<Integer> bars = (subdivArray.keySet()).iterator();
		int sd = 0;
		while(bars.hasNext()){
			Integer barNum = bars.next();
			TreeMap<Integer, TreeMap> bar = subdivArray.get(barNum);
			Iterator<Integer> beats = bar.keySet().iterator();
			while(beats.hasNext()){
				Integer beatNum = beats.next();
				TreeMap<Integer, tcfm_subdivEvent> beat = bar.get(beatNum);
				Iterator<Integer> subdivs = beat.keySet().iterator();
				int pos = 0;
				while(subdivs.hasNext()){
					Integer subdiv = subdivs.next();
					tcfm_subdivEvent ev = (beat.get(subdiv));
					//if(!(e == null))
					//outlet(mainSubdivStaff, new Atom[]{Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(e.getPitch())});
					if(ev != null){
						if(ev.getSubdivIndex() == 0) sd = getSubdivsForTempo(ev.getCurrentTempo());
						int index = ev.getSubdivIndex();
						if(index % (numSubdivs / sd) == 0)
							outlet(mainSubdivStaff, new Atom[]{Atom.newAtom(ev.getStartTime() * 1000.f), Atom.newAtom(ev.getPitch())});
					}
				}
			}
		}
		post("dumpMainSubdivsToMidifile done in " + (System.currentTimeMillis() - time) + " milliseconds");
	}

	public void dumpSubdivsToMidifile(){
		long time = System.currentTimeMillis();
		Iterator bars = (subdivArray.keySet()).iterator();
		while(bars.hasNext()){
			int barNum = (Integer)bars.next();
			TreeMap bar = (TreeMap)subdivArray.get(barNum);
			Iterator beats = bar.keySet().iterator();
			while(beats.hasNext()){
				int beatNum = (Integer)beats.next();
				TreeMap beat = (TreeMap)bar.get(beatNum);
				Iterator subdivs = beat.keySet().iterator();
				while(subdivs.hasNext()){
					int subdiv = (Integer)subdivs.next();
					tcfm_subdivEvent e = (tcfm_subdivEvent)(beat.get(subdiv));
					if(!(e == null)){
						for(int i = 0; i < subdivsToOutput.length; i++){
							if((e.getSubdivIndex() % (numSubdivs / subdivsToOutput[i])) == 0)
								outlet(subdivStaves[i], new Atom[]{Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(e.getPitch())});
						}
					}
				}
			}
		}
		post("dumpSubdivsToMidifile done in " + (System.currentTimeMillis() - time) + " milliseconds");
	}

	public void dumpMarkersToMidifile(){
		long time = System.currentTimeMillis();
		Iterator bars = (markerArray.keySet()).iterator();
		while(bars.hasNext()){
			int barNum = (Integer)bars.next();
			TreeMap bar = (TreeMap)markerArray.get(barNum);
			Iterator beats = bar.keySet().iterator();
			while(beats.hasNext()){
				int beatNum = (Integer)beats.next();
				TreeMap beat = (TreeMap)bar.get(beatNum);
				Iterator subdivs = beat.keySet().iterator();
				while(subdivs.hasNext()){
					int subdiv = (Integer)subdivs.next();
					tcfm_markerEvent e = (tcfm_markerEvent)(beat.get(subdiv));
					if(!(e == null)){
						outlet(markerStaff, new Atom[]{Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(e.getPitch())});
						outlet(markerStaff, "setText", new Atom[]{Atom.newAtom(e.getStartTime() * 1000.f), Atom.newAtom(e.getString())});
					}
				}
			}
		}
		post("dumpMarkersToMidifile done in " + (System.currentTimeMillis() - time) + " milliseconds");
	}

	public void dumpPitches(){
		Iterator bars = (beatArray.keySet()).iterator();
		while(bars.hasNext()){
			int barNum = (Integer)bars.next();
			TreeMap bar = (TreeMap)beatArray.get(barNum);
			Iterator beats = bar.keySet().iterator();
			while(beats.hasNext()){
				int beatNum = (Integer)beats.next();
				TreeMap beat = (TreeMap)bar.get(beatNum);
				Iterator subdivs = beat.keySet().iterator();
				while(subdivs.hasNext()){
					int subdiv = (Integer)subdivs.next();
					tcfm_beatEvent e = (tcfm_beatEvent)(beat.get(subdiv));
					if(!(e == null))
						outlet(beatStaff, "pitch", new Atom[]{Atom.newAtom(barNum), Atom.newAtom(beatNum), Atom.newAtom(subdiv), Atom.newAtom(e.getPitch())});
				}
			}
		}
	}

	public void setPitch(Atom[] args){
		Integer bar = args[0].toInt();
		Integer beat = args[1].toInt();
		Integer subdiv = args[2].toInt();
		Float pitch = args[3].toFloat();

		TreeMap<Integer, TreeMap> beats = beatArray.get(bar);
		TreeMap<Integer, tcfm_beatEvent> subdivs = beats.get(beat);
		tcfm_beatEvent e = subdivs.get(subdiv);
		e.setPitch(pitch);
	}

	protected void notifyDeleted(){
		threadShouldContinue = false;
	}

	class tcfm_event{
		public float currentTempo;
		public float startTime;
		public float pitch = 71;
		public String OSCString = "";
		public String st = "";
		public boolean hasString = false;

		public void setCurrentTempo(float t){currentTempo = t;}
		public float getCurrentTempo(){return currentTempo;}
		public void setStartTime(float t){startTime = t;}
		public float getStartTime(){return startTime;}
		public void setPitch(float p){pitch = p;}
		public float getPitch(){return pitch;}
		public void setOSCString(Atom[] args){
			for(int i = 0; i < args.length; i++)
				OSCString = OSCString.concat(args[i].toString() + " ");
		}
		public String getOSCString(){return OSCString;}
		public void setString(String s){st = s;}
		public String getString(){return st;}
		public void setHasString(boolean b){hasString = b;}
		public boolean hasString(){return hasString;}
	}

	class tcfm_beatEvent extends tcfm_event{
		public int beatIndex;
		public float startTime;
		public int rampDirection;
		public HashMap subdivs = new HashMap();

		public tcfm_beatEvent(Atom[] args){
			beatIndex = args[0].toInt();
		       	startTime = args[1].toFloat();
			currentTempo = args[2].toFloat();
			setOSCString(args);
		}
		public tcfm_beatEvent(){}

		public float getBeatIndex(){ return beatIndex;}
		public void setBeatIndex(int i){beatIndex = i;}

		public float getStartTime(){ return startTime;}
		public void setStartTime(float f){startTime = f;}

		public float getRampDirection(){return rampDirection;}
		public void setRampDirection(int d){rampDirection = d;}

		public void addSubdiv(tcfm_subdivEvent e){
			subdivs.put(e.getSubdivIndex(), e);
		}
		public tcfm_subdivEvent getSubdiv(int i){
			if(!subdivs.containsKey(i)) return null;
			return (tcfm_subdivEvent)subdivs.get(i);
		}
		public HashMap getSubdivs(){
			return subdivs;
		}
		public int getNumSubdivs(){ return subdivs.size();}
	}

	class tcfm_subdivEvent extends tcfm_beatEvent{
		private int subdivIndex;
		public tcfm_subdivEvent(Atom[] args){
			beatIndex = args[0].toInt();
			subdivIndex = args[1].toInt();
			startTime = args[2].toFloat();
			currentTempo = args[3].toFloat();
			setOSCString(args);
		}
		public tcfm_subdivEvent(){}

		public int getSubdivIndex(){return subdivIndex;}
		public void setSubdivIndex(int si){subdivIndex = si;}
	}

	class tcfm_markerEvent extends tcfm_event{
		private float targetTempo;
		private float currentPhase;
		private float targetPhase;
		private float duration;
		private float waitTime;
		private int rampDirection;
		private String markerString;

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
			setOSCString(args);
		}

		public tcfm_markerEvent(){};

		private int calcRampDirection(float ct, float tt){
			if(ct < tt) return 1;
			else if(ct > tt) return -1;
			else return 0;
		}
		public float getTargetTempo(){return targetTempo;}
		public float getCurrentPhase(){return currentPhase;}
		public float getTargetPhase(){return targetPhase;}
		public float getDuration(){return duration;}
		public float getWaitTime(){return waitTime;}
		public int getRampDirection(){return rampDirection;}
		public String getMarkerString(){return markerString;}

	}
}