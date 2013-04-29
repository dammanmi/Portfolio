/* 
 * ChildThread.java
 */


import java.io.*;
import java.net.*;
import java.util.Vector;

public class ChildThread extends Thread 
{
    static  Vector<ChildThread> handlers = new Vector<ChildThread>(20);		// static variables store values across all ChildThreads
	static BufferedReader br = null;			// for file reading
	static BufferedWriter bw = null;			// for file writing
	static String msgArray[] = new String[20];	// holds messages
	static int msgCount = 0;
	static String[] userArray = {"root", "john", "david", "mary"};
	static String[] pwArray = {"root01", "john01", "david01", "mary01"};
	static boolean shutdown;			// shutdown command boolean
	
    private Socket socket;													// non-static variables
    private BufferedReader in;
    private PrintWriter out;
	int msgIndex = 0;
	String userID;
	InetAddress userIP;
	
    public ChildThread(Socket socket) throws IOException 					// ChildThread constructor
    {
	this.socket = socket;
	in = new BufferedReader(
	    new InputStreamReader(socket.getInputStream()));
	out = new PrintWriter(
	    new OutputStreamWriter(socket.getOutputStream()));
	
	if(bw == null)															// run once for first ChildThread created
	{
		br = new BufferedReader(new FileReader("MSG.txt"));					// create file reader/writer
		bw = new BufferedWriter(new FileWriter("MSG.txt", true));
		String currentLine;
		for(;(currentLine = br.readLine()) != null; msgCount++)				// read messages from file
			msgArray[msgCount] = currentLine;
		br.close();
		shutdown = false;													// initiate shutdown as false
	}
	
	userID = "";
	userIP = socket.getInetAddress();		// set IP address
    }

    public void run() 
    {
	String line;
	synchronized(handlers) 
	{
	    // add the new client in Vector class
	    handlers.addElement(this);
	}

	try 
	{
	    while ((line = in.readLine()) != null) 					// loop this thread and read client input
	    {		
		if(line.contains("MSGGET"))								// MSGGET command received
	    {
			this.out.println("200 OK");
	    	this.out.println(msgArray[msgIndex++]);				// send next message from the array
	    	this.out.flush();
	    	if(msgIndex == msgCount)
	    		msgIndex = 0;
	    }
		
	    else if(line.contains("MSGSTORE"))						// MSGSTORE command received
	    {
		    if(this.userID != "")								// if client is logged in
		    {
	    		this.out.println("200 OK");
	    		this.out.flush();
		    	line = in.readLine();							// read message to store
		    	msgArray[msgCount++] = line;					// store new message in array
		    	line = "\n" + line;
		    	bw.write(line);									// append message to file
		    	this.out.println("200 OK");		    	
		    }	
		    else												// client not logged in
		    	this.out.println("401 You are not currently logged in, login first");
		    this.out.flush();
	    }
		
	    else if(line.contains("SHUTDOWN"))						// SHUTDOWN command received
	    {
	    	if(this.userID == "root")							// if client is root
	    	{
	    		shutdown = true;
	    		this.out.println("200 OK");
	    		for(int i = 0; i < handlers.size(); i++) 		// notify all clients of shutdown
	    		{	
	    		    synchronized(handlers) 
	    		    {
	    			ChildThread handler = (ChildThread)handlers.elementAt(i);
	    			handler.out.println("210 the server is about to shutdown......");
	    			handler.out.flush();
	    		    }
	    		}
	    		bw.close();										// close file writer
	    		System.exit(0);
	    		break;
	    	}
	    	else
	    		this.out.println("402 User not allowed to execute this command");	// if client is NOT root
	    	this.out.flush();
	    }
		
	    else if(line.contains("LOGIN"))							// LOGIN command received
	    {
	    	for(int i=0; i<4; i++)								// loop through userIDs and passwords to verify input
	    	{
	    		if(line.contains(" " + userArray[i] + " ") && line.contains(" " + pwArray[i]))	// if valid login
	    		{
	    			this.out.println("200 OK");
	    			this.out.flush();
	    			userID = userArray[i];
	    			if(line.contains("root"))
	    			break;
	    		}
	    	}
	    	if(userID == "")									// if invalid login
	    		this.out.println("410 Wrong UserID or Password");
	    	this.out.flush();
	    }
		
	    else if(line.contains("LOGOUT"))						// LOGOUT command received
	    {
	    	this.out.println("200 OK");
	    	this.out.flush();
	    	userID = "";		    							// set userID to null
	    }
		
	    else if(line.contains("QUIT"))							// QUIT command received
	    {
	    	this.out.println("200 OK");
	    	this.out.flush();
	    	break;
	    }
		
	    else if(line.contains("WHO"))							// WHO command received
	    {
	    	this.out.println("200 OK");
	    	this.out.flush();
	    	String userList  = "The list of active users:";
	    	for(int i=0; i < handlers.size(); i++)
	    	{
	    		ChildThread handler =
	    			    (ChildThread)handlers.elementAt(i);
	    		userList += '\n' + handler.userID + '\t' + handler.userIP;	// make one string of user IDs and IP addresses to output
	    	}
	    	this.out.println(userList);
	    	this.out.flush();
	    }
		
	    else if(line.contains("SEND "))
	    {
	    	ChildThread handler = handlers.elementAt(0);		
	    	String name = line.substring(5);								// extract user ID from client input
	    	boolean userFound = false;
	    	for(int i=0; i < handlers.size(); i++)							// check against all active clients
	    	{
	    		handler = (ChildThread)handlers.elementAt(i);
	    		if(name.contains(handler.userID))
	    		{
	    			userFound = true;
	    			break;
	    		}
	    	}
	    	if(userFound)													// match found
	    	{
		    	this.out.println("200 OK");
		    	this.out.flush();
		    	line = in.readLine();
		    	this.out.println("200 OK");
		    	this.out.flush();
    			String message = "You have a new message from " + this.userID	// send message
    					+ "\n" + this.userID + ": " + line;
    			handler.out.println(message);
    			handler.out.flush();
	    	}
	    	else															// no match found
	    	{
	    		this.out.println("420 Either the user does not exist or is not logged in");
	    		this.out.flush();
	    	}
	    }
	    line = "";
	    }
	} 
	catch(IOException ioe) 
	{
	    ioe.printStackTrace();
	} 
	finally 
	{
		synchronized(handlers) 
		{
		    handlers.removeElement(this);
		}
	    try 
	    {
		in.close();															// close buffered reader/writer and socket
		out.close();
		socket.close();
	    } 
	    catch(IOException ioe) 
	    {
	    } 
	}
    }
}

