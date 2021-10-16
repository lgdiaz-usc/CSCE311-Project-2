/*
    Takes in a file and sends it's contents to p2.
*/
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.h"

using namespace std;

void Error(char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv){
    //Argument Check
    if(argc != 2){
        cout << "Usage: p1 [FILE]" << endl;
        return 0;
    }
    cout << "Searching in file: " << argv[1] << endl;
    vector<char*> input = {};

    //Reading input file
    fstream file;
    file.open(argv[1], ios::in);
    if(file.is_open()){
        string line;
        while(getline(file, line)){
            char* Cline = new char[line.length()];
            for(int i=0; i < line.length(); i++){
                Cline[i] = line[i];
            }
            input.push_back(Cline);
        }
        file.close();
    }

    //Socket Setup
    struct sockaddr_un addr;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];

    //Create local socket
    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(ret == -1){
        Error("socket");
    }

    //Connect socket to socket address
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(addr));
    if(ret == -1){
        Error("The server is down.");
    }

    //send arguments
    for(char* line : input){
        ret = write(data_socket, line, strlen(line) + 1);
        if(ret == -1){
            Error("write");
        }
    }

    //Request result
    strcpy(buffer, "END");
    ret = write(data_socket, buffer, strlen(buffer) + 1);
    if (ret == -1){
        Error("write");
    }

    //Receive result
    while(true){
        //Waiting for next data packet
        ret = read(data_socket, buffer, sizeof(buffer));
        if(ret == -1){
            Error("read");
        }

        //Ensure buffer is 0 terminated
        buffer[sizeof(buffer) - 1] = 0;

        //Handle commands
        if(!strncmp(buffer, "END", sizeof(buffer))){
            break;
        }
        cout << buffer << endl;
    }

    //Close socket
    close(data_socket);

    return 0;
}