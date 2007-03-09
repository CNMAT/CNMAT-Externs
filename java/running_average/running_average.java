/*

running-average
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
NAME: running-average
DESCRIPTION: Computes a running average of it's input.  Java object.
AUTHORS: John MacCallum
COPYRIGHT_YEARS: 2006-7
SVN_REVISION: $LastChangedRevision: 622 $
VERSION 1.0: First versiion
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
*/

import com.cycling74.max.*;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

public class running_average extends MaxObject
{	
	public void version(){
		post("running-average Version 1.0, by John MacCallum.\nCopyright (c) 2006-7 Regents of the University of California.  All rights reserved.");
	}
	
	private int avg_length;
	private ArrayList<Float> avg_array = new ArrayList();
	private float avg_sum = 0;

	public running_average(Atom[] args)
	{
		if(args.length > 0)
			avg_length = args[0].getInt();

		int i;
		
		int inlets[] = new int[1];
		
		String INLET_ASSIST[] = new String[1];
		inlets[0] = DataTypes.ALL;
		INLET_ASSIST[0] = "Input (int/float)";
			
		declareInlets(inlets);
		declareOutlets(new int[]{DataTypes.ALL});
		
		setInletAssist(INLET_ASSIST);
		setOutletAssist(new String[]{"Average output"});
		
		version();

	}	
	
	public void bang()
	{
		output_average();
	}
    
	public void inlet(int i)
	{
		addNewValue((float)i);
	}
    
	public void inlet(float f)
	{
		addNewValue(f);
	}
    
	public void list(float[] l){
		
	}
	
	public void addNewValue(float f){
		float popped;
		if(avg_array.size() == avg_length){
			popped = avg_array.remove(0);
			avg_sum = avg_sum - popped;
		}
		
		avg_array.add(f);
		avg_sum += f;
	}
	
	public void output_average(){
		outlet(0, new Atom[]{Atom.newAtom(avg_sum / avg_array.size())});
	}
	
	public void clear(){
		avg_array.clear();
		avg_sum = 0;
	}
	
	public void length(int l){
		if(l < 1){
			error("running-average: Length must be greater than 0.");
			return;
		}
		clear();
		avg_length = l;
	}
	
	
}