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
		post("tempocurver version 2.6 - bug fixes");
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




	public tempocurver()
	{
		samps_to_target = 0;
        current_phase = 0;
        current_freq = 0;
        interp_mode = LINEAR;
		mode = IDLE;
		verbose = true;
		sr = 44100;	// Default in case pretend_perform is called before DSP turned on
		oneoversr = 1/sr;
		to_do_list = new java.util.LinkedList();

		declareInlets(new int[]{DataTypes.ALL});
		declareOutlets(new int[]{SIGNAL,SIGNAL,DataTypes.MESSAGE});
		createInfoOutlet(false);
		setInletAssist(new String[]{
			"List: target tempo (BPS), target phase (0-1), time to get there"});
		setOutletAssist( new String[]{
			"beat phase (sig)",
			"tempo (beats/sec) (sig)",
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

	public void reset() {
		clear();
		synchronized(this) {
			current_freq = 0.f;
			current_phase = 0.;
		}
		if (verbose) post("reset");
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

		synchronized(this) {
			current_freq = e.target_f;
			current_phase = e.target_p;
		}
		outlet(2,"/jumped",
				   new Atom[]{ Atom.newAtom(e.target_f), 
							   Atom.newAtom(e.target_p)});
	}

	private void plan_ramp(plan_element e) {
		if (e.type != RAMP) {
			post("Error!!! called plan_ramp on a plan_element of type " + e.type);
			return;
		}

		float target_tempo = e.target_f;
		float target_p = e.target_p;
		float time_to_get_there = e.dur;
		double f;
		double p;

		synchronized(this) {
			f = current_freq;
			p = current_phase;
		}
		
		if (target_tempo == f) {
			outlet(2,"/impossible",
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
			post("At a slope of " +
				 (target_tempo-current_freq) / (time_to_get_there - wait));
			post("Thereby bringing phase to " +
					( (current_phase+ wait_increment) +
					  (time_to_get_there - wait) * (current_freq + target_tempo) / 2));
		}

		if (wait < 0.f) {
			// This should never happen
			outlet(2,"/impossible",
				   new Atom[]{ Atom.newAtom("/negative-wait-time"), 
							   Atom.newAtom(wait)}); 
			synchronized(this) {mode = IDLE;};
			return;
		}

		if (wait >= time_to_get_there) {
			outlet(2,"/impossible",
				   new Atom[]{ Atom.newAtom("/need-more-time"), 
							   Atom.newAtom(wait), 
							   Atom.newAtom(time_to_get_there) });
			synchronized(this) {mode = IDLE;};
			return;
		}

		outlet(2,"/plan",
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
		// Compute the ending fractional beat number based on a steady tempo ramp starting immediately
		// Trivial in the linear case: average freq times elapsed time
		double end_phase_starting_now = start_phase + t * (start_freq + end_freq) * 0.5f;
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


	private float how_long_to_wait(float t, double start_freq, double start_phase, 
	                               double end_freq,  double target_fracbeat) {

		double avg_freq = (end_freq+start_freq)*0.5f;
		double answer =  (2.f / (end_freq - start_freq)) * (start_phase + (avg_freq*t) - target_fracbeat);
		// expr (2.0 / ($f1 - $f4)) * ($f5 + (($f1+$f4)*0.5*$f2) - $f3)

		return (float) answer;
	}



	public void dspsetup(MSPSignal[] ins, MSPSignal[] outs)
	{
		sr = (float) outs[0].sr;
	    oneoversr = (float) 1.0 / sr;
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
		MSPSignal[] outs = new MSPSignal[] {
			new MSPSignal(new float[nsamps], (double) sr, (int) nsamps, (short) 0),
			new MSPSignal(new float[nsamps], (double) sr, (int) nsamps, (short) 0)
		};
		do_perform(ins, outs, 0);

		float[] phase = outs[0].vec;
		int beatnum = 0;
		
		if (nsamps >= 2 && phase[0] == 0.0f && phase[1] > 0.0f) {
			// Special case for first beat
			outlet(2,"/future-beat",
					   new Atom[]{ Atom.newAtom(beatnum),
						       Atom.newAtom(0.0f)});
			beatnum++;
		}						       
		for (int i = 1; i<nsamps; ++i) {
		    if (phase[i] < phase[i-1]) {
				outlet(2,"/future-beat",
					   new Atom[]{ Atom.newAtom(beatnum),
						       Atom.newAtom(i*oneoversr)});
			    beatnum++;
			    if (verbose) {
				    post("beat at time " +i*oneoversr + "(phase " + phase[i-1] + ", " + phase[i]);
		    	}
		    }
		}
	    outlet(2,"/future-beat/done");
		
	}


	public void perform(MSPSignal[] ins, MSPSignal[] outs) {
		if (offline) {
			for (int i = 0; i < outs[0].n; ++i) {
				outs[0].vec[i] = outs[1].vec[i] = 0.0f;
			}
		} else {
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
	}

	public void do_perform(MSPSignal[] ins, MSPSignal[] outs, int i) {
		/* This procedure is recursive, to handle the cases where we finish a
		   segment and switch to a new mode (possibly more than once) during
		   the current signal vector.  The object's "mode", "stt" and "stw" variables
		   are always assumed to be and kept accurate. */
		   
		float[] phaseout = outs[0].vec;
		float[] tempoout = outs[1].vec;
        double f, tf;
		double p, tp;
		int m, stt, stw;
		int nsamps = outs[0].n;

		// base case / sanity check
		if (i >= nsamps) return;

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
					    outlet(2,"/holding",
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
				outlet(2,"/finished-hold");
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

				outlet(2,"/starting-ramp",
					   new Atom[]{ Atom.newAtom((tf-f)/ (((float)stt) * oneoversr)), 
						       Atom.newAtom(f), 
						       Atom.newAtom(tf) });
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
				if (java.lang.Math.abs(f-tf) > 0.005) {
					post("Error!!  tried to get to freq " + tf + " but got to " + f + " instead...");
				}
				// Cheat: jump to exact frequency as if everything went correctly
				f = tf;

				double phase_error = java.lang.Math.abs(p-tp);

				// wrap
				if (phase_error > 0.9) phase_error = java.lang.Math.abs(phase_error-1);

				if (phase_error > 0.005) {
					post("Error!!  tried to get to phase " + tp + " but got to " + p + " instead...");
				}

				outlet(2,"/made-it",
					   new Atom[]{ Atom.newAtom(f), 
								   Atom.newAtom(p) });
				synchronized(this) { 
				    mode = IDLE; // so recursive call will pop next segment
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



















































