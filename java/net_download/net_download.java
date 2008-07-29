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
			post("net_download Version 1.1, by John MacCallum.\nCopyright (c) 2007-08 Regents of the University of California.  All rights reserved.");
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

	public class nd_Header{
		LinkedHashMap<String, String> hm = new LinkedHashMap<String, String>(20);
		public nd_Header(String h){
			parseHeader(h);
		}

		private void parseHeader(String h){
			String[] responses = h.split("\n");
			hm.put("response-code", responses[0]);
			for(int i = 1; i < responses.length - 2; i++){
				String[] subs = responses[i].split(":");
				hm.put(subs[0], subs[1].trim());
			}
		}

		public String get(String k){
			return hm.get(k);
		}

		public Set keySet(){
			return hm.keySet();
		}
	}

	public class nd_Client extends Thread{
		OutputStream os = null;
		PrintWriter pw = null;
		BufferedReader br = null;
		URL url = null;
		int amountDownloaded = 0;
		int sleepInterval = 60000;
		boolean verbose = false;

		public nd_Client(Socket sock, URL u, boolean verb){
			this(sock, u);
			verbose = verb;
		}

		public nd_Client(Socket sock, URL u, int si, boolean verb){
			this(sock, u, si);
			verbose = verbose;
		}

		public nd_Client(Socket sock, URL u, int si){
			this(sock, u);
			sleepInterval = si;
		}

		public nd_Client(Socket sock, URL u){
			try{os = sock.getOutputStream();}
			catch(IOException e){
				e.printStackTrace();
				return;
			}
			try{pw = new PrintWriter(sock.getOutputStream(), true);} // last param is autoflush
			catch(IOException e){
				e.printStackTrace();
				return;
			}
			try{br = new BufferedReader(new InputStreamReader(sock.getInputStream()));}
			catch(IOException e){
				e.printStackTrace();
				return;
			}
			try{br.mark(1000000);}
			catch(Exception e){
				e.printStackTrace();
			}
			url = u;
			start();
		}

		public void run(){
			String h = requestHeader(url.getFile());
			nd_Header header = new nd_Header(h);
			if(verbose) printHeader(header);
			Integer length = new Integer(header.get("Content-Length"));
			byte[] buffer = null;
			if(length > amountDownloaded){
				buffer = requestContent(url.getFile(), amountDownloaded, length);
			}
			if(buffer == null){
				error("net_download: null buffer");
				return;
			}

			FileOutputStream fos;
			try{
				File fp = new File("/Users/johnmac/shithole.cdf", true); // append = true
				fos = new FileOutputStream(fp);
			}catch(Exception e){
				e.printStackTrace();
				return;
			}
			try{fos.write(buffer);}
			catch(Exception e){
				e.printStackTrace();
			}
		}

		private byte[] requestContent(String file, int start, int end){
			ByteBuffer buffer = ByteBuffer.allocate(end - start);
			if(verbose){
				post("getting file " + file + " from host " + url.getHost());
				post("GET " + file + " HTTP/1.1");
				post("Host: localhost:8080");
				post("Connection: Close");
			}
			pw.println("GET " + file + " HTTP/1.1");
			pw.println("Host: localhost:8080");
			pw.println("Connection: Close");
			pw.println();

			boolean loop = true;
			int counter = 0;

			while(loop){
				try{
					if(br.ready()){
						int i = 0;

						// skip over header
						while(i != -1){
							i = br.read();
							if((char)i == '\n') ++counter;
							else if((char)i == '\r') ++counter;
							else counter = 0;
							if(counter == 4) break;
						}
						post("skipped over header successfully");
						while(i != -1){
							i = br.read();
							buffer.put((byte)i);
						}
						loop = false;
					}
				}catch(Exception e){
					error("net_download: an exception was thrown while trying to get header from " + file);
					e.printStackTrace();
					return null;
				}
				try{Thread.currentThread().sleep(50);}
				catch(Exception e){
					e.printStackTrace();
					return null;
				}
			}
			return buffer.array();
		}

		private String requestHeader(String file){
			if(verbose){
				post("getting file " + file + " from host " + url.getHost());
				post("HEAD " + file + " HTTP/1.1");
				post("Host: localhost:8080");
				post("Connection: Close");
			}
			pw.println("HEAD " + file + " HTTP/1.1");
			pw.println("Host: localhost:8080");
			pw.println("Connection: Close");
			pw.println();

			boolean loop = true;
			StringBuffer sb = new StringBuffer(8096);
			while(loop){
				try{
					if(br.ready()){
						int i = 0;
						while(i != -1){
							i = br.read();
							sb.append((char)i);
						}
						loop = false;
					}
				}catch(Exception e){
					error("net_download: an exception was thrown while trying to get header from " + file);
					e.printStackTrace();
					return null;
				}
				try{Thread.currentThread().sleep(50);}
				catch(Exception e){
					e.printStackTrace();
					return null;
				}
			}
			try{br.reset();}
			catch(Exception e){
				e.printStackTrace();
			}
			return sb.toString();
		}

		private void printHeader(nd_Header h){
			Iterator<String> it = h.keySet().iterator();
			while(it.hasNext()){
				String k = it.next();
				post(k + ": " + h.get(k));
			}
		}
	}

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

	public void bang(){

		download("http://stereo-ssc.nascom.nasa.gov/data/beacon/ahead/impact/2008/07/STA_LB_IMPACT_20080728_V02.cdf");

		try{socket = new Socket(url.getHost(), 80);}
		catch(Exception e){
			error("net_download: couldn't find host " + url.getHost());
			return;
		}

		nd_Client client = new nd_Client(socket, url, true);
	}

	public void notifyDeleted(){
		willDelete = true;
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
