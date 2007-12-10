/*
stop_all_delays
by John MacCallum, 2007

Copyright (c) 2007.  The Regents of the University of California
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
NAME: stop_all_delays
DESCRIPTION: Stops all delays in a patch and it's subpatches.  Java object.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006-07
SVN_REVISION: $LastChangedRevision: 622 $
VERSION 1.0: First version
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

import com.cycling74.max.*;

public class stop_all_delays extends MaxObject{
	MaxPatcher p;

	public stop_all_delays(){
		createInfoOutlet(false);
		declareIO(1, 1);
		setInletAssist(new String[]{"Bang to stop all delays in this and all sub patches"});
		setOutletAssist(new String[]{"Bang when finished."});
		p = getMaxBox().getPatcher();
		post("stop_all_delays version 1.0, by John MacCallum.\nCopyright (c) 2007 Regents of the University of California.  All rights reserved.");
	}

	public void bang(){
		MaxBox[] b = p.getAllBoxes();
		boolean s = lookForDelays(b);
		outletBang(0);
	}

	private boolean lookForDelays(MaxBox[] b){
		boolean returnCode = true;
		for(int i = 0; i < b.length; i++){
			if(b[i].isPatcher()){
				if(!lookForDelays(b[i].getSubPatcher().getAllBoxes())) returnCode = false;
			}
			else if(b[i].getMaxClass().compareTo("del") == 0 || b[i].getMaxClass().compareTo("delay") == 0){
				if(!stopDelay(b[i])){
					error("stop_all_delays: couldn't stop delay " + b[i] + " in patcher " + b[i].getPatcher().getName());
					returnCode = false;
				}
			}
		}
		return returnCode;
	}

	private boolean stopDelay(MaxBox b){
		return b.send("stop", null);
	}
}