//clientB.cpp   codes are all from beej's book.

#include<stdio.h>
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>

using namespace std;
#define localhost "127.0.0.1"
#define PORTNUMBER "26810"
#define DELIMITER_FOR_ITEM " "
#define SIZE 10000




int main(int argc, char* argv[]){
    
    //prepare the inputs to be sent.
    if(argc<2){
        printf("Error: No input.\n");
        exit(0);
    }
    string nameBs;
    for(int i=1;i<argc;i++){
        nameBs+=argv[i];
        if(i<argc-1){
            nameBs+=DELIMITER_FOR_ITEM;
        }
    }
    
    //variables
    struct addrinfo hints, *res;  //from beej's book
    int socketB=0;
    int numbytesrecv=0;
    char buf[SIZE];
    
    //set up TCP connection socket
    memset(&hints,0,sizeof hints);  //from beej's book
    hints.ai_family=AF_UNSPEC;     //from beej's book
    hints.ai_socktype=SOCK_STREAM;  //from beej's book
    
    getaddrinfo(localhost,PORTNUMBER,&hints,&res);  //from beej's book
    if((socketB=socket(res->ai_family,res->ai_socktype,res->ai_protocol))==-1){
        printf("%s\n",strerror(errno));
    } //from beej's book
    printf("The client is up and running.\n\n");
    
    //connect to the serverC and send the data.
    if(connect(socketB,res->ai_addr, res->ai_addrlen)==-1){
        printf("%s\n",strerror(errno));
    }
    if(send(socketB,nameBs.data(),nameBs.size(),0)==-1){
        printf("%s\n",strerror(errno));
    }
    printf("The client sent %s to the Central server.\n",nameBs.c_str());
    
    //receive result from serverC and print it to the standard output.
    numbytesrecv=recv(socketB,buf,SIZE-1,0);
    buf[numbytesrecv]='\0';
    printf("%s",buf);
    close(socketB);
    return 0;
}


