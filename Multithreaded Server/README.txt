Michael Damman
CIS 427
Fall 2012
Project 2: Multi Threaded Server


How to run:

To run the server, the following files must be located inside the same directory in the UMD SSH: 

MultiThreadServer.java
Client.java
ChildThread.java
Makefile, MSG.txt.

Open at least 2 SSH terminals, and in one of them, execute the command "make".  This will build all three java files.  Then, in one termanal (the server), execute the following command:

java MultiThreadServer

In all other terminals, execute this command: (I tried multiple IP addresses, including my own and ones given in class, this is the only that would work for me).

java Client 127.0.0.1

The server terminal will output that new connections have been formed with each of the clients.  Up to 20 clients may connect with the server at a time.



Commands Implemented:

The following commands may be entered from a client terminal.

MSGGET: This command returns the next saved message of the day from the server and outputs it to the client's terminal.


MSGSTORE: This command checks if the client is logged in.  If they are, the client may then enter a message to be saved on the server.

LOGIN: This user enters the command "LOGIN username password" to log in to the server.  If the username and password match with a username and password saved on the server, the server logs the user's client in under that name.

LOGOUT: This command removes the client's logged in status.

QUIT: This command closes the client's connection to the server.

SHUTDOWN: This command notifies all connected clients that the server is shutting down.  The clients close, then the server shuts itself down.

WHO: This command compiles a list of all clients names and IP addresses that are connected to the server and outputs it to the client that called it.

SEND: This user enters a command of the form "SEND username".  The server checks if the username matches with a currently logged in client.  If it does, the server notifies the user who then enters a message to send to that client.  The server then delivers the message to the client.



Known Bugs:

This is not necessarily a bug, but a coding decision that could result in some invalid commands being accepted.  For most string comparisons on the Client and ChildThread sides, I used the function string.contains().  I did this because I could not find another reliable way to test that two strings contain the same message.  I tried both (string == "mystring") and (string == "mystring\n") and could not get them to match every time.  String.contains() works for all valid commands, and some "mostly valid" commands so that is what I went with.



Sample Outputs:

MSGGET:

c:MSGGET
s:200 OK
s:Test Message 1
c:MSGGET
s:200 OK
s:Test Message 2
c:MSGGET
s:200 OK
s:Test Message 3
c:MSGGET
s:200 OK
s:Test Message 4
c:MSGGET
s:200 OK
s:Test Message 5
c:MSGGET
s:200 OK
s:Test Message 1


MSGSTORE, LOGIN, and LOGOUT:

c:MSGSTORE
s:401 You are not currently logged in, login first
c:LOGIN david david1
s:410 Wrong UserID or Password
c:LOGIN david david01
s:200 OK
c:New Message 1
s:200 OK
c:MSGGET
s:200 OK
s:New Message 1
c:LOGOUT
s:200 OK
c:MSGSTORE
s:401 You are not currently logged in, login first


QUIT

c:QUIT
s:200 OK
-client returns to command line prompt
-server remains active


SHUTDOWN:

c:SHUTDOWN
s:402 User not allowed to execute this command
c:LOGIN root root01
s:200 OK
c:SHUTDOWN
s:the server is about to shutdown...(to all clients)
-all clients exit to command line
-server exits to command line


WHO and SEND:

c:WHO
s:200 OK
s:The list of active users:
s:john	/127.0.0.1
s:mary	/127.0.0.1
s:david	/127.0.0.1
c:SEND brian
s:420 Either the user does not exist or is not logged in
c:SEND mary
s:200 OK
c:heeeeey mary
s:200 OK

(at mary's terminal)
s:You have a new message from john
s:john:heeeeey mary
c:SEND john
s:200 OK
c:heeeeeeeeeeeey john
s:200 OK

(back at john's terminal)
s:You have a new message from mary
s:mary:heeeeeeeeeeeey john
