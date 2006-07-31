import com.cycling74.max.*;

public class tc_editor extends MaxObject
{

	private static final String[] INLET_ASSIST = new String[]{
		"from jit.cellblock first outlet",
		"from jit.cellblock third outlet"
	};
	private static final String[] OUTLET_ASSIST = new String[]{
		"to jit.cellblock inlet""
	};
	
	public firsttime(Atom[] args)
	{
		declareInlets(new int[]{DataTypes.ALL, DataTypes.ALL});
		declareOutlets(new int[]{DataTypes.ALL});
		
		setInletAssist(INLET_ASSIST);
		setOutletAssist(OUTLET_ASSIST);

	}
    
	public void bang()
	{
	}
    
	public void inlet(int i)
	{
	}
    
	public void inlet(float f)
	{
	}
    
    
	public void list(Atom[] list)
	{
	}
    
}



