
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>

using namespace std;
#define localhost "127.0.0.1"
#define PORTNUMBER "25810"
#define SIZE 10000


int main(int argc, char* argv[]){
    
    if(argc<2){
        printf("Error: No input.\n");
        exit(0);
    }
    
    int socketA=0;
    struct addrinfo hints, *res; // from beej's book
    char buf[SIZE];
    int numbytesrecv=0;
    
    //set up TCP connection socket
    memset(&hints,0,sizeof hints); //from beej's book
    hints.ai_family=AF_UNSPEC;    //from beej's book
    hints.ai_socktype=SOCK_STREAM;  //from beej's book
    
    getaddrinfo(localhost,PORTNUMBER,&hints,&res);  //from beej's book
    if((socketA=socket(res->ai_family,res->ai_socktype,res->ai_protocol))==-1){
        printf("%s\n",strerror(errno));
    }  //from beej's book
    printf("The client is up and running.\n\n");
    
    //connect to serverC and send input.
    if(connect(socketA,res->ai_addr,res->ai_addrlen)==-1){
        printf("%s\n",strerror(errno));
    }
    if(send(socketA,argv[1],strlen(argv[1]),0)==-1){
        printf("%s\n",strerror(errno));
    }
    printf("The client sent %s to the Central server.\n",argv[1]);
    freeaddrinfo(res);
    
    //receive result from serverC and print it to the standard output.
    numbytesrecv=recv(socketA,buf,SIZE-1,0);  //from beej's book
    buf[numbytesrecv]='\0';
    printf("%s",buf);
    close(socketA);
    return 0;
    
}






