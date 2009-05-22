import com.cycling74.max.*;
import com.cycling74.jitter.*;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.ListIterator;
import java.io.File;
import java.io.FileWriter;
import java.io.FileReader;
import java.io.BufferedReader;

public class bpf extends MaxObject{
	private String drawto = null;
	private ArrayList<LinkedList<Point3D>> functions = new ArrayList<LinkedList<Point3D>>();
	private int currentFunction = 0;
	private JitterObject sk;
	private MaxQelem q = new MaxQelem(this, "set_drawto");
	private int selected = -1;

	public class Point3D extends java.awt.geom.Point2D.Double{
		private double z;

		public Point3D(double[] coords){
			x = coords[0];
			y = coords[1];
			z = coords[2];
		}

		public double getZ(){return z;}
		public void setLocation(double xx, double yy, double zz){
			x = xx;
			y = yy;
			z = zz;
		}
		public String toString(){
			return x + " " + y + " " + z;
		}

		public double[] getCoords(){
			return new double[]{x, y, z};
		}
	}

	public bpf(){
		declareAttribute("drawto");
		declareIO(1, 1);
		createInfoOutlet(false);
		functions.add(new LinkedList<Point3D>());
		q.set();
		sk = new JitterObject("jit.gl.sketch");

	}

	public bpf(String drawto){
		this();
		drawto = drawto;
	}

	public void list(Atom[] args){
		double[] size = getScreenSize();
		switch(args.length){
		case 2:
			insert((int)(args[0].toDouble() * size[0]), (int)(Math.abs(size[1] - (args[1].toDouble() * size[1]))), currentFunction);
			break;
		case 3:
			insert((int)(args[1].toDouble() * size[0]), (int)(Math.abs(size[1] - (args[2].toDouble() * size[1]))), args[0].toInt());
			break;
		}
		drawFunctions();
	}

	public void inlet(float f){
		if(f < 0){
			f = 0.f;
		}else if(f > 1){
			f = 1.f;
		}
		double[] size = getScreenSize();

		double xs = f * size[0];
		Atom[] tmp = sk.call("screentoworld", new Atom[]{Atom.newAtom(xs)});
		double xw = tmp[0].toDouble();
		//post(xs + "");
		double ys, yw, y, ys1, ys2;
		int i = 0;
		for(LinkedList<Point3D> ll : functions){
			if(xw < ll.getFirst().x || xw > ll.getLast().x){
				outlet(0, new Atom[]{Atom.newAtom(i), Atom.newAtom(f), Atom.newAtom(0.0f)});
			}else if(xw == ll.getFirst().x){
				Atom[] screen = sk.call("worldtoscreen", Atom.newAtom(ll.getFirst().getCoords()));
				outlet(0, new Atom[]{Atom.newAtom(i), Atom.newAtom(f), Atom.newAtom(Math.abs(size[1] - screen[1].toDouble()) / size[1])});
			}else{
				ListIterator<Point3D> it = ll.listIterator();
				Point3D prev = it.next();
				Point3D current;
				while(it.hasNext()){
					current = it.next();
					if(xw >= prev.x && xw <= current.x){
						Atom[] prev_screen = sk.call("worldtoscreen", Atom.newAtom(prev.getCoords()));
						Atom[] current_screen = sk.call("worldtoscreen", Atom.newAtom(current.getCoords()));
						double m = ((prev_screen[1].toDouble() - current_screen[1].toDouble()) / 
							   (prev_screen[0].toDouble() - current_screen[0].toDouble()));
						double b = prev_screen[1].toDouble() - (m * prev_screen[0].toDouble());
						ys = (m * xs) + b;
						outlet(0, new Atom[]{Atom.newAtom(i), Atom.newAtom(f), Atom.newAtom(Math.abs(size[1] - ys) / size[1])});
					}
					prev = current;
				}
			}
			i++;
		}
	}

	private double[] getScreenSize(){
		// Get the size of the window we're drawing to.  Seems like there must be a better way of doing this...
		Atom[] size = sk.call("screentoworld", new Atom[]{Atom.newAtom(0), Atom.newAtom(0)});
		size = sk.call("worldtoscreen", new Atom[]{Atom.newAtom(size[0].toFloat() * -1.), Atom.newAtom(size[1].toFloat() * -1.)});
		double[] out = new double[2];
		out[0] = size[0].toDouble();
		out[1] = size[1].toDouble();
		return out;
	}

	public void addFunction(){
		functions.add(new LinkedList<Point3D>());
		currentFunction = functions.size() - 1;
		drawFunctions();
	}

	public void removeFunction(){
		removeFunction(currentFunction);
	}

	public void removeFunction(int f){
		functions.remove(f);
		if(currentFunction - 1 > functions.size()){
			currentFunction = 0;
		}
	}

	public void setFunction(int f){
		if(f > functions.size()){
			error("bpf: there are only " + functions.size() + " functions");
			return;
		}
		currentFunction = f;
		drawFunctions();
	}

	private int insert(int x, int y, int cf){
		double min = 10000000000.;
		int mini = -1;
		Atom[] world_atom = sk.call("screentoworld", new Atom[]{Atom.newAtom(x), Atom.newAtom(y)});
		double[] world = new double[]{world_atom[0].toDouble(), world_atom[1].toDouble(), world_atom[2].toDouble()};
		int i = 0;

		if(cf > functions.size() - 1){
			for(i = functions.size() - 1; i < cf; i++){
				functions.add(new LinkedList<Point3D>());
			}
		}

		//post("inserting point at " + x + " " + y);
		LinkedList<Point3D> function = functions.get(cf);
		if(function.size() == 0){
			function.add(new Point3D(world));
			return 0;
		}


		if(function.size() == 1){
			if(function.getFirst().getX() < world[0]){
				function.add(new Point3D(world));
				return 1;
			}else{
				function.add(0, new Point3D(world));
				return 0;
			}
		}
		for(Point3D p : functions.get(cf).toArray(new Point3D[0])){
			if(Math.abs((p.x - world[0])) < min){
				min = Math.abs(p.x - world[0]);
				mini = i;
			}
			i++;
		}

		Point3D p = functions.get(cf).get(mini);
		if(p.x < world[0]){		
			//post("inserting " + world[0] + " into slot " + mini + 1);
			functions.get(cf).add(mini + 1, new Point3D(world));
			return mini + 1;
		}else{
			//post("inserting " + world[0] + " into slot " + mini);
			functions.get(cf).add(mini, new Point3D(world));
			return mini;
		}
	}

	private void remove(int i){
		if(i < 0 || i > functions.get(currentFunction).size()){
			return;
		}
		LinkedList<Point3D> function = functions.get(currentFunction);
		function.remove(i);
	}

	private int findNearby(int x, int y){
		LinkedList<Point3D> ll = functions.get(currentFunction);
		// look for an x point within 2 pixels
		int i = 0;
		post("number of points: " + ll.size());
		for(Point3D p : ll.toArray(new Point3D[0])){
			Atom[] screen = sk.call("worldtoscreen", Atom.newAtom(p.getCoords()));
			double xs = screen[0].toInt();
			double ys = screen[1].toInt();
			if(Math.abs(x - xs) < 3 && Math.abs(y - ys) < 3){
				//post("selected point " + i + " " + x + " " + y + " " + xs + " " + ys);
				return i;
			}
			i++;
		}

		return -1;
	}

	public void mouse(Atom[] args){
		int near = findNearby(args[0].toInt(), args[1].toInt());

		if(args[4].toInt() == 1){
			if(near >= 0){
				remove(near);
			}
			drawFunctions();
			return;
		}

		if(near != -1){
			selected = near;
		}

		if(selected != -1){
			remove(selected);
		}
		selected = insert(args[0].toInt(), args[1].toInt(), currentFunction);
		if(args[2].toInt() == 0){
			selected = -1;
		}
		post("number of points: " + functions.get(currentFunction).size());
		drawFunctions();
	}

	private void drawFunctions(){
		sk.call("glclear");
		int i = 0;
		for(LinkedList<Point3D> ll : functions){
			Point3D prev = null;
			Atom[] ret;
			sk.call("glenable", new Atom[]{Atom.newAtom("line_stipple")});
			sk.call("glenable", new Atom[]{Atom.newAtom("line_width")});
			if(i == currentFunction){
				ret = sk.call("gllinestipple", new Atom[]{Atom.newAtom(1), Atom.newAtom((short)0xffff)});
				ret = sk.call("gllinewidth", new Atom[]{Atom.newAtom(2)});
			}else{
				ret = sk.call("gllinestipple", new Atom[]{Atom.newAtom(1), Atom.newAtom((short)0x1C47)});
				ret = sk.call("gllinewidth", new Atom[]{Atom.newAtom(1)});
			}

			if(ret != null){
				for(int j = 0; j < ret.length; j++){
					post(ret[j].toString());
				}
			}
			for(Point3D p : ll.toArray(new Point3D[0])){
				//post(i + " " + p.toString());

				sk.call("moveto", Atom.newAtom(p.getCoords()));
				sk.call("circle", new Atom[]{Atom.newAtom(0.02)});
				if(prev != null){
					sk.call("lineto", Atom.newAtom(prev.getCoords()));
				}
				prev = p;
			}
			i++;
		}
	}

	private void set_drawto(){
		if(drawto != null){
			sk.call("drawto", drawto);
		}
	}

	public void notifyDeleted(){
		sk.call("glclear");
		sk.freePeer();
	}

	public void clear(){
		for(LinkedList ll : functions){
			ll.clear();
		}
		drawFunctions();
	}

	public void dump(){
		int i = 0;
		double[] size = getScreenSize();
		for(LinkedList<Point3D> ll : functions){
			for(Point3D p : ll.toArray(new Point3D[0])){
				Atom[] screen = sk.call("worldtoscreen", Atom.newAtom(p.getCoords()));
				outlet(0, new Atom[]{Atom.newAtom(i), Atom.newAtom(screen[0].toDouble() / size[0]), Atom.newAtom(Math.abs(size[1] - screen[1].toDouble()) / size[1])});
			}
			i++;
		}
	}

	public void write(){
		String filename = MaxSystem.saveAsDialog("", "");
		if(filename != null){
			write(filename);
		}
	}

	public void write(String filename){
		File fp = new File(filename);
		FileWriter fw;
		try{fw = new FileWriter(fp);}
		catch(Exception e){
			e.printStackTrace();
			return;
		}
		String out;
		int i = 0;
		double[] size = getScreenSize();
		for(LinkedList<Point3D> ll : functions){
			for(Point3D p : ll.toArray(new Point3D[0])){
				Atom[] screen = sk.call("worldtoscreen", Atom.newAtom(p.getCoords()));
				out = Atom.newAtom(i) + " " + Atom.newAtom(screen[0].toDouble() / size[0]) + " " + Atom.newAtom(Math.abs(size[1] - screen[1].toDouble()) / size[1]) + "\n";
				try{fw.write(out);}
				catch(Exception e){
					e.printStackTrace();
				}
			}
			i++;
		}
		try{fw.close();}
		catch(Exception e){
			e.printStackTrace();
		}
	}

	public void read(){
		String filename = MaxSystem.openDialog("");
		if(filename != null){
			read(filename);
		}
	}

	public void read(String filename){
		File fp = new File(filename);
		BufferedReader br;
		try{
			br = new BufferedReader(new FileReader(fp));
			String s = br.readLine();
			while(s != null){
				String[] parsed = s.split(" ");
				Atom[] l = new Atom[parsed.length];
				int i = 0;
				for(String ss : parsed){
					l[i] = Atom.newAtom(new Double(ss));
					i++;
				}
				list(l);
				s = br.readLine();

			}
		}catch(Exception e){
			e.printStackTrace();
			return;
		}

	}

}