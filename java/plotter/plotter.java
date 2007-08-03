import com.cycling74.max.*;
import com.cycling74.jitter.*;

import java.io.*;
import java.util.*;
import java.net.*;

public class plotter extends MaxObject implements JitterNotifiable
{	
	public void version(){
		post("plotter Version 1.0, by John MacCallum.\nCopyright (c) 2007 Regents of the University of California.  All rights reserved.");
	}

	JitterListener listener;
	JitterObject sk;
	JitterObject r;
	JitterMatrix jm = null;
	JitterMatrix gl_jm = null;
	JitterObject handle;
	//Map datamap = Collections.synchronizedMap(new HashMap());
	List xdata = Collections.synchronizedList(new ArrayList());
	List ydata = Collections.synchronizedList(new ArrayList());
	List zdata = Collections.synchronizedList(new ArrayList());
	
	List xdataBuffer = Collections.synchronizedList(new ArrayList());
	List ydataBuffer = Collections.synchronizedList(new ArrayList());
	List zdataBuffer = Collections.synchronizedList(new ArrayList());
	
	float wx, wy, aspect;
	boolean shouldResetMinMax = true;
	boolean shouldProcess = false;
	boolean shouldDrawCrosshairs = false;
	boolean shouldMakeDisplayBuffer = true;
	//float[] mouse = new float[]{0, 0};
	int currentXPos = 0;
	int mouseX = 0;
	int activePlot = 0;
	int numDims = 0;
	int maxNumDims = 3;
	float[] min = new float[maxNumDims];
	float[] max = new float[maxNumDims];
	float min_disp, max_disp, miny_disp, maxy_disp;
	ArrayList colors = new ArrayList();
	String viewname;
	int[] dim = new int[]{256, 256};
	float defaultZ = -.5f; // this is the z argument for world coordinates.
	String xyz_colorscheme = "rainbow1";
	boolean sonogramStyle = false;
	int major_grid = 0;
	int minor_grid = 0;
	int max_major_grid = 10;
	int max_minor_grid = 10;
	float[] selectionBox_world = new float[]{-1, -1, -1, -1}; // x, y, w, h (xy = lower left corner)
	float[] selectionBox_screen = new float[]{-1, -1, -1, -1}; // x, y, w, h (xy = lower left corner)
	
	int mouseDown = 0;
	
	public plotter(Atom[] args){
		declareInlets(new int[]{DataTypes.ALL, DataTypes.ALL, DataTypes.ALL});	
		declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL, DataTypes.ALL});	
		
		String view;
		if(args.length == 0) viewname = "";
		else viewname = args[0].getString();
		
		if(args.length > 1)
			dim[0] = args[1].toInt();
		if(args.length > 2)
			dim[1] = args[2].toInt();
		else dim[1] = dim[0];
		
		gl_jm = new JitterMatrix(4, "char", dim[0], dim[1]);
		sk = new JitterObject("jit.gl.sketch", gl_jm.getName());
		//sk.setAttr("ortho", 2);
		r = new JitterObject("jit.gl.render", gl_jm.getName()); 
		listener = new JitterListener(viewname, this);
		//handle = new JitterObject("jit.gl.handle", gl_jm.getName());
		
		for(int i = 0; i < maxNumDims; i++)
			min[i] = max[i] = 0;
		min_disp = min[0];
		max_disp = max[0];
		
		sk.call("reset");
		
		sk.call("glclearcolor", new Atom[]{Atom.newAtom(0), Atom.newAtom(0), Atom.newAtom(0), Atom.newAtom(0)});
		sk.call("glclear");
		
		/*
		sk.call("glmatrixmode", "projection");
		sk.call("glloadidentity");
		sk.call("gluperspective", new Atom[]{Atom.newAtom(55.), Atom.newAtom(1.0), Atom.newAtom(1.), Atom.newAtom(2.)});
		*/
				
		colors.add(new float[]{1.f, 0.f, 0.f});
		colors.add(new float[]{0.f, 1.f, 0.f});
		colors.add(new float[]{0.f, 0.f, 1.f});
		colors.add(new float[]{1.f, 1.f, 0.f});
		colors.add(new float[]{1.f, 0.f, 1.f});
		colors.add(new float[]{0.f, 1.f, 1.f});
		colors.add(new float[]{1.f, 1.f, 1.f});
		
		version();
	}	
	
	public void bang(){		
		if(shouldProcess){
			process();
			shouldProcess = false;
		}
		
		if(!sonogramStyle){
			r.send("erase");
			r.send("drawclients");
			r.send("swap");
			
			if(gl_jm != null)
				outlet(0, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(gl_jm.getName())});
		} else {
			if(jm != null)
				outlet(0, new Atom[]{Atom.newAtom("jit_matrix"), Atom.newAtom(jm.getName())});
		}
	}
	
	private void process(){
		if(zdata.size() > 0 && sonogramStyle == true){
			process_matrix();
		} else {
			process_gl();
		}
	}
	
	private void process_gl(){
		float aspect = (float)dim[0] / (float)dim[1];
		float[] thisColor = new float[3];
		float[] tmp;
		float[] x, y, z;
		boolean plotz = false;
		int[] order;
		
		if(shouldMakeDisplayBuffer) makeDisplayBuffer();
		clear_context();
		
		drawCrosshairs();
			
		float zz;
		
		synchronized(this.xdataBuffer){
		synchronized(this.ydataBuffer){
		synchronized(this.zdataBuffer){
			if(verifyData() > 0) return;
						
			order = new int[xdataBuffer.size()];
			for(int i = 0; i < xdataBuffer.size(); i++){
				if(i < activePlot) order[i + 1] = i;
				else if(i > activePlot) order[i] = i;
			}
			order[0] = activePlot;
						
			for(int i = 0; i < xdataBuffer.size(); i++){
				x = (float[])xdataBuffer.get(order[i]);
				y = (float[])ydataBuffer.get(order[i]);
				z = null;
				//post("order = " + order[i] + " " + zdataBuffer.size() + " " + xdataBuffer.size());
				if(zdataBuffer.size() > 0){
					z = (float[])zdataBuffer.get(order[i]);
					plotz = true;
				}
				
				tmp = (float[])colors.get(order[i] % colors.size());
				for(int c = 0; c < tmp.length; c++)
					thisColor[c] = tmp[c];
				
				if(order[i] != activePlot){
					sk.call("gllinestipple", new Atom[]{Atom.newAtom(1), Atom.newAtom(0xAAAA)});
					sk.call("glenable", "line_stipple");
					for(int c = 0; c < thisColor.length; c++) thisColor[c] *= 0.5;
				} else sk.call("gldisable", "line_stipple");
				
				sk.call("glcolor", new Atom[]{Atom.newAtom(thisColor[0]), Atom.newAtom(thisColor[1]), Atom.newAtom(thisColor[2])});
				
				if(plotz && z != null) zz = scale(z[0], min[2], max[2], -1.f, 1.f);
				else zz = defaultZ;
				
				sk.call("moveto", new Atom[]{Atom.newAtom(scale(x[0], min_disp, max_disp, -aspect, aspect)), 
					Atom.newAtom(scale(y[0], miny_disp, maxy_disp, -1.f, 1.f)), Atom.newAtom(zz)});
				
				for(int j = 1; j < x.length; j++){
					if(plotz && z != null) zz = scale(z[j], min[2], max[2], -1.f, 1.f);
					else zz = defaultZ;
					sk.call("lineto", new Atom[]{Atom.newAtom(scale(x[j], min_disp, max_disp, -aspect, aspect)), 
						Atom.newAtom(scale(y[j], miny_disp, maxy_disp, -1.f, 1.f)), Atom.newAtom(zz)});
						
				}
			}
			outputMinMax();
			if(mouseDown == 1) drawSelection();
			drawGrid();
		}}}
		
		shouldProcess = false;
	}
	
	private void process_matrix(){
		int xlength;
		int[] color;
		float[] x, y, z;
		synchronized(this.xdata){
		synchronized(this.ydata){
		synchronized(this.zdata){
			if(verifyData() > 0){
				post("error " + verifyData());
				return;
			}
			
			xlength = ((float[])xdata.get(0)).length;
			x = ((float[])xdata.get(0)); // we only allow one x array
			y = ((float[])ydata.get(0)); // and one y array
			if(jm == null) jm = new JitterMatrix(4, "char", xlength, y.length); // this assumes that all y arrays will be the same length
			for(int i = 0; i < x.length; i++){
				z = ((float[])zdata.get(i));
				for(int j = 0; j < y.length; j++){
					color = getColor(Math.round(scale(z[j], min[2], max[2], 0, 255)));
					jm.setcell2d((int)x[i], (int)y[j], new int[]{255, color[0], color[1], color[2]});
					//post(x[i] + " " + y[j] + " " + color[0] + " " + color[1] + " " + color[2]);
				}
			}
			outputMinMax();
		}}}
		shouldProcess = false;
	}
	
	private int verifyData(){
		if(xdata.size() == 0 || ydata.size() == 0) return 1;
		if(!sonogramStyle){
			if(xdata.size() != ydata.size()) return 2;
			for(int i = 0; i < xdata.size(); i++){
				if(((float[])xdata.get(i)).length != ((float[])ydata.get(i)).length) return 3;
				if(zdata.size() > 0){
					if(((float[])xdata.get(i)).length != ((float[])zdata.get(i)).length) return 4;
				}
			}
		} else {
			//post("zdata.size() = " + zdata.size() + " ((float[])xdata.get(0)).length = " + ((float[])xdata.get(0)).length);
			if(zdata.size() == 0) return 5;
			if(zdata.size() != ((float[])xdata.get(0)).length) return 6;
		}
		return 0;
	}
	
	private void makeDisplayBuffer(){
		float thismin, thismax, thisymin, thisymax;
		thismin = thismax = thisymin = thisymax = 0;	
		boolean doz;
		if(selectionBox_screen[0] < 0){
			thismin = min[0];
			thismax = max[0];
		} else {
			if(selectionBox_screen[2] > 0){
				thismin = scale(selectionBox_screen[0], 0.f, (float)dim[0], min[0], max[0]);
				thismax = scale(selectionBox_screen[0] + selectionBox_screen[2], 0, dim[0], min[0], max[0]);
			} else {
				thismax = scale(selectionBox_screen[0], 0.f, (float)dim[0], min[0], max[0]);
				thismin = scale(selectionBox_screen[0] + selectionBox_screen[2], 0, dim[0], min[0], max[0]);
			}
		}
		//post("min = " + min[0] + " max = " + max[0] + " thismin = " + thismin + " thismax = " + thismax);
		
		synchronized(this.xdata){
		synchronized(this.ydata){
		synchronized(this.zdata){
		synchronized(this.xdataBuffer){
		synchronized(this.ydataBuffer){
		synchronized(this.zdataBuffer){
			xdataBuffer.clear();
			ydataBuffer.clear();
			zdataBuffer.clear();
			doz = zdata.size() > 0;
			for(int i = 0; i < xdata.size(); i++){
				ArrayList xtmp = new ArrayList();
				ArrayList ytmp = new ArrayList();
				ArrayList ztmp = new ArrayList();
				
				float[] x_sub;// = new float[((float[])xdata.get(i)).length];
				float[] y_sub;// = new float[((float[])ydata.get(i)).length];
				float[] z_sub = null;
				//if(doz) z_sub = new float[((float[])zdata.get(i)).length];
				
				float[] x = ((float[])xdata.get(i));
				float[] y = ((float[])ydata.get(i));
				float[] z = null;
				if(doz) z = ((float[])zdata.get(i));
				
				int k = 0;
				for(int j = 0; j < x.length; j++){
					if(x[j] >= thismin && x[j] <= thismax){
						//post("x[" + j + "] = " + x[j] + " thismin = " + thismin + " thismax = " + thismax);
						xtmp.add(x[j]);
						ytmp.add(y[j]);
						if(doz) ztmp.add(z[j]);
						if(i == 0 && k == 0) thisymin = thisymax = y[j];
						if(y[j] > thisymax) thisymax = y[j];
						if(y[j] < thisymin) thisymin = y[j];
						++k;
					}
				}
				x_sub = new float[xtmp.size()];
				y_sub = new float[ytmp.size()];
				if(doz) z_sub = new float[ztmp.size()];
				for(int j = 0; j < xtmp.size(); j++){
					x_sub[j] = ((Float)(xtmp.get(j))).floatValue();
					y_sub[j] = ((Float)(ytmp.get(j))).floatValue();
					if(doz) z_sub[j] = ((Float)(ztmp.get(j))).floatValue();
				}
				xdataBuffer.add(x_sub);
				ydataBuffer.add(y_sub);
				if(doz) zdataBuffer.add(z_sub);
			}
			min_disp = thismin;
			max_disp = thismax;
			miny_disp = thisymin;
			maxy_disp = thisymax;
		}}}}}} // end of synchronization
		shouldMakeDisplayBuffer = false;
		currentXPos = 0;
		shouldDrawCrosshairs = false;
	}
	
	public void inlet(int i){
		switch(i){
			case 28: // ascii left arrow
				dec_x();
				break;
			case 29: // ascii right arrow
				inc_x();
				break;
		}
	}
    
	public void inlet(float f){
	}
    
	public void list(Atom[] args){
		int inlet = getInlet();
		float[] data = Atom.toFloat(args);
		float thisMax = max[inlet];
		float thisMin = min[inlet];
		boolean reset = false;
		
		switch(inlet){
			case 0:
				if(xdata.size() == 0) reset = true;
				xdata.add(data);
				break;
			case 1:
				if(ydata.size() == 0) reset = true;
				ydata.add(data);
				break;
			case 2:
				if(zdata.size() == 0) reset = true;
				zdata.add(data);
				break;
		}
		
		if(reset) thisMin = thisMax = data[0];
		
		for(int i = 0; i < data.length; i++){
			if(data[i] > thisMax) thisMax = data[i];
			if(data[i] < thisMin) thisMin = data[i];
		}
		max[inlet] = thisMax;
		min[inlet] = thisMin;
		
		if(xdata.size() == ydata.size()){
			shouldProcess = true;
			shouldMakeDisplayBuffer = true;
		}
	}
		
	private void drawCrosshairs(){
		if(!shouldDrawCrosshairs) return;
		float aspect = (float)dim[0] / (float)dim[1];
		float x = ((float[])xdataBuffer.get(activePlot))[currentXPos];
		float y = ((float[])ydataBuffer.get(activePlot))[currentXPos];
		float z;
		boolean usedefz = true;
		if(zdataBuffer.size() > 0 && !sonogramStyle) usedefz = false;
		if(!usedefz) z = ((float[])zdataBuffer.get(activePlot))[currentXPos];
		else z = defaultZ;
			
		
		float x_world = scale(x, min_disp, max_disp, -aspect, aspect);
		float y_world = scale(y, miny_disp, maxy_disp, -1, 1);
		float z_world = z;
		if(!usedefz) z_world = scale(z, min[2], max[2], -1, 1);
		
		
		sk.call("glcolor", new Atom[]{Atom.newAtom(1.f), Atom.newAtom(1.f), Atom.newAtom(1.f)});
		sk.call("moveto", new Atom[]{Atom.newAtom(x_world), Atom.newAtom(y_world), Atom.newAtom(z_world)});
		sk.call("lineto", new Atom[]{Atom.newAtom(x_world), Atom.newAtom(-1.f), Atom.newAtom(z_world)});
		sk.call("moveto", new Atom[]{Atom.newAtom(-aspect), Atom.newAtom(y_world), Atom.newAtom(z_world)});
		sk.call("lineto", new Atom[]{Atom.newAtom(x_world), Atom.newAtom(y_world), Atom.newAtom(z_world)});
		
		if(!usedefz)
			outlet(2, new Atom[]{Atom.newAtom("/coordinates"), Atom.newAtom(Math.round(x)), Atom.newAtom(y), Atom.newAtom(z)});
		else
			outlet(2, new Atom[]{Atom.newAtom("/coordinates"), Atom.newAtom(Math.round(x)), Atom.newAtom(y)});
	}
	
	private void drawGrid(){
		float aspect = (float)dim[0] / (float)dim[1];
		float x, y;
		float majorcolor = 0.25f;
		float minorcolor = 0.15f;
		if(major_grid > 0){
			sk.call("gldisable", "line_stipple");
			sk.call("glcolor", new Atom[]{Atom.newAtom(majorcolor), Atom.newAtom(majorcolor), Atom.newAtom(majorcolor)});
			for(int i = 0; i < major_grid; i++){
				x = (((2.f * aspect) / (major_grid)) * (i)) - aspect;
				y = ((2.f / major_grid) * i) - 1.f;
				sk.call("moveto", new Atom[]{Atom.newAtom(x), Atom.newAtom(1.f), Atom.newAtom(defaultZ)});
				sk.call("lineto", new Atom[]{Atom.newAtom(x), Atom.newAtom(-1.f), Atom.newAtom(defaultZ)});
				
				sk.call("moveto", new Atom[]{Atom.newAtom(-aspect), Atom.newAtom(y), Atom.newAtom(defaultZ)});
				sk.call("lineto", new Atom[]{Atom.newAtom(aspect), Atom.newAtom(y), Atom.newAtom(defaultZ)});
			}
		}
		if(minor_grid > 0){
			sk.call("gldisable", "line_stipple");
			sk.call("glcolor", new Atom[]{Atom.newAtom(minorcolor), Atom.newAtom(minorcolor), Atom.newAtom(minorcolor)});
			for(int i = 0; i < major_grid * minor_grid; i++){
				x = (((2.f * aspect) / (major_grid * minor_grid)) * (i)) - aspect;
				y = ((2.f / (major_grid * minor_grid)) * i) - 1.f;
				sk.call("moveto", new Atom[]{Atom.newAtom(x), Atom.newAtom(1.f), Atom.newAtom(defaultZ)});
				sk.call("lineto", new Atom[]{Atom.newAtom(x), Atom.newAtom(-1.f), Atom.newAtom(defaultZ)});
				
				sk.call("moveto", new Atom[]{Atom.newAtom(-aspect), Atom.newAtom(y), Atom.newAtom(defaultZ)});
				sk.call("lineto", new Atom[]{Atom.newAtom(aspect), Atom.newAtom(y), Atom.newAtom(defaultZ)});
			}
		}
	}
	
	private void drawSelection(){
		if(mouseDown == 0) return;
		
		sk.call("glenable", "blend");
		sk.call("glblendfunc", new Atom[]{Atom.newAtom("src_alpha"), Atom.newAtom("one_minus_src_alpha")});
		sk.call("glcolor", new Atom[]{Atom.newAtom(1.f), Atom.newAtom(0.f), Atom.newAtom(0.f), Atom.newAtom(.3f)});
		sk.call("quad", new Atom[]{Atom.newAtom(selectionBox_world[0]), Atom.newAtom(selectionBox_world[1]), Atom.newAtom(defaultZ),
									Atom.newAtom(selectionBox_world[0]), Atom.newAtom(selectionBox_world[1] + selectionBox_world[3]), Atom.newAtom(defaultZ), 
									Atom.newAtom(selectionBox_world[0] + selectionBox_world[2]), Atom.newAtom(selectionBox_world[1] + selectionBox_world[3]), Atom.newAtom(defaultZ), 
									Atom.newAtom(selectionBox_world[0] + selectionBox_world[2]), Atom.newAtom(selectionBox_world[1]), Atom.newAtom(defaultZ)});
		
		/*
		post("draw: " + selectionBox_world[0] + " " + selectionBox_world[1] + " " 
				+ selectionBox_world[0] + " " + (selectionBox_world[1] + selectionBox_world[3]) + " " 
				+ (selectionBox_world[0] + selectionBox_world[2]) + " " + (selectionBox_world[1] + selectionBox_world[3]) + " " 
				+ (selectionBox_world[0] + selectionBox_world[2]) + " " + selectionBox_world[1]);
		*/
	}
		
	private void clear_context(){
		sk.call("reset");
		
		//sk.call("glmatrixmode", "projection");
		//sk.call("glloadidentity");
		//sk.call("gluperspective", new Atom[]{Atom.newAtom(55.), Atom.newAtom(1.0), Atom.newAtom(1.), Atom.newAtom(2.)});
		
		sk.call("glclearcolor", new Atom[]{Atom.newAtom(0), Atom.newAtom(0), Atom.newAtom(0), Atom.newAtom(0)});
		sk.call("glclear");
		
	}
	
	private float scale(float v, float min_in, float max_in, float min_out, float max_out){
		return ((v - min_in) / (max_in - min_in)) * (max_out - min_out) + min_out;
	}
	
	public void clear(){
		shouldResetMinMax = true;
		//datamap.clear();
		xdata.clear();
		ydata.clear();
		zdata.clear();
		
		xdataBuffer.clear();
		ydataBuffer.clear();
		zdataBuffer.clear();
		
		clear_context();
		//sk.call("reset");
		//r.call("erase");
		numDims = 0;
		jm = null;
		currentXPos = 0;
		selectionBox_world = new float[]{-1, -1, -1, -1}; // x, y, w, h (xy = lower left corner)
		selectionBox_screen = new float[]{-1, -1, -1, -1}; // x, y, w, h (xy = lower left corner)
		shouldDrawCrosshairs = false;
		activePlot = 0;
	}
			
	public void notifyDeleted() {
		sk.freePeer();
		r.freePeer();
	}
	
	public void notify(JitterEvent e){
		//this gets the name of the listening context - in this case, the pwindow
		String subjectname = e.getSubjectName(); 
		//this gets the type of event...mouse, mouseidle, etc.
		String eventname = e.getEventName();
		Atom[] args = e.getArgs();
		float aspect = (float)dim[0] / (float)dim[1];
		
		MaxSystem.showCursor();
		if ((eventname.equals("mouse"))){
			if(args[4].toInt() == 1){ // command + shift
				if(mouseDown == 0){
					selectionBox_screen[0] = args[0].toFloat();
					selectionBox_screen[1] = args[1].toFloat();
					selectionBox_screen[2] = 0;
					selectionBox_screen[3] = 0;
					
					selectionBox_world[0] = scale(args[0].toFloat(), 0, dim[0], -aspect, aspect); // x
					selectionBox_world[1] = -1.f; // y
					selectionBox_world[2] = scale(args[0].toFloat(), 0, dim[0], -aspect, aspect); // w
					selectionBox_world[3] = 2.f; // h
				}
				selectionBox_screen[2] = args[0].toInt() - selectionBox_screen[0];
				selectionBox_screen[3] = args[1].toInt() - selectionBox_screen[1];
				
				selectionBox_world[2] = scale(args[0].toFloat(), 0, dim[0], -aspect, aspect) - selectionBox_world[0];
				selectionBox_world[3] = 2.f;
				
				if(args[2].toInt() == 0){
					shouldMakeDisplayBuffer = true;
				}
				mouseDown = args[2].toInt();
				//post("selections = " + selectionBox_world[0] + " " + selectionBox_world[1] + " " + selectionBox_world[2] + " " + selectionBox_world[3]);
				
			} else {
				if(args[2].toInt() == 1) MaxSystem.hideCursor();
				
				if(args[0].toInt() > mouseX) inc_x();
				else if(args[0].toInt() < mouseX) dec_x();
				mouseX = args[0].toInt();	
				shouldDrawCrosshairs = true;					
			}
			
			shouldProcess = true;
		}
	}
	
	public void inc_x(){
		synchronized(this.xdataBuffer){
			if(xdataBuffer.size() == 0) return;
			if(currentXPos + 1 < ((float[])xdataBuffer.get(activePlot)).length - 0) ++currentXPos;
		}
		shouldProcess = true;
	}
	
	public void dec_x(){
		if(xdataBuffer.size() == 0) return;
		if(currentXPos - 1 >= 0) --currentXPos;
		shouldProcess = true;
	}
	
	public void movecrosshair(int x){
		if(xdata.size() == 0) return;
		//if(x <= ((float[])xdataBuffer.get(activePlot)).length - 1 && x >= 0) currentXPos = x;
		shouldProcess = true;
	}
	
	public void dump(){
		
	}
	
	public void activePlot(int i){
		if(i > xdata.size() - 1) return;
		if(i < 0) return;
		activePlot = i;
		shouldProcess = true;
	}
	
	public void dim(Atom[] args){
		dim[0] = args[0].toInt();
		if(args.length > 1)
			dim[1] = args[1].toInt();
		else dim[1] = dim[0];
		gl_jm = new JitterMatrix(4, "char", dim[0], dim[1]);
	}
	
	private int[] getColor(int v){
		if(v < 0 || v > 255){
			post("gonna return null for value: " + v);
			return null;
		}
		int[] colors = new int[]{0, 0, 0};
		
		if(xyz_colorscheme.compareTo("rainbow2") == 0){
			colors[0] = (int)Math.round(Math.sin(.25 * 2 * Math.PI * (v / 256.)) * 255.);
			colors[1] = (int)Math.round(Math.sin(.5 * 2 * Math.PI * (v / 256.)) * 255.);
			colors[2] = (int)Math.round(Math.cos(.25 * 2 * Math.PI * (v / 256.)) * 255.);
		} else if(xyz_colorscheme.compareTo("rainbow1") == 0){
			colors[0] = (int)Math.round(Math.pow(scale(v, 0, 255, 0, 16), 2));
			//colors[1] = (int)Math.round(Math.sin(.5 * 2 * Math.PI * (v / 256.)) * 255.);
			if(v <= 127) colors[1] = v * 2;
			else colors[1] = 255 - ((v - 128) * 2);
			colors[2] = (int)Math.round(Math.pow(scale(v - 255, 0, 255, 0, -16), 2));
		} else if(xyz_colorscheme.compareTo("rainbow3") == 0){
			colors[0] = v;
			if(v >= 128) colors[1] = 128 - (v - 128);
			else colors[1] = v;
			colors[1] = (int)Math.round(Math.sqrt((double)colors[1] / 128.) * 200.);
		
			colors[2] = (int)Math.round(Math.sqrt((255. - (double)v) / 255.) * 255.);	
		} else if(xyz_colorscheme.compareTo("grayscale") == 0){
			colors[0] = colors[1] = colors[2] = v;
		}
		/*
		for(int i = 0; i < 3; i++)
			colors[i] = v + 100 >= 255 ? colors[i] : (int)Math.round(colors[i] * ((v + 100) / 255.));
		*/
		return colors;
	}
	
	public void set_xyz_colorscheme(String s){
		if(s.compareTo("rainbow1") == 0 || s.compareTo("rainbow2") == 0 || 
				s.compareTo("rainbow3") == 0 || s.compareTo("grayscale") == 0){
			xyz_colorscheme = s;
			shouldProcess = true;
		}
	}	
	
	public void setz(float zz){
		defaultZ = zz;
		shouldProcess = true;
	}
	
	public void outputMinMax(){
		if(xdata.size() > 0){
			outlet(2, new Atom[]{Atom.newAtom("/x/max"), Atom.newAtom(max_disp)});
			outlet(2, new Atom[]{Atom.newAtom("/x/min"), Atom.newAtom(min_disp)});
		}
		if(ydata.size() > 0){
			outlet(2, new Atom[]{Atom.newAtom("/y/max"), Atom.newAtom(maxy_disp)});
			outlet(2, new Atom[]{Atom.newAtom("/y/min"), Atom.newAtom(miny_disp)});
		}
		if(zdata.size() > 0){
			outlet(2, new Atom[]{Atom.newAtom("/z/max"), Atom.newAtom(max[2])});
			outlet(2, new Atom[]{Atom.newAtom("/z/min"), Atom.newAtom(min[2])});
		}
	}
	
	public void sonogramStyle(int i){
		if(i == 0) sonogramStyle = false;
		else sonogramStyle = true;
		shouldProcess = true;
	}
	
	public void rotate(Atom[] args){
		for(int i = 0; i < args.length; i++)
			System.out.println(args[i].toFloat());
	}
	
	public void major_grid(int i){
		if(i < 0) major_grid = 0;
		else if(i > max_major_grid) major_grid = max_major_grid;
		else major_grid = i;
		shouldProcess = true;
	}
	
	public void minor_grid(int i){
		if(i < 0) minor_grid = 0;
		else if(i > max_minor_grid) minor_grid = max_minor_grid;
		else minor_grid = i;
		shouldProcess = true;
	}
	
	public void zoomOut(){
		selectionBox_screen[0] = 0;
		selectionBox_screen[1] = 0;
		selectionBox_screen[2] = dim[0];
		selectionBox_screen[3] = dim[1];
		
		selectionBox_world[0] = -((float)dim[0] / (float)dim[1]);
		selectionBox_world[1] = -1;
		selectionBox_world[2] = -((float)dim[0] / (float)dim[1]);
		selectionBox_world[3] = 2;
		
		shouldMakeDisplayBuffer = true;
		shouldProcess = true;
	}
}
