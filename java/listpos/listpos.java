import com.cycling74.max.*;
import java.util.ArrayList;

public class listpos extends MaxObject{
	private float n = 666.f;

	public listpos(int i){
		this();
		n = (float)i;
	}

	public listpos(float i){
		this();
		n = i;
	}

	public listpos(){
		declareIO(2, 2);
		createInfoOutlet(false);
	}

	public void inlet(int i){
		if(getInlet() == 1){
			n = (float)i;
		}
	}

	public void inlet(float i){
		if(getInlet() == 1){
			n = i;
		}
	}

	public void list(Atom[] l){
		if(getInlet() == 0){
			ArrayList<Atom> al = new ArrayList<Atom>(l.length);
			Atom[] out;
			for(int i = 0; i < l.length; i++){
				if(n == l[i].toFloat()){
					al.add(Atom.newAtom(i));
				}
			}
			out = new Atom[al.size()];
			out = al.toArray(out);
			if(out.length > 0){
				outlet(1, out.length);
				outlet(0, out);
			}
		}
	}
}