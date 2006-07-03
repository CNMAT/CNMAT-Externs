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
 
	private int samps_to_target;
	private int samps_to_wait;
	private float target_phase;	// Fractional beat
	private float target_freq;	// Beats/second
	private float current_phase;
	private float current_freq;
    private int interp_mode;	// 1=linear, 2=quadratic
	private float oneoversr;
	private float sr;
	private int mode;			// 0 = hold, 1=wait, 2=ramp
	private boolean verbose;

	private static final String[] INLET_ASSIST = new String[]{
		"List: target tempo (BPS), target phase (0-1), time to get there"
	};
	private static final String[] OUTLET_ASSIST = new String[]{
		"beat phase (sig)",
		"tempo (beats/sec) (sig)",
		"OSC messages"
	};
	

	public tempocurver()
	{
		samps_to_target = 0;
        current_phase = 0;
        current_freq = 2;
        interp_mode = 1;
		mode = 0;
		verbose = true;


		declareInlets(new int[]{DataTypes.ALL});
		declareOutlets(new int[]{SIGNAL,SIGNAL,DataTypes.MESSAGE});
		createInfoOutlet(false);
		setInletAssist(new String[]{
			"List: target tempo (BPS), target phase (0-1), time to get there"});
		setOutletAssist(OUTLET_ASSIST);

	}
 
	public void verbose(int v) {
		verbose = (v != 0);
		post("verbosity " + verbose);
	}

	public void test_casting() {
		float f;
		for (f = -1.f; f <= 2.f; f = f+0.1f) {
			post("f " + f + " to int " + (int) f  + "\n");
		}
	}

	public void list(float target_tempo, float target_phase, float time_to_get_there) {
		// post("you asked for " + target_tempo + " " + target_phase + " " + time_to_get_there);

		while(target_phase>1.0) {
			target_phase -= 1.0;
		}

		if (time_to_get_there == 0) {
			synchronized(this) {
				current_freq = target_tempo;
				current_phase = target_phase;
			}
		} else {
			if (target_tempo == current_freq) {
				outlet(2,"/impossible",
					   new Atom[]{ Atom.newAtom("/same-tempo"), 
								   Atom.newAtom(target_tempo)}); 
				return;
			}
			float target_fracbeat = target_fracbeat_linear(time_to_get_there, target_tempo, target_phase);

			float wait =  how_long_to_wait(target_tempo, time_to_get_there, target_fracbeat);

			if (verbose) {
				post("Target fracbeat is " + target_fracbeat + "\n");
				post("Wait time is " + wait + " seconds\n");
				float wait_increment = wait*current_freq;

				post("So that means " + wait + " seconds at current tempo " + current_freq);
				post("to increment current phase " +  current_phase + " by " +
					 wait_increment + " to " + (current_phase+wait*current_freq));
				post("Then ramp to new tempo " + target_tempo + " over the remaining " + 
					(time_to_get_there - wait) + " seconds");
				post("Thereby bringing phase to " +
						( (current_phase+ wait_increment) +
						  (time_to_get_there - wait) * (current_freq + target_tempo) / 2));
			}

			if (wait < 0.f) {
				// This should never happen
				outlet(2,"/impossible",
					   new Atom[]{ Atom.newAtom("/negative-wait-time"), 
								   Atom.newAtom(wait)}); 
				return;
			}
			if (wait > time_to_get_there) {
				outlet(2,"/impossible",
					   new Atom[]{ Atom.newAtom("/need-more-time"), 
								   Atom.newAtom(wait), 
								   Atom.newAtom(time_to_get_there) });
				return;
			}


			synchronized(this) {
				mode = 1; // Now we're waiting to start ramp
				samps_to_target = (int) (time_to_get_there * sr);
				samps_to_wait = (int) (wait * sr);
				// For now, just pre-compute the wait time and hope it all works out
				// Even better would be to re-check everything dynamically...
				target_phase = -9999; // Not used
				target_freq = target_tempo;
			}
		}
	}

	// A "fracbeat" is a fractional beat number, a sort of unwrapped phase.
	private float target_fracbeat_linear(float t, float end_freq, float end_phase) {
		float phase, freq;
		synchronized(this) {
			phase = current_phase;
			freq = current_freq;
		}

		// Compute the ending fractional beat number based on a steady tempo ramp starting immediately
		// Trivial in the linear case: average freq times elapsed time
		float end_phase_starting_now = phase + t * (freq + end_freq) * 0.5f;
		// Cast to int will round down
		int int_beats = (int) (end_phase_starting_now - end_phase);
		float target = int_beats + end_phase;
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

	private float how_long_to_wait(float end_freq, float t, float target_fracbeat) {
		float phase, freq;
		synchronized(this) {
			phase = current_phase;
			freq = current_freq;
		}

		float avg_freq = (end_freq+freq)*0.5f;
	

		return  (2.f / (end_freq - freq)) * (phase + (avg_freq*t) - target_fracbeat);
		// expr (2.0 / ($f1 - $f4)) * ($f5 + (($f1+$f4)*0.5*$f2) - $f3)
	}



	public void dspsetup(MSPSignal[] ins, MSPSignal[] outs)
	{
		sr = (float) outs[0].sr;
	    oneoversr = (float) 1.0 / sr;
	}

	public void perform(MSPSignal[] ins, MSPSignal[] outs) 
	{
		float[] phaseout = outs[0].vec;
		float[] tempoout = outs[1].vec;
        float p, f, tf, df;
		int i, m, wait;
		int hold_this_sigvec;	// # samples of this signal vector to keep tempo constant
		int nsamps = outs[0].n;

        synchronized(this) {
			// Grab local copies of our state variables
            p = current_phase;
			f = current_freq;
			tf = target_freq;
			m = mode;
			wait = samps_to_wait;
        }
        
		if (m==0) {
			// Holding; keep tempo constant
			hold_this_sigvec = nsamps;
			// df = 0;
		} else if (m == 1) {
			// waiting...
			if (wait > nsamps) {
				// still waiting, so holding for this signal vector
				hold_this_sigvec = nsamps;
				// df = 0;
			} else {
				// This is the signal vector where we start ramping
				hold_this_sigvec = samps_to_wait;
				m = 2;
			}
		} else {
			// Ramping
			hold_this_sigvec = 0;
		}

		for(i = 0; i < hold_this_sigvec; i++) {
			// The constant-tempo case
            phaseout[i] = p;
			tempoout[i] = f;
            p = p + f*oneoversr;
			while (p > 1.0) {
				p -= 1.0;
			}
		}

		if (m == 2) {
			// Ramping;
			int samps_for_entire_ramp = samps_to_target - samps_to_wait;
			df = (tf - f) / ((float) samps_for_entire_ramp);
			int ramp_this_sigvec = java.lang.Math.min(nsamps, samps_to_target);

			for (i = hold_this_sigvec; i < ramp_this_sigvec; ++i) {
				phaseout[i] = p;
				tempoout[i] = f;
   	        	p = p + f*oneoversr;
				f = f + df;
				while (p > 1.0) {
					p -= 1.0;
				}
			}

			if (samps_to_target < nsamps) {
				// We finished the ramp this sigvec
				if (java.lang.Math.abs(f-tf) > 0.0001) {
					post("Error!!  tried to get to freq " + tf + " but got to " + f + " instead...");
				}

				f = tf;

				outlet(2,"/made-it",
					   new Atom[]{ Atom.newAtom(f), 
								   Atom.newAtom(p) });

				for (i = samps_to_target; i < nsamps; ++i) {
					// back to constant-tempo
					phaseout[i] = p;
					tempoout[i] = f;
           			p = p + f*oneoversr;
					while (p > 1.0) {
						p -= 1.0;
					}
				}
				m = 0;	// Now we're holding at a steady tempo until further notice
			}
		}
			
		if (i != nsamps) {
			post("Error!!! Didn't compute the whole signal vector");
		}

        synchronized(this) {
			// Update our state variables with the new values
            current_phase = p;
			current_freq = f;
			if (m == 1) {
				samps_to_wait -= nsamps;
			} else {
				samps_to_wait = 0;
			}
			mode = m;
			samps_to_target -= nsamps;
        }
	}

}






















