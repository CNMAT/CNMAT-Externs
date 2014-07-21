import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class test2 extends MaxObject{
	public test2(){}

	public void float64(String s){
		JitterMatrix jm = new JitterMatrix(s);
		double[] foo = new double[1];
		post("val = " + jm.getcellDouble(new int[]{0, 0})[0]);
		post("val from array = " + foo[0]);
	}

	public void float32(String s){
		JitterMatrix jm = new JitterMatrix(s);
		float[] foo = new float[1];
		post("val = " + jm.getcellFloat(new int[]{0, 0})[0]);
		post("val from array = " + foo[0]);
	}

}
