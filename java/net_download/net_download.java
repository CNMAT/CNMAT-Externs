// Thanks to Ali Momeni for some of this code.

import com.cycling74.max.*;
import com.cycling74.jitter.*;

import java.io.*;
import java.util.*;
import java.net.*;
import javax.swing.text.*;
import javax.swing.text.html.*;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;

public class net_download extends MaxObject
{	
	public void version(){
		post("net_download Version 1.0, by John MacCallum.\nCopyright (c) 2007 Regents of the University of California.  All rights reserved.");
	}

	public net_download(Atom[] args){
		declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL});	
		version();
	}	
	
	public void bang(){
	}
    
	public void inlet(int i){
	}
    
	public void inlet(float f){
	}
    
	public void list(Atom[] args){
	}
	
	public void get_html(Atom[] args){
		String url;
		String tmp;
		ArrayList tags = new ArrayList();
		ArrayList attributes = new ArrayList();
		int counter = 2;
		
		url = args[0].getString();
		
		if(args.length == 1) return;
		
		if(args[1].getString().compareTo("tags") == 1) return;
		
		tmp = args[2].getString();
		while(tmp.compareTo("attributes") != 0){
			tags.add(tmp);
			tmp = args[++counter].getString();
		}
		for(int i = ++counter; i < args.length; i++){
			attributes.add(args[i].getString());
		}
		get_html(url, tags, attributes);
	}
	
	public void get_html(final String url, final ArrayList tags, final ArrayList attributes){
		Thread t = new Thread() 
			{
			public void run()
				{
				Reader reader;
				URLConnection conn;
				try{
					conn = new URL(url).openConnection();
					conn.addRequestProperty("User-Agent", "Mozilla/4.76"); 
				   
					reader = new InputStreamReader(conn.getInputStream());
				} catch (Exception e){
					e.printStackTrace();
					return;
				}
				
				EditorKit kit = new HTMLEditorKit();
				Document doc = kit.createDefaultDocument();

				// post("searching..."+str);
				// The Document class does not yet handle charset's properly.
				doc.putProperty("IgnoreCharsetDirective", Boolean.TRUE);
				try {
					// Parse the HTML.
					kit.read(reader, doc, 0);

					// Iterate through the elements of the HTML document.
					ElementIterator it = new ElementIterator(doc);
					javax.swing.text.Element elem;
					
					while ((elem = it.next()) != null) {
						for(int i = 0; i < tags.size(); i++){
							SimpleAttributeSet s = (SimpleAttributeSet)elem.getAttributes().getAttribute(HTML.getTag((String)tags.get(i)));
								
							if (s != null) {				
								for(int j = 0; j < attributes.size(); j++){
									String link = s.getAttribute(HTML.getAttributeKey((String)attributes.get(j))).toString();
									outlet(1, url);
									outlet(0, new Atom[]{Atom.newAtom((String)tags.get(i)), Atom.newAtom((String)attributes.get(j)), 
										Atom.newAtom(link.toString())});
								}
							}
						}				
					}
				outletBang(1);
				
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		};
		t.start();
	}
	
	public void get_image(final Atom[] args){	
		Thread t = new Thread() 
			{
			public void run()
				{
					String imageURL = args[0].toString();
					String outputPath = null;
					JitterMatrix jitmat;
					URL url;
					
					if(args.length > 1)
						outputPath = args[1].toString();
						
					BufferedImage image = null;
					try{ 
						url = new URL(imageURL);
						image = ImageIO.read(url);
						jitmat = new JitterMatrix(image);
					}catch (IOException e) {
						post("¥ error: net_download: couldn't find an image at " + imageURL);
						e.printStackTrace();
						return;
					}
					outlet(0,"image",new Atom[]{Atom.newAtom("jit_matrix"),Atom.newAtom(jitmat.getName())});
					if(outputPath != null){
						File f = new File(outputPath);
						try{
							ImageIO.write(image, "jpg", f);
							outlet(0, "image_filename", outputPath);
						} catch(Exception e){
							e.printStackTrace();
						}
					}
				}
			};
		t.start();
		
	}
	
	public void get_file(final String url){
		Thread t = new Thread() 
		{
		public void run()
			{
			BufferedInputStream iostream = null;
			URLConnection conn;
			ArrayList<Byte> buffer = new ArrayList();
			int b = 0;
			//File out = new File("/Users/johnmac/porkbutt.cdf");
			File fp = null;
			FileOutputStream iostreamout = null;
			int offset = 0;
			try{
				fp = File.createTempFile(url.substring(url.lastIndexOf("/") + 1), "");
				iostreamout = new FileOutputStream(fp);
			} catch(Exception e){
				e.printStackTrace();
			}
			
			try{
				conn = new URL(url).openConnection();
				conn.addRequestProperty("User-Agent", "Mozilla/4.76"); 
				iostream = new BufferedInputStream(conn.getInputStream());
			}catch(Exception e){e.printStackTrace();}
			
			try{b = iostream.read();}
			catch(Exception e){e.printStackTrace();}
			int counter = 0;
			while(b >= 0){
				try{
					buffer.add(new Byte((byte)b));
					b = iostream.read();
					++offset;
				} catch (Exception e){
					e.printStackTrace();
					return;
				}
			}
			
			byte[] buffer_byte = new byte[buffer.size()];
			for(int i = 0; i < buffer.size(); i++)
				buffer_byte[i] = ((Byte)buffer.get(i)).byteValue();
			try{
				iostreamout.write(buffer_byte);
			} catch (Exception e){
				e.printStackTrace();
				return;
			}
			try{iostreamout.close();}
			catch(Exception e){e.printStackTrace();}
			outlet(0, new Atom[]{Atom.newAtom("filepath"), Atom.newAtom(fp.getAbsolutePath())});
			post("finished downloading\n\t" + url);
			}
		};
		t.start();
	}
		
	public void clear(){
	}
		
	public void notifyDeleted() {
	}
	
}
