import com.cycling74.max.*;

import java.io.*;
import java.util.*;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;

public class net_download extends MaxObject
{	
	public void version(){
		if(!alreadydidcopyright){
			post("net_download Version 2.0, by John MacCallum.\nCopyright (c) 2007-08 Regents of the University of California.  All rights reserved.");
			alreadydidcopyright = true;
		}
	}
	private static boolean alreadydidcopyright = false;		
	private boolean verbose = false;
	private String downloadDir = null;
	private boolean percent = false;
	private int sleepinterval = 60000;
	private boolean willDelete = false;
	private Socket socket = null;
	private URL url = null;
	private boolean net_download_willexit = false;
	private NetDownloadClientThread client = null;

	public void download(String u){
		try{url = new URL(u);}
		catch(Exception e){
			error("net_download: " + u + " is a malformed url");
			return;
		}
       	}

	public net_download(Atom[] args){
		declareOutlets(new int[]{DataTypes.ALL, DataTypes.ALL});	
		createInfoOutlet(false);
		declareAttribute("percent");
		declareAttribute("verbose");
		declareAttribute("sleepinterval");
		downloadDir = System.getProperty("java.io.tmpdir");

		version();
	}	

	public void test(){
		StringBuffer buffer = new StringBuffer(8192);
		Socket socket = null;
		//BufferedInputStream bir = null;
		BufferedReader br = null;
		OutputStream os = null;
		PrintWriter pw = null;

		try{socket = new Socket(url.getHost(), 80);}
		catch(Exception e){
			e.printStackTrace();
			return;
		}

		try{os = socket.getOutputStream();}
		catch(IOException e){
			e.printStackTrace();
			return;
		}
		try{pw = new PrintWriter(socket.getOutputStream(), true);} // last param is autoflush
		catch(IOException e){
			e.printStackTrace();
			return;
		}
		try{br = new BufferedReader(new InputStreamReader(socket.getInputStream()));}
		catch(Exception e){
			e.printStackTrace();
			return;
		}

		pw.println("OPTIONS * HTTP/1.1");
		pw.println("Host: localhost:8080");
		pw.println();

		boolean loop = true;
		int counter = 0;

		while(loop){
			try{
				if(br.ready()){
					int i = 0;
					while(i != -1){
						i = br.read();
						if(i == -1) break;
						buffer.append((char)i);
					}
					loop = false;
				}
			}catch(Exception e){
				e.printStackTrace();
				return;
			}
		}
		post(buffer.toString());
	}

	public void bang(){

		//download("http://stereo-ssc.nascom.nasa.gov/data/beacon/ahead/impact/2008/07/STA_LB_IMPACT_20080728_V02.cdf");
		if(url == null){
			error("net_download: you must specify a url to download!");
			return;
		}

		try{socket = new Socket(url.getHost(), 80);}
		catch(Exception e){
			error("net_download: couldn't find host " + url.getHost());
			return;
		}
		if(client != null) client = null;
		client = new NetDownloadClientThread(socket, url, true);
	}

	public void notifyDeleted(){
		post("calling stop method");
		if(client != null) client.stopThread();
	}

	public void setDownloadDir(String s){

        }

	public void percent(boolean b){
		percent = b;
	}

	public void verbose(boolean b){
		verbose = b;
	}

	public void sleepinterval(int i){
		sleepinterval = i;
	}
}
