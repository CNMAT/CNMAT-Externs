/*
Copyright (c) 2006.
The Regents of the University of California (Regents).
All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes, without
fee and without a signed licensing agreement, is hereby granted, provided that
the above copyright notice, this paragraph and the following two paragraphs
appear in all copies, modifications, and distributions.  Contact The Office of
Technology Licensing, UC Berkeley, 2150 Shattuck Avenue, Suite 510, Berkeley,
CA 94720-1620, (510) 643-7201, for commercial licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley.

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
     ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
     REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

*/

import com.cycling74.max.*;
import com.cycling74.msp.*;


public class tempocurver extends MSPPerformer {
	public void version() {
		post("tempocurver version 3.5 - pretend_perform outputs /future_subdiv");
	}
 
	private double current_phase;
	private double current_freq;
	private float target_phase;	// Fractional beat
	private float target_freq;	// Beats/second
 
	private int samps_to_target;	// For all modes, the time to spend on the current segment
	private int samps_to_wait;      // Time to wait before starting a ramp
	private double freq_slope_per_sample;	// For linear ramps, the slope (beats/sec/sec)

    //        private float ramp_dur;         // Duration of the ramp (seconds)
        private int interp_mode;	// LINEAR or QUADRATIC (some day)
	private static final int LINEAR = 1;
	private static final int QUADRATIC = 2;
	private float oneoversr;
	private float sr;
	private int mode;			// HOLD, WAIT, RAMP, or IDLE
	private boolean verbose;
	private boolean offline;	// If true, then perform() always returns all zeros
	private boolean cheat;		// Jump to exact tempo+phase after a ramp?
	private boolean just_reset;

	private java.util.LinkedList to_do_list;

	private class plan_element {
		int type;		// 0 = hold, 1 = jump, 2 = ramp
		float dur;		// duration, in seconds
		float target_p;	// For ramps, the target phase
		float target_f;	// For ramps, the target frequency
		public plan_element(int t, float d, float f, float p) {
			type = t; dur = d; target_f = f; target_p = p; 
		}
	};	

	// Enum types for mode variable and plan_element's type variable:
	private static final int HOLD = 10;
	private static final int HOLDBEATS = 15;	
	private static final int WAIT = 11;
	private static final int RAMP = 12;
	private static final int IDLE = 13;
	private static final int JUMP = 14;

	private String mode_name(int m) {
		if (m == HOLD) return "HOLD";
		if (m == HOLDBEATS) return "HOLDBEATS";
		if (m == WAIT) return "WAIT";
		if (m == RAMP) return "RAMP";
		if (m == IDLE) return "IDLE";
		if (m == JUMP) return "JUMP";
		return ("mode " + m);
	}

    private int perform_i;    // Used in do_perform and outletOSC
	private boolean pretending = false; // True when we're doing a "pretend_perform"
	private int pretend_subdivisions = 8; // # of subdivisions per beat for pretend_perform
	private float oneover_pretend_subdivisions = 0.125f; // reciprocal of above

	public tempocurver()
	{
		samps_to_target = 0;
        current_phase = 0;
        current_freq = 0;
        interp_mode = LINEAR;
		mode = IDLE;
		verbose = true;
		cheat = false;
		just_reset = false;
		sr = 44100;	// Default in case pretend_perform is called before DSP turned on
		oneoversr = 1/sr;
		to_do_list = new java.util.LinkedList();

		declareInlets(new int[]{DataTypes.ALL});
		declareOutlets(new int[]{SIGNAL,SIGNAL,SIGNAL,DataTypes.MESSAGE});
		createInfoOutlet(false);
		setInletAssist(new String[]{
			"List: target tempo (BPS), target phase (0-1), time to get there"});
		setOutletAssist( new String[]{
			"beat phase (sig)",
			"tempo (beats/sec) (sig)",
			"jumping (0=no, 1=yes) (sig)",
			"OSC messages"
		});
		version();
	}

	// Synchronization mechanism to protect against DSP happening 
	// when only some of the tempocurver objects have received their commands
	private static int num_to_start_together = 0;
	private static int num_ready_to_start = 0;
	private boolean i_am_ready_to_start = false;
 
	public void start_together(int n) {
	    num_to_start_together = n;
		if (verbose) {
			post("No tempocurver will begin its plan unless at least " +
				n + " tempocurver objects have a plan.");
		}
	}



	public void verbose(int v) {
		verbose = (v != 0);
		post("verbosity " + verbose);
	}

	public void offline(int v) {
		offline = (v != 0);
		post("offline " + offline);
	}

	public void cheat(int c) {
		cheat = (c != 0);
		post("cheat " + cheat);
	}

	// Not inherently thread-safe; relying on caller to call as synchronized
	private void todo_push(plan_element e) {
		if (to_do_list.isEmpty() && i_am_ready_to_start == false) {
			synchronized(tempocurver.class) {
				num_ready_to_start++;
				i_am_ready_to_start = true;
			}
		}

		to_do_list.add(e);
	}

	// Not inherently thread-safe; relying on caller to call as synchronized
	private plan_element todo_pop() {
		if (to_do_list.isEmpty()) {
			return null;
		} else {
			java.lang.Object o = to_do_list.remove(0);
			plan_element e = (plan_element) o;	
			return e;
		}
	}

	public void reset(double freq, double phase) {
		clear();
		synchronized(this) {
			current_freq = freq;
			current_phase = phase;
			just_reset = true;
		}
		if (verbose) post("reset to freq " + freq + ", phase " + phase);
 	}

	public void reset() {
		reset(0.,0.);
	}

	public void clear() {
		// as in clear the to-do list
		synchronized(this) {
			to_do_list.clear();
			mode = IDLE;
		}

		if (i_am_ready_to_start) {
			synchronized(tempocurver.class) {
				// No longer ready to start
				num_ready_to_start--;
				i_am_ready_to_start = false;
			}
		}
		if (verbose) post("cleared");

	}

	public void testq() {
		plan_element e;
		synchronized(this) {e = todo_pop();}
		if (e == null) {
			post("can't remove - to-do list is empty");
		} else {
			post("removed " + mode_name(e.type));
		}
	}

	public void testq(int i) {
		synchronized(this) {todo_push(new plan_element(i, 2, 3, 4));}
		post("added " + i);
	}


	private float fix_phase(float p) {
		// In case user asks for a phase outside 0-1 range
		while(p>1.0) {
			p -= 1.0;
		}

		while(p < 0.0) {
			p += 1.0;
		}
		return p;
	}


	public void list(float target_tempo, float target_phase, float time_to_get_there) {
		// post("you asked for " + target_tempo + " " + target_phase + " " + time_to_get_there);

		if (time_to_get_there == 0) {
			jump(target_tempo, target_phase);
		} else {
			ramp(target_tempo, target_phase, time_to_get_there);
		}
	}

	public void jump(float freq, float phase) {
		plan_element e = new plan_element(JUMP, 0, freq, fix_phase(phase));
		synchronized(this) { todo_push(e); }
	}

	public void ramp(float target_tempo, float target_phase, float time_to_get_there) {
		if (time_to_get_there == 0.) {
			jump(target_tempo, target_phase);
		} else {
			plan_element e = new plan_element(RAMP, time_to_get_there, target_tempo, fix_phase(target_phase));
			synchronized(this) { todo_push(e); }
		}
	}

	public void hold(float time) {
		plan_element e = new plan_element(HOLD, time, 0, 0);
		synchronized(this) { todo_push(e); }
	}

	public void hold_beats(float numbeats) {
		plan_element e = new plan_element(HOLDBEATS, numbeats, 0, 0);
		synchronized(this) { todo_push(e); }
	}

	private void do_jump(plan_element e) {
		if (e.type != JUMP) {
			post("Error!!! called do_jump on a plan_element of type " + e.type);
			return;
		}
		
		outletOSC("/jumped",
				   new Atom[]{ Atom.newAtom(e.target_f), 
							    Atom.newAtom(e.target_p)});

		synchronized(this) {
			current_freq = e.target_f;
			current_phase = e.target_p;
		}
	}

	private void plan_ramp(plan_element e) {
		if (e.type != RAMP) {
			post("Error!!! called plan_ramp on a plan_element of type " + e.type);
			return;
		}

		float target_tempo = e.target_f;
		float target_p = e.target_p;
		float time_to_get_there = e.dur;
		float f_float;
		double f;
		double p;

		synchronized(this) {
			f = current_freq;
			p = current_phase;
		}

		f_float = (float) f;
		
		if (target_tempo == f_float) {
			outletOSC("/impossible",
				   new Atom[]{ Atom.newAtom("/same-tempo"), 
							   Atom.newAtom(target_tempo)}); 
			synchronized(this) {mode = IDLE;};
			return;
		}
		double target_fracbeat = target_fracbeat_linear(time_to_get_there, f, p, target_tempo, target_p);
		float wait =  how_long_to_wait(time_to_get_there, f, p, target_tempo, target_fracbeat);


		if (verbose) {
			post("Target fracbeat is " + target_fracbeat + "\n");
			post("Wait time is " + wait + " seconds\n");
			double wait_increment = wait*current_freq;

			post("So that means " + wait + " seconds at current tempo " + current_freq);
			post("to increment current phase " +  current_phase + " by " +
				 wait_increment + " to " + (current_phase+wait*current_freq));
			post("Then ramp to new tempo " + target_tempo + " over the remaining " + 
				(time_to_get_there - wait) + " seconds");
			post("at a slope of " +
				 (target_tempo-current_freq) / (time_to_get_there - wait));
			post("thereby bringing phase to " +
					( (current_phase+ wait_increment) +
					  (time_to_get_there - wait) * (current_freq + target_tempo) / 2));
		}

		if (wait < 0.f) {
			// This should never happen
			outletOSC("/impossible",
				   new Atom[]{ Atom.newAtom("/negative-wait-time"), 
							   Atom.newAtom(wait)}); 
			synchronized(this) {mode = IDLE;};
			return;
		}

		if (wait >= time_to_get_there) {
			outletOSC("/impossible",
				   new Atom[]{ Atom.newAtom("/need-more-time"), 
							   Atom.newAtom(wait), 
							   Atom.newAtom(time_to_get_there) });
			synchronized(this) {mode = IDLE;};
			return;
		}

		/// xxx make it output just the wait time, separate from the plan message
		outletOSC("/plan",
			   new Atom[]{ Atom.newAtom(0.), Atom.newAtom(current_freq),
						   Atom.newAtom(wait), Atom.newAtom(current_freq),
						   Atom.newAtom(time_to_get_there), Atom.newAtom(target_tempo)});

		synchronized(this) {
			mode = WAIT; // Now we're waiting to start ramp
			samps_to_target = (int) ((time_to_get_there-wait) * sr);
			// For now, just pre-compute the wait time and hope it all works out
			// Even better would be to re-check everything dynamically...
			samps_to_wait = (int) (wait * sr);
			//			ramp_dur = (time_to_get_there - wait);
			target_phase = target_p; // Used only to see if we made it correctly @ end
			target_freq = target_tempo;
			freq_slope_per_sample = (target_freq - current_freq) / samps_to_target; 
		}
	}

	// A "fracbeat" is a fractional beat number, a sort of unwrapped phase.
	private double target_fracbeat_linear(float t, double start_freq, double start_phase, 
                                         double end_freq, double end_phase) {

		double end_phase_starting_now = end_phase_of_linear_ramp(start_phase, t, start_freq, end_freq);
		// Cast to int will round down
		int int_beats = (int) (end_phase_starting_now - end_phase);
		double target = int_beats + end_phase;
		if (end_freq>current_freq) {
			// Tempo is increasing, so can end at lower phase by waiting to accelerate
			// So we were right to round down; do nothing
		} else {
			// Tempo is decreasing, so can end at higher phase by waiting to ritard	
			// So should have rounded up instead of down
			target += 1.f;
		}
		return target;
	}

	private double end_phase_of_linear_ramp(double start_phase, float t, double start_freq, double end_freq) {
		// Compute the ending fractional beat number based on a steady tempo ramp starting immediately
		// Trivial in the linear case: average freq times elapsed time	
		return  start_phase + (start_freq + end_freq) * (t * 0.5f);
	}

	public void nowait_phase(double start_freq, double start_phase, float t, double end_freq) {
		float end_phase = (float) end_phase_of_linear_ramp(start_phase, t, start_freq, end_freq);

		outletOSC("/nowait_phase",
				   new Atom[]{ Atom.newAtom(start_freq),
							   Atom.newAtom(start_phase),
							   Atom.newAtom(t),
							   Atom.newAtom(end_freq),
							   Atom.newAtom(end_phase) });
	}

	public void nowait_phase(float t, double end_freq) {
		// If only 2 arguments, it means to start from current phase and frequency
		double start_freq, start_phase;


		synchronized(this) {
			start_phase = current_phase;
			start_freq = current_freq;
		}

		nowait_phase(start_freq, start_phase, t, end_freq);
	}


	private float how_long_to_wait(float t, double start_freq, double start_phase, 
	                               double end_freq,  double target_fracbeat) {

		double avg_freq = (end_freq+start_freq)*0.5f;
		double answer =  (2.f / (end_freq - start_freq)) * (start_phase + (avg_freq*t) - target_fracbeat);
		// expr (2.0 / ($f1 - $f4)) * ($f5 + (($f1+$f4)*0.5*$f2) - $f3)

		return (float) answer;
	}

	public void predict_wait(double start_freq, double start_phase, 
							 float t, double end_freq, double end_phase) {


		post("predict_wait( " + start_freq + ", " + start_phase + ", " + t + ", " + end_freq + ", " + end_phase + ")");

		double end_fracbeat = target_fracbeat_linear(t, start_freq, start_phase, end_freq, end_phase);

		post("  end_fracbeat " + end_fracbeat);

		float wait = how_long_to_wait(t,  start_freq, start_phase, end_freq, end_fracbeat);

		post("  wait " + wait);

		outletOSC("/predict_wait",
				   new Atom[]{ Atom.newAtom(start_freq),
							   Atom.newAtom(start_phase),
							   Atom.newAtom(t),
							   Atom.newAtom(end_freq),
							   Atom.newAtom(end_fracbeat),
							   Atom.newAtom(wait) });
	}


	public void dspsetup(MSPSignal[] ins, MSPSignal[] outs)
	{
		sr = (float) outs[0].sr;
	    oneoversr = (float) 1.0 / sr;
	}

	public Atom[] prependTwoAtoms(Atom a, Atom b, Atom[] cddr) {
        Atom[] toOutput = new Atom[2+cddr.length];
        toOutput[0] = a;
        toOutput[1] = b;
        for (int j = 0; j < cddr.length; ++j) {
            toOutput[j+2] = cddr[j];
        }
  		return toOutput;
	}

    private void outletOSC(java.lang.String message) {
		if (pretending) {
			outlet(2, "/future", new Atom[]{Atom.newAtom(perform_i * oneoversr),
										    Atom.newAtom(message) });
		} else { 
			outlet(2, message);
		}
    }

    private void outletOSC(java.lang.String message, Atom[] args) {
		if (pretending) {
			outlet(2, "/future",
				prependTwoAtoms(Atom.newAtom(perform_i * oneoversr),
								Atom.newAtom(message),
								args ));
		} else { 
			outlet(2, message, args);
		}
    }

	public void future_subdivisions(int n) {
		if (n < 0) n=0;
		pretend_subdivisions = n;
		oneover_pretend_subdivisions = 1.0f/((float) n);

		if (verbose) {
			post("pretend_perform will output " + n + " /future_subdiv messages per beat.");
		}
	}

    public void pretend_perform() {
		// No arguments version means keep going until to-do list is empty

		// So figure out total time of the current to-do list
		
		java.util.Iterator iter = to_do_list.iterator();
		float total_time = 0;
		float current_freq = 0;		

		while (iter.hasNext()) {
			java.lang.Object o = iter.next();
			plan_element e = (plan_element) o;	
			switch (e.type) {
				case HOLDBEATS:
					total_time += e.dur / current_freq;
					break;
				case RAMP: 
					current_freq = e.target_f;
					// no break!
				case HOLD: case WAIT: total_time += e.dur; break;
				case JUMP: 
					/* take zero time */; 
					current_freq = e.target_f;
					break;
				case IDLE: /* take zero time */; break;
				default: post("unrecognized plan_element type: " + mode_name(e.type));
			}
		}
		int nsamps = (int) (total_time * sr);
		post("Total time: " + total_time + ", nsamps " + nsamps);
		pretend_perform(nsamps+1);	// add one so we get to the final beat
	}

	public void pretend_perform(int nsamps) {
		MSPSignal[] ins = new MSPSignal[] {};
		int nbytes = nsamps * 3 * 4;
		post("About to allocate " + nbytes + " bytes for pretend_perform.");
		/// Could catch java.lang.OutOfMemoryError and tell user
		// to find max.java.config.txt and change
		// max.jvm.option -Xmx256m to some higher number
		MSPSignal[] outs = new MSPSignal[] {
			new MSPSignal(new float[nsamps], (double) sr, (int) nsamps, (short) 0),
			new MSPSignal(new float[nsamps], (double) sr, (int) nsamps, (short) 0),
			new MSPSignal(new float[nsamps], (double) sr, (int) nsamps, (short) 0)
		};
		pretending=true;
		do_perform(ins, outs, 0);
		pretending=false;

		float[] phase = outs[0].vec;
		float[] tempo = outs[1].vec;
		float[] jumped = outs[2].vec;
		int beatnum = 0;
		int subdivnum = 0;
		
		if (nsamps >= 2) {
			// Special cases for first beat and first subdivision
			if (phase[0] == 0.0f && phase[1] > 0.0f) {
				outletOSC("/future-beat",
						  new Atom[]{ Atom.newAtom(beatnum),
									  Atom.newAtom(0.0f),
									  Atom.newAtom(tempo[0]) });
				beatnum++;
				subdivnum = 0;
			}
			for (int s = 0; s < pretend_subdivisions; ++s) {
				float subdiv_phase = s * oneover_pretend_subdivisions;
				if (phase[0] == subdiv_phase && phase[1] > subdiv_phase) {
					outletOSC("/future_subdiv", 
							  new Atom[] {Atom.newAtom(beatnum-1),
										  Atom.newAtom(subdivnum),
										  Atom.newAtom(0.f),
										  Atom.newAtom(tempo[0]) });
					subdivnum++;
				}
			}
		}

		for (int i = 1; i<nsamps; ++i) {
		    if ((phase[i] < phase[i-1]) && phase[i] < 0.01) {
				outletOSC("/future-beat",
					   new Atom[]{ Atom.newAtom(beatnum),
						       Atom.newAtom(i*oneoversr),
							   Atom.newAtom(tempo[i]) });
			    beatnum++;
			    if (verbose) {
				    post("beat at time " +i*oneoversr + " (phase " + phase[i-1] + ", " + phase[i] +")");
		    	}

				// Every beat is also subdivision 0
				subdivnum = 0;
				outletOSC("/future_subdiv",
						  new Atom[] {Atom.newAtom(beatnum-1),
									  Atom.newAtom(subdivnum),
									  Atom.newAtom(i*oneoversr),
									  Atom.newAtom(tempo[i]) });
				subdivnum++;
		    }

			if (jumped[i] > 0.) {
				for (int s = 1; s < pretend_subdivisions; ++s) {
					float subdiv_phase = s * oneover_pretend_subdivisions;
					if (phase[i] == subdiv_phase) {
						// Jumped to this subdivision
						outletOSC("/future_subdiv",
								  new Atom[] {Atom.newAtom(beatnum-1),
											  Atom.newAtom(subdivnum),
											  Atom.newAtom(i*oneoversr),
											  Atom.newAtom(tempo[i]) });
						subdivnum++;
					}
				}
			} else {
				// Not a jump; did we cross a subdivision phase?
				for (int s = 1; s < pretend_subdivisions; ++s) {
					float subdiv_phase = s * oneover_pretend_subdivisions;
                    if (phase[i] >= subdiv_phase && phase[i-1] < subdiv_phase) {
                        outletOSC("/future_subdiv",
                                  new Atom[] {Atom.newAtom(beatnum-1),
                                              Atom.newAtom(subdivnum),
                                              Atom.newAtom(i*oneoversr),
                                              Atom.newAtom(tempo[i]) });
						subdivnum++;
					}
				}
			}
		}
	    outletOSC("/future-beat/done");
		
	}



	public void perform(MSPSignal[] ins, MSPSignal[] outs) {
		if (offline) {
			for (int i = 0; i < outs[0].n; ++i) { 
				outs[0].vec[i] = outs[1].vec[i] = outs[2].vec[i] = 0.0f;
			}
		} else {
			// Default is not to be jumping
			for (int i = 0; i < outs[0].n; ++i) {
				outs[2].vec[i] = 0.0f;
			}
			// Unless we just reset
			synchronized(this) {
				if (just_reset) {
					outs[2].vec[0] = 1.0f;
					just_reset = false;
				}
			}
			do_perform(ins, outs, 0);
		}
	}

	private void do_hold(int from, int to, float[] phaseout, float[] tempoout) {
		double f;
		double p;

        synchronized(this) {
			// Grab local copies of our state variables
            p = (float) current_phase;
			f = current_freq;
		}
		for (int i = from; i < to; ++i) {
			// The constant-tempo case
            phaseout[i] = (float) p;
			tempoout[i] = (float) f;
            p = p + f*oneoversr;
			while (p >= 1.0) {
				p -= 1.0;
			}
		}
        synchronized(this) {
			// Update our state variables with the new values
            current_phase = p;
			// current_freq = f;   // freq didn't change; this is the hold case!
		}
		perform_i = to;    // Now the current index for perform is the end of the hold we just did.
	}

	private void do_ramp(int from, int to, float[] phaseout, float[] tempoout) {
		double f, df;
		double p;
		int m;

        synchronized(this) {
			// Grab local copies of our state variables and simple derived values
            p = current_phase;
			f = current_freq;
			m = mode;


			/* Bad old way recomputing the slope each signal vector:

			// The amount to change the frequency on each sample (in the linear case)
			// is simply the remaining frequency difference divided by the remaining
			// number of samples
			// ??? should it be minus one because the freq only changes between  samples...?
			df = (target_freq - f) / (samps_to_target-2);
			*/

			df = freq_slope_per_sample;
		}

		if (m != RAMP) {
			post("error: do_ramp was called not in RAMP mode!");
			do_hold(from, to, phaseout, tempoout);
			return;
		}			


		// post("*** df = " + df + " so slope is " + df * sr);

		// Ramping;

		for (int i = from; i < to; ++i) {
			phaseout[i] = (float) p;
			tempoout[i] = (float) f;
			p = p + f*oneoversr;
			f = f + df;
			while (p > 1.0) {
				p -= 1.0;
			}
		}
        synchronized(this) {
			// Update our state variables with the new values
            current_phase = p;
			current_freq = f;
		}
		perform_i = to;    // Now the current index for perform is the end of the ramp we just did.
	}

	public void do_perform(MSPSignal[] ins, MSPSignal[] outs, int i) {
		/* This procedure is recursive, to handle the cases where we finish a
		   segment and switch to a new mode (possibly more than once) during
		   the current signal vector.  The object's "mode", "stt" and "stw" variables
		   are always assumed to be and kept accurate. 

		   The "perform_i" variable is a global class variable, so that when we're
		   doing pretend_perform and we output an OSC message, the OSCoutlet method
		   knows the "current time" of the OSC message.  */
		   
		float[] phaseout = outs[0].vec;
		float[] tempoout = outs[1].vec;
        double f, tf;
		double p, tp;
		int m, stt, stw;
		int nsamps = outs[0].n;

		// base case / sanity check
		if (i >= nsamps) return;

		perform_i = i;


        synchronized(this) {
			m = mode;
			// post("** In perform, m = mode = " + m + " = " + mode);
        }        
		
		if (m == IDLE) {
			// We were idle (or we just finished a segment); 
			// is there now something else to do?
			
			// First check if we're waiting for a number of instances to start together
			if (!to_do_list.isEmpty() && (num_to_start_together > num_ready_to_start)) {
				if (verbose) {
					post("Not starting; only " + num_ready_to_start + 
						 " instances are ready and you want " + num_to_start_together);
				}
				do_hold(i, nsamps, phaseout, tempoout);
			} else {
				// Pop the next item from the to-do list
				plan_element e;
				synchronized(this) {e = todo_pop();}
				if (e == null) {
					// Nope, still idle
					do_hold(i, nsamps, phaseout, tempoout);
				} else {
					if (verbose) post("switching to mode " + mode_name(e.type));
	 
					if (e.type == HOLD) {
					    outletOSC("/holding",
	 						   new Atom[]{ Atom.newAtom(current_freq),
						                   Atom.newAtom(e.dur)});
						synchronized(this) {
							mode = HOLD;
							samps_to_target = (int) (e.dur * sr);
						}
					} else if (e.type == HOLDBEATS) {
						post("gonna hold for " + e.dur + " beats at tempo " + current_freq);
						synchronized(this) {
							mode = HOLD;
							samps_to_target = (int)  ((e.dur / current_freq) * sr);
						}
					} else if (e.type == JUMP) {
						// Set state variables and output
						do_jump(e);
						// Indicate in the "jumped" signal outlet that a jump happened
						outs[2].vec[i] = 1.0f;
						// Carry on; leave mode as IDLE so next recursion will start next segment
					} else if (e.type == RAMP) {
						plan_ramp(e);
					} else {
						post("error: scheduled segment has unknown mode " + e.type);
						// leave mode as IDLE
					}
					
					// Now that we updated mode and associated state variables, recurse
					do_perform(ins, outs, i);
				}

			}
		} else if (m == HOLD) {
			synchronized(this) {stt = samps_to_target;}

			if (i+stt > nsamps) {
				// Hold for entire signal vector
				do_hold(i, nsamps, phaseout, tempoout);
				synchronized(this) { samps_to_target -= (nsamps - i); };
			} else {
				// Finish HOLD segment
				do_hold(i, i+stt, phaseout, tempoout);
				outletOSC("/finished-hold");
				synchronized(this) { mode = IDLE; };  // so recursive call will pop next segment
				do_perform(ins, outs, i+stt);
			}
		} else if (m == WAIT) {
			synchronized(this) {stw = samps_to_wait;}

			if (i+stw > nsamps) {
				// wait for entire signal vector
				do_hold(i, nsamps, phaseout, tempoout);
				synchronized(this) { samps_to_wait -= (nsamps - i); };
			} else {
				// This is the signal vector where we start ramping
				do_hold(i, i+stw, phaseout, tempoout);

				synchronized(this) { 
					f = current_freq;
					tf = target_freq;
					stt = samps_to_target;
				};
				
				float duration_of_ramp = ((float)stt) * oneoversr;

				outletOSC("/starting-ramp",
					   new Atom[]{ Atom.newAtom((tf-f)/ duration_of_ramp), 
						       Atom.newAtom(f), 
						       Atom.newAtom(tf) });

				// xxx make it also output the duration of the ramp
				synchronized(this) { mode = RAMP; };
				do_perform(ins, outs, i+stw);
			}
		} else if (m == RAMP) {
			synchronized(this) {stt = samps_to_target;}
			
			if (i+stt > nsamps) {
				// ramp for entire signal vector
				do_ramp(i, nsamps, phaseout, tempoout);
				synchronized(this) { samps_to_target -= (nsamps - i); };
			} else {
				// Ramp finishes this signal vector
				do_ramp(i, i+stt, phaseout, tempoout);
				
				synchronized(this) { 
					f = current_freq;
					p = current_phase;
					tf = target_freq;
					tp = target_phase;
				};
				if (java.lang.Math.abs(f-tf) > 0.0005) {
					post("Error!!  tried to get to freq " + tf + " but got to " + f + " instead...");
				}

				double phase_error = java.lang.Math.abs(p-tp);

				// wrap
				if (phase_error > 0.9) phase_error = java.lang.Math.abs(phase_error-1);

				if (phase_error > 0.0005) {
					post("Error!!  tried to get to phase " + tp + " but got to " + p + " instead...");
				}



				outletOSC("/made-it",
					   new Atom[]{ Atom.newAtom(f), 
								   Atom.newAtom(p) });
				synchronized(this) { 
				    mode = IDLE; // so recursive call will pop next segment

					if (cheat) {
						// Now that we got close or printed an error otherwise, we cheat:
						// jump to exact frequency and phase as if everything went correctly
						current_freq = tf;
						current_phase = tp;
					}
				};  
				do_perform(ins, outs, i+stt);
			}
		} else {
			post("error: current mode is unknown: " + m);
		}
			
			
		/* All this should be gone...	
		if (i != nsamps) {
			post("Error!!! Didn't compute the whole signal vector");
			post("i " + i + ", nsamps " + nsamps + ", mode " + mode + ", m " + m);
			post("hold_this_sigvec " + hold_this_sigvec);
		}

        synchronized(this) {
			// Update our state variables with the new values
			if (m == 1) {
				samps_to_wait -= nsamps;
			} else {
				samps_to_wait = 0;
			}
			mode = m;
			samps_to_target -= nsamps;
        }
        
        */
	} // do_perform()
} // class tempocurver

























































































