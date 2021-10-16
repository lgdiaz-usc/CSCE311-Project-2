# CSCE311-Project-2
These two programs use a Unix domain socket to pass imformation between each other. First p2 is run with a single argument, being a word to look for in it's inputs. p2 sets up a domain socket and, acting like a server, waits for a client program to connect to it. Then, p1 is run, also with a single argument, being a path to a file to search for a word in. p1 then reads the file and stores each line in a cstring array. p1 then connects to p2's socket and sends each line in the array to p2 through the socket one by one. p2 reads each line sent to it by p1 and checks if it contains the target word. If a line has a match, it is stored in another cstring array. When p1 has sent all of it's lines, it sends a signal to p2, telling itthat it has finished. After sorting through all of the lines, p2 then sends the matching lines back to p1, which prints them to the screen upon arrival. Once p2 has sent all of its lines, it signals p1 that it has finished, prompting p1 to close the connection and exit. p2 waits for another client to connect and doesn't close unless an instance of p1 is called with the included file "quit" has the argument.

Compilation/Run
1. cd /path/to/code/
2. make
3. ./p2 "\[TARGET\]"
4. ./p1 "\[FILE\]"

NOTE
Do NOT close p2 with Ctrl+C.
Use "./p1 ./quit", instead.
If you do it anyway, than edit the variable SOCKET_NAME in connection.h and recompile.