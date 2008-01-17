/*

midifile
by John MacCallum, 2006-07

Copyright (c) 2006-07.  The Regents of the University of California
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
COPYRIGHT_YEARS: 2006-07
SVN_REVISION: $LastChangedRevision: 622 $
VERSION 2.1.1: Added play, play_from, and read messages and implemented open and save dialogs.
VERSION 2.1.2: Fixed a bug where an error would be reported if play was called and there was no data in one of the tracks.
VERSION 2.1.3: Gratuitous increment of the version number.
VERSION 2.1.4: Play now outputs the channel number if there is one.
VERSION 2.2: Now handles meta messages
VERSION 2.2.1: Fixed a bug that caused all tracks of a multi-track file to start together even if one of them had a delay at the beg.
VERSION 2.2.2: Understands text meta events
VERSION 3.0: Totally redesigned to make much better use of javax.sound.midi
VERSION 3.0.1: Sets all note off velocities to 0 by default
VERSION 3.0.2: Now outputs /text message for meta events like markers
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

import com.cycling74.max.*;

import java.io.File;
import java.io.IOException;
import java.util.*;

import javax.sound.midi.*;

public class midifile extends MaxObject implements Receiver, MetaEventListener{
	public void version(){
		post("midifile Version 3.0.1, by John MacCallum.\nCopyright (c) 2006-7 Regents of the University of California.  All rights reserved.");
	}

	private	Sequence sequence = null;
	private	Sequencer sequencer = null;
	private	Transmitter transmitter = null;
	private	File fp = null;

	private Track[] tracks = null;

	private	int originalNumTracks = 1;
	private	int numTracks = 2;

	private int def_duration = 250;
	private int def_channel = 0;
	private int def_velocity = 60;

	private static final int MAIN_OUTLET = 0;
	private static final int DUMP_OUTLET = 1;
	private static final int BANG_OUTLET = 3;
	private static final int INFO_OUTLET = 2;

	private boolean ignoreTempoChanges = false;
	private boolean ignoreNoteOffVelocity = true;

	private boolean verbose = false;

	private int[][] recordState = null;

	public midifile(Atom[] args){
                if(args.length > 0)
                        numTracks = originalNumTracks = args[0].getInt() + 1;

		if(args.length > 1) def_duration = args[1].toInt();
		if(args.length > 2) def_velocity = args[2].toInt();
		if(args.length > 3) def_channel = args[3].toInt();

		recordState = new int[numTracks][16];

                int inlets[] = new int[numTracks - 1];
                String INLET_ASSIST[] = new String [numTracks - 1];
                for(int i = 0; i < numTracks - 1; i++){
                        inlets[i] = DataTypes.ALL;
                        INLET_ASSIST[i] = "Track " + (i + 1) + " midi input";
                }

                declareInlets(inlets);
                declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL, DataTypes.MESSAGE, DataTypes.ALL});
		createInfoOutlet(false);

                setInletAssist(INLET_ASSIST);
                setOutletAssist(new String[]{"Playback", "Dump", "Bang when done playing or dumping", "Information about the midifile"});

		initSequence();

		version();
	}

	public void ignoreTempoChanges(int i){
		if(i == 0) ignoreTempoChanges = false;
		else ignoreTempoChanges = true;
	}

	public void list(Atom[] args){
		if(sequencer.isRunning()){
			error("midifile: can't add notes to sequence while it is being played");
			return;
		}

		addEventToTrack(args, getInlet() + 1);
	}

	public void addEventToTrack(Atom[] args){
		if(args[0].toInt() >= numTracks){
			error("midifile: the sequence only has " + numTracks + ".  Can't add event to track number " + args[0].toInt());
			return;
		}

		Atom[] a = new Atom[args.length - 1];
		for(int i = 0; i < a.length; i++)
			a[i] = args[i + 1];
		addEventToTrack(a, args[0].toInt());
	}

	public void addEventToTrack(Atom[] args, int t){
		int note = args.length > 1 ? args[1].toInt() : 60;
		int velocity = args.length > 2 ? args[2].toInt() : def_velocity;
		int channel = args.length > 3 ? args[3].toInt() : def_channel;
		int duration = args.length > 4 ? args[4].toInt() : def_duration;
		int command = velocity > 0 ? ShortMessage.NOTE_ON : ShortMessage.NOTE_OFF;

		long note_on_time = args[0].toLong() * 1000;
		long note_off_time = note_on_time + (duration * 1000);
		long note_on_tick, note_off_tick;

		if(ignoreTempoChanges){
			note_on_tick = (long)((args[0].toDouble() / 500.) * ((double)sequence.getResolution()));
			note_off_tick = (long)((note_on_tick + (((double)duration / 500.) * ((double)sequence.getResolution()))));
		} else {
			sequencer.setMicrosecondPosition(note_on_time);
			note_on_tick = sequencer.getTickPosition();
			sequencer.setMicrosecondPosition(note_off_time);
			note_off_tick = sequencer.getTickPosition();
		}

		MidiEvent ev = createNoteEvent(command, channel, note, velocity, note_on_tick);
		MidiEvent ev_off = createNoteEvent(ShortMessage.NOTE_OFF, channel, note, 0, note_off_tick);

		//Track[] tracks = sequence.getTracks();
		tracks[t].add(ev);
		tracks[t].add(ev_off);
	}

	private static MidiEvent createNoteEvent(int command, int channel, int key, int velocity, long tick){
		ShortMessage message = new ShortMessage();
		try{
			message.setMessage(command, channel, key, velocity);
		}catch(InvalidMidiDataException e){
			e.printStackTrace();
		}
		return new MidiEvent(message, tick);
	}

	public void setText(Atom[] args){
		int timeStamp = args[0].toInt();
		String st = new String();
		for(int i = 0; i < args.length - 1; i++)
			st = st.concat(args[i + 1].toString() + " ");

		byte[] data;
		try{data = st.getBytes("US-ASCII");}
		catch(Exception e){
			e.printStackTrace();
			error("midifile: couldn't encode text.");
			return;
		}

		addMetaEvent(1, data, timeStamp);
	}

	public void addMetaEvent(Atom[] args){
		byte[] data = new byte[args.length - 2];
		int timeStamp = args[1].toInt();
		for(int i = 0; i < data.length; i++){
			data[i] = args[i + 2].toByte();
		}
		addMetaEvent(args[0].toInt(), data, timeStamp);
	}

	private void addMetaEvent(int type, byte[] data, int timeStamp){
		int length = data.length;
		long tick;
		sequencer.setMicrosecondPosition(timeStamp * 1000);
		tick = sequencer.getTickPosition();
		//Track[] tracks = sequence.getTracks();

		MetaMessage m = new MetaMessage();
		try{m.setMessage(type, data, length);}
		catch(InvalidMidiDataException e){e.printStackTrace();}
		MidiEvent ev = new MidiEvent(m, tick);
		tracks[0].add(ev);
	}

	public void read(){
		String filepath;
		filepath = MaxSystem.openDialog();
		if(filepath == null) return;
		read(filepath);
	}
	
	public void read(String path){
		//Track[] tracks = null;
		tracks = null;
		try{
			fp = new File(path);
			sequence = MidiSystem.getSequence(fp);
			tracks = sequence.getTracks();
			if(tracks.length < numTracks){
				for(int i = 0; i < numTracks - tracks.length; i++)
					sequence.createTrack();
			}
			sequencer.setSequence(sequence);
		} catch (Exception e){
			error("Couldn't read " + path);
			return;
		}

		tracks = sequence.getTracks();
		numTracks = tracks.length;
		if(verbose) postSequenceInfo();
		getInfo();
	}

	public void write(){
		String filepath;
		filepath = MaxSystem.saveAsDialog(new String(""), new String(""));
		if(filepath == null) return;
		
		write(filepath);
	}
	
	public void write(String filepath){
		File outputFile = new File(filepath);
		try{
			MidiSystem.write(sequence, numTracks > 1 ? 1 : 0, outputFile);
		} catch (IOException e) {
			//e.printStackTrace();
			//System.exit(1);
			error("midifile: couldn't create file at " + filepath);
			return;
		}
		post("midifile: successfully wrote file " + filepath);
	}

	public void play(){
		if(sequence == null) return;
		sequencer.start();
	}

	public void play(float startPos){
		if(sequence == null) return;
		sequencer.setMicrosecondPosition(Math.round(startPos * 1000.f));
		sequencer.start();
	}

	public void stop(){
		if(!sequencer.isRunning()) return;
		sequencer.stop();
		sequencer.setTickPosition(0);
	}

	public void pause(){
		if(!sequencer.isRunning()) return;		
		sequencer.stop();
	}

	/*
	public void recordEnable(){
		Track[] tracks = sequence.getTracks();
		for(int i = 0; i < numTracks; i++){
			Track theTrack = tracks[1];
			sequencer.recordEnable(theTrack, -1);
			for(int j = 0; j < 16; j++)
				recordState[i][j] = 1;
		}
	}

	public void recordDisable(){
		sequencer.recordDisable(null);
		for(int i = 0; i < numTracks; i++){
			for(int j = 0; j < 16; j++)
				recordState[i][j] = 0;
		}
	}

	public void recordEnable(int[] t){
		int channel = t[0];
		Track[] tracks = sequence.getTracks();
		Track theTrack = tracks[t[1]];
		if(t[2] == 1){
			sequencer.recordEnable(theTrack, channel);
			recordState[t[1]][channel] = 1;
		}
		else{
			sequencer.recordDisable(theTrack);
			recordState[t[1]][channel] = 0;
			for(int i = 0; i < 16; i++){
				if(recordState[t[1]][i] == 1)
					sequencer.recordEnable(theTrack, i);
			}
		}
		
	}
	*/

	public void dump(){
		stop();

		//Track[] tracks = sequence.getTracks();

		if(tracks == null || tracks.length == 0) return;
		openSequencer();
		for(int i = 0; i < tracks.length; i++){
			for(int j = 0; j < tracks[i].size(); j++){
				MidiEvent ev = tracks[i].get(j);
				MidiMessage m = ev.getMessage();
				Atom[] oscmessage;
				float time;
				sequencer.setTickPosition(ev.getTick());
				time = ((float)sequencer.getMicrosecondPosition()) / 1000.f;

				if(verbose) post("status = " + m.getStatus());
				if(m.getStatus() == 255)
					oscmessage = formatMetaMessage((MetaMessage)m, time);
				else oscmessage = formatMidiMessage(m, time);
				if(oscmessage != null) outlet(DUMP_OUTLET, oscmessage);
			}
		}
		outlet(BANG_OUTLET, "bang");
	}

	private Atom[] formatMidiMessage(MidiMessage m, float timeStamp){
		int length = m.getLength();
		int status = m.getStatus();
		byte[] message = m.getMessage();

		if(status >= 128 && status <= 159){
			// 128-143 = note off, channels 1-16.
			// 144-159 = note on, channels 1-16.
			int note, velocity, channel;
			note = (int)(message[1] & 0xFF);
			velocity = (int)(message[2] & 0xFF);
			if(status <= 143 && ignoreNoteOffVelocity) velocity = 0;
			channel = status <= 143 ? status - 128 : status - 144;

			if(timeStamp == -1)
				return new Atom[]{Atom.newAtom("note"), Atom.newAtom(note), Atom.newAtom(velocity), Atom.newAtom(channel)};
			else
				return new Atom[]{Atom.newAtom("note"), Atom.newAtom(timeStamp), Atom.newAtom(note), Atom.newAtom(velocity), Atom.newAtom(channel)};
		}

		return null;
	}

	private Atom[] formatMetaMessage(MetaMessage m, float timeStamp){
		int type = m.getType();
		byte[] data = m.getData();

		//http://www.sonicspot.com/guide/midifiles.html
		post("type = " + type);
		switch (type) {
			case 0:
				return new Atom[]{Atom.newAtom("/text"), Atom.newAtom(new String(data))};
			case 1:
				return new Atom[]{Atom.newAtom("/text"), Atom.newAtom(new String(data))};
			case 3:
				String st = new String(data);
				post("META EVENT " + st);

				break;
			case 47: // end of track
				outlet(BANG_OUTLET, "bang");
				break;
			case 81: // set tempo
				// returns the tempo in microseconds per minute.
				float ms_per_min = 60000000.f;
				int val = 0;
				
				// convert byte array to an int
				for (int k = data.length - 1,  j = 0; k >= 0; k--, j++)
					val += (data[k] & 0xff) << (8 * j);

				if(timeStamp == -1)
					return new Atom[]{Atom.newAtom("tempo"), Atom.newAtom(ms_per_min / val)};
				else
					return new Atom[]{Atom.newAtom("tempo"), Atom.newAtom(timeStamp), Atom.newAtom(ms_per_min / val)};
			case 84: // SMPTE offset
				// hour, minute, sec, frame, subframe

				post("********************");
				post("timeStamp = " + timeStamp);
				for(int i = 0; i < data.length; i++)
					post(i + " " + data[i]);
				if(timeStamp == -1)
					return new Atom[]{Atom.newAtom("SMPTE_offset"), Atom.newAtom(data[0]), Atom.newAtom(data[1]), Atom.newAtom(data[2]), Atom.newAtom(data[3]), Atom.newAtom(data[4])};
				else
					return new Atom[]{Atom.newAtom("SMPTE_offset"), Atom.newAtom(timeStamp), Atom.newAtom(data[0]), Atom.newAtom(data[1]), Atom.newAtom(data[2]), Atom.newAtom(data[3]), Atom.newAtom(data[4])};
			case 88: // time sig
				// returns an array of 4 bytes corresponding to:
				//	numerator
				//	denominator (this is actually the power to which 2 must be raised->midi blows
				//	metro = specifies how often the metronome should click in terms of the number of clock signals per click, 
				//		which come at a rate of 24 per quarter-note.
				//	number of 32nds per quarter

				if(timeStamp == -1)
					return new Atom[]{Atom.newAtom("time_sig"), Atom.newAtom(data[0]), Atom.newAtom(Math.pow(2, data[1])), Atom.newAtom(data[2]), Atom.newAtom(data[3])};
				else
					return new Atom[]{Atom.newAtom("time_sig"), Atom.newAtom(timeStamp), Atom.newAtom(data[0]), Atom.newAtom(Math.pow(2, data[1])), Atom.newAtom(data[2]), Atom.newAtom(data[3])};
			case 89: // key sig
				// the first byte is the key sig from -7 to 7 (<0 flats, >0 sharps)
				// second byte is 0 = major, 1 = minor

				if(timeStamp == -1)
					return new Atom[]{Atom.newAtom("key_sig"), Atom.newAtom(data[0]), Atom.newAtom(data[1])};
				else
					return new Atom[]{Atom.newAtom("key_sig"), Atom.newAtom(timeStamp), Atom.newAtom(data[0]), Atom.newAtom(data[1])};
				//default:
				//post("found unknown meta event: " + type);
		}
		return null;
	}

	private void openSequencer(){
		if(!sequencer.isOpen()){ 
			try{sequencer.open();}
			catch(MidiUnavailableException e){
				e.printStackTrace();
			}
			try{sequencer.setSequence(sequence);}
			catch(InvalidMidiDataException e){
				e.printStackTrace();
			}
		}
	}

	private void closeSequencer(){
		if(sequencer.isOpen())
			sequencer.close();
	}

	public void clear(){
		stop();
		sequence = null;
		fp = null;
		initSequence();
	}

	private void initSequence(){
		numTracks = originalNumTracks;
		try{sequence = new Sequence(Sequence.PPQ, 500, numTracks);}
		catch(InvalidMidiDataException e){e.printStackTrace();}
		try{
			sequencer = MidiSystem.getSequencer(false);
			transmitter = sequencer.getTransmitter();
			transmitter.setReceiver(this);
		}catch(MidiUnavailableException e){
			e.printStackTrace();
		}
		sequencer.addMetaEventListener(this);
		try{sequencer.setSequence(sequence);}
		catch(InvalidMidiDataException e){e.printStackTrace();}

		openSequencer();
		sequencer.setTempoInBPM(120.f);

		tracks = sequence.getTracks();
	}

	public void postSequenceInfo(){
		post("divisionType = " + sequence.getDivisionType());
		post("resolution = " + sequence.getResolution());
		post("microsecondLength = " + sequence.getMicrosecondLength());
		post("tickLength = " + sequence.getTickLength());
	}

	public void tellmeeverything(){
		//Track[] tracks = sequence.getTracks();

		post("**************************************************");

		post("sequence has " + tracks.length + " track(s)");
		post("");
		for(int i = 0; i < tracks.length; i++)
			post("track " + i + " has " + tracks[i].size() + " events");

		post("");
		post("Sequence info:");
		postSequenceInfo();

		post("");
		post("sequencer is " + (sequencer.isRunning() ? "" : "not") + " running");
		post("sequencer is " + (sequencer.isOpen() ? "open" : "closed"));

		post("**************************************************");
	}

	public void getInfo(){
		if(fp != null){
			try{
				outlet(INFO_OUTLET, new Atom[]{Atom.newAtom("/filepath"), Atom.newAtom(fp.getCanonicalPath())});
			}catch(Exception e){
				e.printStackTrace();
			}
		}else outlet(INFO_OUTLET, new Atom[]{Atom.newAtom("/filepath"), Atom.newAtom("")});
		outlet(INFO_OUTLET, new Atom[]{Atom.newAtom("/duration"), Atom.newAtom(sequencer.getMicrosecondLength() / 1000.f)});

		//Track[] tracks = sequence.getTracks();
		outlet(INFO_OUTLET, new Atom[]{Atom.newAtom("/numTracks"), Atom.newAtom(tracks.length)});

		long loopStart, loopEnd, loopCount;
		sequencer.setTickPosition(sequencer.getLoopStartPoint());
		loopStart = sequencer.getMicrosecondPosition() / 1000;
		sequencer.setTickPosition(sequencer.getLoopEndPoint());
		loopEnd = sequencer.getMicrosecondPosition() / 1000;
		loopCount = sequencer.getLoopCount();
		outlet(INFO_OUTLET, new Atom[]{Atom.newAtom("/loop/start"), Atom.newAtom(loopStart)});
		outlet(INFO_OUTLET, new Atom[]{Atom.newAtom("/loop/end"), Atom.newAtom(loopEnd)});
		outlet(INFO_OUTLET, new Atom[]{Atom.newAtom("/loop/count"), Atom.newAtom(loopCount)});
	}

	public void mute(int t, int s){
		sequencer.setTrackMute(t, (s == 0 ? false : true));
	}

	public void solo(int t, int s){
		sequencer.setTrackSolo(t, (s == 0 ? false : true));
	}

	public void setTempoFactor(float f){
		sequencer.setTempoFactor(f);
	}

	public void setLoop(int start, int end){
		setLoop(start, end, Sequencer.LOOP_CONTINUOUSLY);
	}

	public void setLoop(int start, int end, int n){
		long startTick, endTick;
		sequencer.setMicrosecondPosition((long)(start * 1000));
		sequencer.setLoopStartPoint(sequencer.getTickPosition());
		sequencer.setMicrosecondPosition((long)(end * 1000));
		sequencer.setLoopEndPoint(sequencer.getTickPosition());

		sequencer.setLoopCount(n);
	}

	public void setLoopStart(int start){
		sequencer.setMicrosecondPosition(start * 1000);
		sequencer.setLoopStartPoint(sequencer.getTickPosition());
	}

	public void setLoopEnd(int end){
		sequencer.setMicrosecondPosition(end * 1000);
		sequencer.setLoopEndPoint(sequencer.getTickPosition());
	}

	public void setLoopCount(int n){
		sequencer.setLoopCount(n);
	}

	public void setDefaultDuration(int i){def_duration = i;}
	public void setDefaultChannel(int i){def_channel = i;}
	public void setDefaultVelocity(int i){def_velocity = i;}
	public void verbose(boolean b){verbose = b;}

	public void ignoreNoteOffVelocity(int i){ignoreNoteOffVelocity = i == 0 ? false : true;}

	public void notifyDeleted(){
		sequencer.stop();
		sequencer.close();
		transmitter.close();
	}

	////////////////////////////////////////////////////////////
	// the methods below implement javax.sound.midi.Receiver
	////////////////////////////////////////////////////////////
	public void close(){

	}

	public void send(MidiMessage m, long timeStamp){
		//Atom[] oscmessage = formatMidiMessage(m, timeStamp);
		Atom[] oscmessage = formatMidiMessage(m, -1);
		if(m != null && oscmessage != null)
			outlet(MAIN_OUTLET, oscmessage);
	}

	////////////////////////////////////////////////////////////
	// the methods below implement javax.sound.midi.MetaEventListener
	////////////////////////////////////////////////////////////
	public void meta(MetaMessage m){
		Atom[] oscmessage = formatMetaMessage(m, -1);
		if(m != null && oscmessage != null)
			outlet(MAIN_OUTLET, oscmessage);
	}
}