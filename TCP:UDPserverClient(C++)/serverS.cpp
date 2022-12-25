//serverS.cpp


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<netdb.h>
#include<fstream>
#include<sstream>
#include<string.h>
#include "scores.h"


using namespace std;

#define localhost "127.0.0.1"
#define PORTNUMBER "22810"
#define SERVERC_PORTNUMBER "24810"
#define MAX_SENDING_PACKET_SIZE 8000
#define LENGTH_PADDING_TO 8
#define DELIMITER_FOR_ITEM " "
#define FILENAME "scores.txt"

struct addrinfo hints, *res;
int listensocketS=0;
struct sockaddr_storage client_addr;
socklen_t addr_len=sizeof(client_addr);
char* buf;
int numbytesrecv=0;



/**
 * @brief This function is for setting up the addrinfo variable.
 * This block of codes is copied from Beej's book and modified, mainly "memset()";
 */
void hintsinitialization(){
    memset(&hints, 0, sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_DGRAM;
}


/**
 * @brief This function is for construction a UDP listening socket for serverS.
 * This block of codes is copied from Beej's book and modified, mainly "getaddrinfo(),socket(),bind(),freeaddrinfo()";
 */
void constructlistensocket(){
    getaddrinfo(localhost,PORTNUMBER,&hints,&res);
    if((listensocketS=socket(res->ai_family,res->ai_socktype,res->ai_protocol))==-1){
        printf("%s\n",strerror(errno));
    }
    if(bind(listensocketS,res->ai_addr,res->ai_addrlen)==-1){
        printf("%s\n",strerror(errno));
    }
    freeaddrinfo(res);
}


/**
 * @brief This function is for receving data from serverC and remove the sending times information in the header.
 */
void recvfromcentral(){
    buf=new char[MAX_SENDING_PACKET_SIZE+1];
    
    string tempstring;
    int recvtimes;
    
    numbytesrecv=recvfrom(listensocketS,buf,MAX_SENDING_PACKET_SIZE,0,(struct sockaddr*)&client_addr,&addr_len);   //copied from Beej's book
    buf[numbytesrecv]='\0';
    tempstring=buf;
    istringstream tempstream(tempstring);
    tempstream>>recvtimes;
    for(int i=LENGTH_PADDING_TO;i<=numbytesrecv;i++){
        buf[i-LENGTH_PADDING_TO]=buf[i];
    }
    while(recvtimes>1){
        char tempbuf[MAX_SENDING_PACKET_SIZE+1];
        numbytesrecv=recvfrom(listensocketS,tempbuf,MAX_SENDING_PACKET_SIZE,0,(struct sockaddr*)&client_addr,&addr_len);   //copied from Beej's book
        tempbuf[numbytesrecv]='\0';
        char *combinedArray=new char[strlen(buf)+strlen(tempbuf)+1];
        strcpy(combinedArray,buf);
        strcat(combinedArray,tempbuf);
        delete[] buf;
        buf=combinedArray;
        buf[strlen(buf)]='\0';
        recvtimes--;
    }
    printf("The ServerS received a request from Central to get the scores.\n");
}



/**
 * @brief This funciton is for converting an integer to a string
 * @param i the integer to be converted.
 * @return string the string after conversion.
 */
string intToString(int i){
    char tempArray[100];
    string tempString;
    
    sprintf(tempArray,"%d",i);
    tempString=tempArray;
    return tempString;
}




/**
 * @brief This function is for adding a fix length header to the front of a string of data.
 * @param header the header to be added to the front of the data string
 * @param data the data to be added a header
 * @return string the string after catenating the header and the data.
 */
string addHeader(string header,string data){
    int headerSize=header.size();
if(header.size()>LENGTH_PADDING_TO-1){
            printf("error: the message sent to serverC is larger than 80 billion bytes exceeding default setting limit.\n");
        throw 0;
    }else{
        for(int i=0;i<LENGTH_PADDING_TO-headerSize;i++){
            header+=DELIMITER_FOR_ITEM;
        }
    }
    header+=data;
    data=header;
    return data;
}



/**
 * @brief This function is for sending result to serverC.
 * This block of codes is copied from Beej's book and modified, mainly "getaddrinfo(), sendto(), freeaddrinfo()";
 * @param result contains the data to be sent to serverC
 */
void sendtocentral(string result){
    int resultsize=result.size()+LENGTH_PADDING_TO;
    int sendingtimes=resultsize/MAX_SENDING_PACKET_SIZE;
    
    if(MAX_SENDING_PACKET_SIZE*sendingtimes<resultsize){
        sendingtimes++;
    }
    result=addHeader(intToString(sendingtimes),result);
    
    getaddrinfo(localhost,SERVERC_PORTNUMBER,&hints,&res);               //copied from Beej's book
    while(sendingtimes>0){
        if(sendingtimes==1){
            if(sendto(listensocketS,result.data(),result.size(),0,res->ai_addr,res->ai_addrlen)==-1){
                printf("%s\n",strerror(errno));
            }          //copied from Beej's book
        }else{
            if(sendto(listensocketS,result.data(),MAX_SENDING_PACKET_SIZE,0,res->ai_addr,res->ai_addrlen)==-1){
                printf("%s\n",strerror(errno));
            }          //copied from Beej's book
            result=result.substr(MAX_SENDING_PACKET_SIZE);
        }
        sendingtimes--;
    }
    printf("The ServerS finished sending the scores to Central.\n");
    freeaddrinfo(res);
}






int main(){
    
    //construct a score object initialized by the provided file.
    scores myScores(FILENAME);
    
    //set up a UDP listening socket for serverS.
    hintsinitialization();
    constructlistensocket();
    printf("The ServerS is up and running using UDP on port %s.\n",PORTNUMBER);
    
    while(true){
        
        try{
            //variables
            string bufstring;
            string ID;
            string name;
            string result;
            
            //receive data from serverC.
            recvfromcentral();
            
            //extract name from the received data and get its score from the score object.
            bufstring=buf;
            istringstream bufstream(bufstring);
            while(bufstream>>ID){
                bufstream>>name;
                result+=(ID+DELIMITER_FOR_ITEM);
                result+=(myScores.getScore(name)+DELIMITER_FOR_ITEM);
            }
            //send back a mapping of ID and score to severC
            sendtocentral(result);
            delete[] buf;
        }
        catch(int a){
            continue;
        }
    }
    close(listensocketS);
    return 0;
}





