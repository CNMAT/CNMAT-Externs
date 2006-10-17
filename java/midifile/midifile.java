/*

midifile
by John MacCallum, 2006

Copyright (c) 2006.  The Regents of the University of California
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
NAME: midifile
DESCRIPTION: Reads / writes / plays type 0 and 1 midi files.  Java object.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006
SVN_REVISION: $LastChangedRevision: 622 $
VERSION 2.1.1: Added play, play_from, and read messages and implemented open and save dialogs.
VERSION 2.1.2: Fixed a bug where an error would be reported if play was called and there was no data in one of the tracks.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

import com.cycling74.max.*;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.sound.midi.*;

public class midifile extends MaxObject
{	
	public void version(){
		post("midifile Version 2.1.2, by John MacCallum.\nCopyright (c) 2006 Regents of the University of California.  All rights reserved.");
	}
	
	private int numVoices;
	private int midifile_type;
	private ArrayList events = new ArrayList();
	private int def_note = 60;
	private int def_vel = 64;
	private float def_dur = 0;
	private int def_chan = 1;
	private boolean record = false;
	private boolean verbose = false;
	private ArrayList clocks = new ArrayList();
	private ArrayList playList = new ArrayList();
	
	class mf_MidiEvent {
		private float time;
		private int note;
		private float dur;
		private int vel;
		private int chan;
		
		public mf_MidiEvent(float t, int p, float d, int v, int c){
			time = t; note = p; dur = d; vel = v; chan = c;
		}
		public float gettime() { return time; }
		public int getnote() { return note; }
		public float getdur() { return dur; }
		public int getvel() { return vel; }
		public int getchan() { return chan; }
	}

	public midifile(Atom[] args)
	{
		numVoices = 1;
		midifile_type = 0;
		
		if(args.length > 0)
			numVoices = args[0].getInt();

		if(numVoices > 32){
			bail("midifile: Too many tracks, dude -- 32 is the max.");
			return;
		}
		int i;
		
		for(i = 0; i < numVoices; i++){
			events.add(new ArrayList());
		}
		
		int inlets[] = new int[numVoices];
		
		String INLET_ASSIST[] = new String [numVoices];
		for(i = 0; i < numVoices; i++){
			inlets[i] = DataTypes.ALL;
			INLET_ASSIST[i] = "Track " + i + " midi input";
		}
		if(numVoices > 1) midifile_type = 1;
			
		declareInlets(inlets);
		declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL});
		
		setInletAssist(INLET_ASSIST);
		setOutletAssist(new String[]{"Playback (note, vel, dur)", "Dump outlet"});
		
		// There's a short delay the first time a mf_MidiEvent gets instantiated, 
		// so do it here once at initialization.
		mf_MidiEvent me = new mf_MidiEvent(0, 0, 0, 0, 0);

		version();

	}	
	
	public void bang()
	{
	}
    
	public void inlet(int i)
	{
		list(new float[]{(float)i});
	}
    
	public void inlet(float f)
	{
		list(new float[]{f});
	}
    
    
	public void list(float[] l){
		if(!record) return;
		
		int inlet = getInlet();
		int i;
		
		if(events.size() != numVoices){
			events.clear();
			for(i = 0; i < numVoices; i++){
				events.add(new ArrayList());
			}
		}
		
		mf_MidiEvent me = new mf_MidiEvent(l[0], 
							((l.length > 1) ? (int)l[1] : def_note), 
							((l.length > 2) ? (int)l[2] : def_dur), 
							((l.length > 3) ? (int)l[3] : def_vel), 
							((l.length > 4) ? (int)l[4] : def_chan));
		
		((ArrayList)events.get(inlet)).add(me);

		if(verbose){
			post("event number " + ((ArrayList)events.get(inlet)).size() + " : " + me.gettime() + " " + 
				 me.getnote() + " " + me.getdur() + " " + me.getvel() + " " + me.getchan());
		}
	}
	
	public void read(){
		String filepath;
		filepath = MaxSystem.openDialog();
		if(filepath == null) return;
		
		read(filepath);
	}
	
	public void read(String path){
		File file = new File(path);
		MidiFileFormat mf = null;
		Sequence seq = null;

		try{
			mf = MidiSystem.getMidiFileFormat(file);
		} catch (InvalidMidiDataException e){
			error("Couldn't read " + path);
			return;
		} catch (IOException e){
			error("Couldn't read " + path);
			return;
		}
		
		try{
			seq = MidiSystem.getSequence(file);
		} catch (InvalidMidiDataException e){
			error("Couldn't read " + path);
			return;
		} catch (IOException e){
			error("Couldn't read " + path);
			return;
		}
		
		float framesPerSecond = 0;
		if(seq.getDivisionType() == Sequence.PPQ){
			framesPerSecond = 1.f;
			if(verbose) post("Division type: PPQ");
		}
		else if(seq.getDivisionType() == Sequence.SMPTE_24){
			framesPerSecond = 24.f;
			if(verbose) post("Division type: SMPTE_24");
		}
		else if(seq.getDivisionType() == Sequence.SMPTE_25){
			framesPerSecond = 25.f;
			if(verbose) post("Division type: SMPTE_25");
		}
		else if(seq.getDivisionType() == Sequence.SMPTE_30){
			framesPerSecond = 30.f;
			if(verbose) post("Division type: SMPTE_30");
		}
		else if(seq.getDivisionType() == Sequence.SMPTE_30DROP){
			framesPerSecond = 29.97f;
			if(verbose) post("Division type: SMPTE_30DROP");
		}
		else error("Couldn't get division type");
		
		float ticksPerFrame = (float)seq.getResolution();
		if(verbose) post("Resolution: " + ticksPerFrame);
		
		float tickLength = ((1 / framesPerSecond) / ticksPerFrame);
		if(verbose) post("Tick length: " + tickLength);
		
		Track[] tracks = seq.getTracks();
		
		int i = 0;
		int j = 0;
		int k = 0;
		MidiEvent event = null;
		MidiMessage message = null;
		long time, note, dur, vel, chan;
		
		if(events.size() > 0)
			events.clear();
			
		for(i = 0; i < tracks.length; i++){
			events.add(new ArrayList());
			for(j = 0; j < tracks[i].size(); j++){
				time = note = vel = chan = 0;
				
				try{
					event = tracks[i].get(j);
				} catch(ArrayIndexOutOfBoundsException e) {
					error("Ran out of room");
					return;
				}
				message = event.getMessage();
				// 128-143 = chan 1-16 note on.
				// 144-159 = chan 1-16 note off.
				if(message.getStatus() <= 159 && message.getStatus() >= 128){
					byte[] midibytes = message.getMessage();
					
					time = (long)((event.getTick() * tickLength) * 1000);
					if(message.getLength() > 1) note = (int)(midibytes[1] & 0xFF);
					if(message.getLength() > 2) vel = (int)(midibytes[2] & 0xFF);
					if(message.getStatus() >= 128 && message.getStatus() <= 143)
						chan = message.getStatus() - 127;
					else chan = message.getStatus() - 143;
					
					((ArrayList)events.get(i)).add(new mf_MidiEvent((float)time, (int)note, 0, (int)vel, (int)chan));
					if(verbose) post("track: " + i + " event: " + j + " time: " + time + " note: " + (int)note + " vel: " + (int)vel + " chan " + (int)chan);
				}
			}
		}
		
		//post(mf.getByteLength() + " " + mf.getDivisionType() + " " + mf.getMicrosecondLength() + " " + mf. getResolution() + " " + mf.getType());
		//post("size: " + tracks[0].size());
	}
	
	public void write_separate_files(){
		String filepath;
		filepath = MaxSystem.saveAsDialog(new String("Screw you."), new String(""));
		
		write_separate_files(filepath);
	}
	
	public void write_separate_files(String path){
		if(path.length() == 0){
			error("midifile: Error: You must specify an output file path.");
			return;
		}
		
		int i, j;
		for(i = 0; i < numVoices; i++){
			if(!((ArrayList)events.get(i)).isEmpty()){
				String filepath = path + "track" + i + ".mid";
				
				Sequence sequence = null;
				try{
					sequence = new Sequence(Sequence.PPQ, 1000);
				} catch (InvalidMidiDataException e){
					error("Couldn't create sequence");
					return;
				}
				Track track[] = new Track[1];
				
				track[0] = sequence.createTrack();
				for(j = 0; j < ((ArrayList)events.get(i)).size(); j++){
					addEventToTrack((mf_MidiEvent)((ArrayList)events.get(i)).get(j), track[0]);
				}
				if(writeFile(sequence, 0, filepath) == 1)
					error("file wasn't written!");
			}
		}
	}
	
	public void write(String path){
		if(path.length() == 0){
			error("midifile: Error: You must specify an output file path.");
			return;
		}
		
		File outputFile = new File(path);
		Sequence sequence = null;
		try{
			sequence = new Sequence(Sequence.PPQ, 1000);
		} catch (InvalidMidiDataException e){
			error("Couldn't create sequence");
			return;
		}
		
		int i, j;

		int numVoicesRecorded = 0;

		Track tracks[] = new Track[numVoices];
		
		// Only create tracks for voices that actually contain data.
		for(i = 0; i < numVoices; i++){
			if(!((ArrayList)events.get(i)).isEmpty()){
				tracks[i] = sequence.createTrack();
				++numVoicesRecorded;
			}
		}
		
		if(numVoicesRecorded == 0){
			error("No events were recorded--bailing out of writing file");
			return;
		}
		
		// If we're going to write a type 1 file, make sure there is data
		// in more than one track.
		if(numVoicesRecorded == 1)
			midifile_type = 0;
		if(numVoicesRecorded > 1)
			midifile_type = 1;

		for(j = 0; j < numVoices; j++){
			for(i = 0; i < ((ArrayList)events.get(j)).size(); i++){
				addEventToTrack((mf_MidiEvent)((ArrayList)events.get(j)).get(i), tracks[j]);
			}
		}
		if(writeFile(sequence, midifile_type, path) == 1)
			error("file wasn't written!");
	}
	
	public void write(){
		String filepath;
		filepath = MaxSystem.saveAsDialog(new String("Screw you."), new String(""));
		if(filepath == null) return;
		
		write(filepath);
	}
	
	private int writeFile(Sequence seq, int type, String filepath){
		File outputFile = new File(filepath);
		try{
			MidiSystem.write(seq, type, outputFile);
		} catch (IOException e) {
			//e.printStackTrace();
			//System.exit(1);
			error("midifile: couldn't create file at " + filepath);
			return 1;
		}
		return 0;
	}
	
	public void play(){
		int i, j;
		
		stop();
		makePlayList();
		
		for(i = 0; i < events.size(); i++)
			clocks.add(i, new MaxClock(new Callback(this, "play_callback", i)));
		
		for(i = 0; i < events.size(); i++){
			if(((ArrayList)events.get(i)).size() != 0)
				((MaxClock)clocks.get(i)).delay(0);
		}
		
		//post("number of events: " + ((ArrayList)events.get(0)).size());
		
	}
	
	private void makePlayList(){
		int i, j, k;
		mf_MidiEvent thisME, lastME;
		int[] times;
		
		if(verbose) post("Making playlistÉ");
		for(i = 0; i < events.size(); i++){
			playList.add(new ArrayList());
			if(((ArrayList)events.get(i)).size() > 0){
				//thisME = ((mf_MidiEvent)((ArrayList)events.get(i)).get(0));
				//((ArrayList)playList.get(i)).add(thisME);
				for(j = 0; j < ((ArrayList)events.get(i)).size(); j++){
					thisME = ((mf_MidiEvent)((ArrayList)events.get(i)).get(j));

					for(k = 0; k < ((ArrayList)playList.get(i)).size(); k++){
						if(thisME.gettime() < ((mf_MidiEvent)((ArrayList)playList.get(i)).get(k)).gettime())
							break;
					}
					if(verbose) post(k + " " + thisME.gettime() + " " + thisME.getnote() + " " + thisME.getvel());
					((ArrayList)playList.get(i)).add(k, thisME);
					/*
					for(k = 0; k < ((ArrayList)playList.get(i)).size(); k++){
						if(thisME.gettime() + thisME.getdur() < ((mf_MidiEvent)((ArrayList)playList.get(i)).get(k)).gettime())
							break;
					}				
					((ArrayList)playList.get(i)).add(k, new mf_MidiEvent(thisME.gettime() + thisME.getdur(), thisME.getnote(), 0, 0, thisME.getchan()));
					*/
					/*
					for(k = 0; k < ((ArrayList)playList.get(i)).size(); k++){
						post("time: " + ((mf_MidiEvent)((ArrayList)playList.get(i)).get(k)).gettime());
					}
					*/
				}
			}
		}
	}
	
	private void play_callback(int v){
		MaxClock cl = (MaxClock)clocks.get(v);
		mf_MidiEvent thisME = ((mf_MidiEvent)((ArrayList)playList.get(v)).get(0));
		
		((ArrayList)playList.get(v)).remove(0);
		outlet(0, new Atom[]{Atom.newAtom("/track" + v), Atom.newAtom(thisME.getnote()), Atom.newAtom(thisME.getvel()), (thisME.getdur() > 0) ? Atom.newAtom(thisME.getdur()) : Atom.newAtom("")});
		
		if(((ArrayList)playList.get(v)).size() == 0){
			cl.unset();
			outlet(0, new Atom[]{Atom.newAtom("/track" + v), Atom.newAtom("bang")});
		}
		else cl.delay(((mf_MidiEvent)((ArrayList)playList.get(v)).get(0)).gettime() - thisME.gettime());
		//cl.delay(thisME.getdur());
	}
	
	public void play_from(float start){
		int i, j;
		stop();
		makePlayList();
		
		for(i = 0; i < playList.size(); i++)
			clocks.add(i, new MaxClock(new Callback(this, "play_callback", i)));
		
		for(i = 0; i < playList.size(); i++){
			for(j = 0; j < ((ArrayList)playList.get(i)).size(); j++){
				if(start > ((mf_MidiEvent)((ArrayList)playList.get(i)).get(0)).gettime())
					((ArrayList)playList.get(i)).remove(0);
				else break;
			}
			MaxClock cl = (MaxClock)clocks.get(i);
			cl.delay(((mf_MidiEvent)((ArrayList)playList.get(i)).get(0)).gettime() - start);
		}
	}
	
	public void stop(){ 
		int i, j;
		for(i = 0; i < clocks.size(); i++){
			MaxClock c = (MaxClock)clocks.get(i);
			c.unset();
		}
		//for(i = 0; i < clocks.size(); i++)
		//	clocks.remove(i);
			
		for(i = 0; i < events.size(); i++){
			for(j = 0; j < 128; j++)
				outlet(0, new Atom[]{Atom.newAtom("/track" + i), Atom.newAtom(j), Atom.newAtom(0)});
		}
		
		clocks.clear();
		playList.clear();
	} 
	
	public void notifyDeleted(){ 
		int i;
		for(i = 0; i < clocks.size(); i++){
			MaxClock c = (MaxClock)clocks.get(i);
			c.release();
		}
	}
	
	private static void addEventToTrack(mf_MidiEvent me, Track track){
		int note, time, dur, vel, chan;
				
		time = java.lang.Math.round(me.gettime());
		note = me.getnote();
		dur = java.lang.Math.round(me.getdur());
		vel = me.getvel();
		chan = me.getchan();
		
		if(note > 127){ 
			note = 127;
			error("note value out of range -- setting to 127");
		}
		if(vel > 127){ 
			vel = 127;
			error("velocity value out of range -- setting to 127");
		}
		if(chan > 16){ 
			chan = 16;
			error("channel value out of range -- setting to 16");
		}
		
		//track.add(createNoteOnEvent(note, time, vel));
		//track.add(createNoteOffEvent(note, time + dur));
		
		track.add(createNoteEvent((vel != 0) ? ShortMessage.NOTE_ON : ShortMessage.NOTE_OFF, note, vel, time));
		if(dur != 0) track.add(createNoteEvent(ShortMessage.NOTE_OFF, note, 0, time + dur));
	}
	
	/*
	private static MidiEvent createNoteOnEvent(int nKey, long lTick, int vel){
		return createNoteEvent(ShortMessage.NOTE_ON, nKey, vel, lTick);
	}

	private static MidiEvent createNoteOffEvent(int nKey, long lTick){
		return createNoteEvent(ShortMessage.NOTE_OFF, nKey, 0, lTick);
	}
	*/
	
	private static MidiEvent createNoteEvent(int nCommand, int nKey, int nVelocity, long lTick){
		ShortMessage message = new ShortMessage();
		try{
			message.setMessage(nCommand, 0, nKey, nVelocity);
		}catch (InvalidMidiDataException e){
			error("createNoteEvent couldn't creat an event");
			//e.printStackTrace();
			//System.exit(1);
		}
		MidiEvent	event = new MidiEvent(message, lTick);
		return event;
	}

	private static void out(String strMessage){
		System.out.println(strMessage);
	}

	public void clear(){
		int i;

		for(i = 0; i < numVoices; i++){
			((ArrayList)events.get(i)).clear();
		}

		for(i = 0; i < playList.size(); i++)
			((ArrayList)playList.get(i)).clear();
	}

	public void record(int r){
		record = (r != 0);
		post((record) ? "recordingÉ" : "recording is off.");
		if(record) clear();	
	}

	public void verbose(int v){
		verbose = (v != 0);
	}
	
	public void tellmeeverything(){
		post("verbosity is " + ((verbose) ? "on" : "off"));
		post("recording is " + ((record) ? "on" : "off"));
		int i;
		for(i = 0; i < numVoices; i++){
			post("voice " + i + " has " + ((ArrayList)events.get(i)).size() + " events ");
		}
	}
				
	public void dump(){
		int i, j;
		mf_MidiEvent me;
		
		for(i = 0; i < numVoices; i++){
			for(j = 0; j < ((ArrayList)events.get(i)).size(); j++){
				me = (mf_MidiEvent)((ArrayList)events.get(i)).get(j);
			
				float time = me.gettime();
				int note = me.getnote();
				float dur = me.getdur();
				int vel = me.getvel();
				int chan = me.getchan();
				
				outlet(1, new Atom[]{Atom.newAtom("/track" + i), Atom.newAtom(time), Atom.newAtom(note), Atom.newAtom(dur), 
						Atom.newAtom(vel), Atom.newAtom(chan)});
			}
		}
		
	}
	
	public void setnote(int n){def_note = n;}
	public void setdur(int n){def_dur = n;}
	public void setvel(int n){def_vel = n;}
	public void setchan(int n){def_chan = n;}
}