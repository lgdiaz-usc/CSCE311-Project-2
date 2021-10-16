/*
    Takes in file contents from p1 and a target word
    and sends the lines that contains the target word
    back to p1.
*/
#include <iostream>
#include <vector>
#include <string.h>
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
    //Argument check
    if(argc != 2){
        cout << "Usage: p2 [TARGET]" << endl;
        return 0;
    }
    cout << "Searching for target: " << argv[1] << endl;

    //Socket setup
    struct sockaddr_un name;
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    char buffer[BUFFER_SIZE];

    //Create socket
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if(connection_socket == -1){
        Error("socket");
    }

    //Bind socket to socket name
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    ret = bind(connection_socket, (const struct sockaddr *) &name, sizeof(name));
    if(ret == -1){
        Error("bind");
    }

    //Prepare for accepting connections
    int backlog = 20;
    ret = listen(connection_socket, backlog);
    if(ret == -1){
        Error("listen");
    }

    //Main loop for handling connections
    while(true){
        //Waiting for incoming connection
        data_socket = accept(connection_socket, NULL, NULL);
        if(data_socket == -1){
            Error("accept");
        }

        vector<char*> output = {};
        //Handling connection
        while(true){
            //Waiting for next data packet
            ret = read(data_socket, buffer, sizeof(buffer));
            if(ret == -1){
                Error("read");
            }

            //Ensure buffer is 0 terminated
            buffer[sizeof(buffer) - 1] = 0;

            //Handle commands
            if(!strncmp(buffer, "DOWN", sizeof(buffer))){
                down_flag = 1;
                break;
            }
            if(!strncmp(buffer, "END", sizeof(buffer))){
                break;
            }

            //Find strings
            char* test = new char[sizeof(buffer)];
            for(int i=0; i < sizeof(buffer); i++){
                test[i] = buffer[i];
            }
            char* ptr = strtok(buffer, " ");
            while(ptr != NULL){
                if(!strncmp(ptr, argv[1], sizeof(ptr))){
                    output.push_back(test);
                    break;
                }
                ptr = strtok(NULL, " ");
            }
        }

        //Send result
        for(char* line : output){
            ret = write(data_socket, line, sizeof(buffer));
            if(ret == -1){
                Error("write");
            }
        }

        //Tell client it's done sending data
        strcpy(buffer, "END");
        ret = write(data_socket, buffer, strlen(buffer) + 1);
        if (ret == -1){
            Error("write");
        }

        //Close socket
        close(data_socket);

        //Quit on DOWN command
        if(down_flag){
            break;
        }
    }

    close(connection_socket);

    //Unlink the socket
    unlink(SOCKET_NAME);

    return 0;
}