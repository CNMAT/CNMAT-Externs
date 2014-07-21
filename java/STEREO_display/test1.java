import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class test1 extends MaxObject{
	public test1(){}

	public void float64_1(){
		JitterMatrix jm = new JitterMatrix(1, "float64", 1, 1);
		jm.setcell(new int[]{0, 0}, new double[]{0.618034});
		outlet(0, "jit_matrix", jm.getName());
	
		double[] foo = new double[1];
		jm.copyMatrixToArray(foo);
		post("val = " + jm.getcellDouble(new int[]{0, 0})[0]);
		post("val from array = " + foo[0]);
	}

	public void float64_2(){
		JitterMatrix jm = new JitterMatrix(1, "float64", 1, 1);
		jm.setcell(new int[]{0, 0}, 0, 0.618034);
		outlet(0, "jit_matrix", jm.getName());
	
		double[] foo = new double[1];
		jm.copyMatrixToArray(foo);
		post("val = " + jm.getcellDouble(new int[]{0, 0})[0]);
		post("val from array = " + foo[0]);
	}

	public void float32_1(){
		JitterMatrix jm = new JitterMatrix(1, "float32", 1, 1);
		jm.setcell(new int[]{0, 0}, new float[]{0.618034f});
		outlet(0, "jit_matrix", jm.getName());
	
		float[] foo = new float[1];
		jm.copyMatrixToArray(foo);
		post("val = " + jm.getcellFloat(new int[]{0, 0})[0]);
		post("val from array = " + foo[0]);
	}

	public void float32_2(){
		JitterMatrix jm = new JitterMatrix(1, "float32", 1, 1);
		jm.setcell(new int[]{0, 0}, 0, 0.618034f);
		outlet(0, "jit_matrix", jm.getName());
	
		float[] foo = new float[1];
		jm.copyMatrixToArray(foo);
		post("val = " + jm.getcellFloat(new int[]{0, 0})[0]);
		post("val from array = " + foo[0]);
	}

	public void float32_rand(){
		float r = (float)Math.random();
		JitterMatrix jm = new JitterMatrix(1, "float32", 1, 1);
		jm.setcell(new int[]{0, 0}, 0, r);
		outlet(0, "jit_matrix", jm.getName());
	
		float[] foo = new float[1];
		jm.copyMatrixToArray(foo);
		post("val = " + jm.getcellFloat(new int[]{0, 0})[0]);
		post("val from array = " + foo[0]);

		post("r = " + r);
		if(r == jm.getcellFloat(new int[]{0, 0})[0]) post("r == getcellFloat: true");
		else post("r == getcellFloat: false");
		if(r == foo[0]) post("r == val from array: true");
		else post("r == val from array: true");
	}

	public void float64_rand(){
		double r = Math.random();
		JitterMatrix jm = new JitterMatrix(1, "float64", 1, 1);
		jm.setcell(new int[]{0, 0}, 0, r);
		outlet(0, "jit_matrix", jm.getName());
	
		double[] foo = new double[1];
		jm.copyMatrixToArray(foo);
		post("val = " + jm.getcellDouble(new int[]{0, 0})[0]);
		post("val from array = " + foo[0]);

		post("r = " + r);
		if(r == jm.getcellDouble(new int[]{0, 0})[0]) post("r == getcellDouble: true");
		else post("r == getcellDouble: false");
		if(r == foo[0]) post("r == val from array: true");
		else post("r == val from array: true");
	}

}
