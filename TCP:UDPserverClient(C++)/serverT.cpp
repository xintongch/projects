//serverT.cpp

#include<fstream>
#include<vector>
#include<cstring>
#include<queue>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sstream>
#include<string.h>
#include<errno.h>
#include<sys/wait.h>
#include "graph.h"

using namespace std;
#define localhost "127.0.0.1"
#define PORTNUMBER "21810"
#define SERVERC_PORTNUMBER "24810"
#define MAX_SENDING_PACKET_SIZE 8000
#define LENGTH_PADDING_TO 8
#define DELIMITER_FOR_SECTION ";"
#define DELIMITER_FOR_BLOCK "|"
#define DELIMITER_FOR_ITEM " "
#define FILENAME "edgelist.txt"


//global variables
struct addrinfo hints, *res;
int listensocketT=0;
sockaddr_in client_addr;
socklen_t addr_len=sizeof(client_addr);
int numbytesrecv;
char* buf;
string nameA;
vector<string> nameBs;
string result;
int nameAID=0;
int nameAcomponentIndex=0;
bool foundconnection=false;



/**
 * @brief This function is for setting up the addrinfo variable.
 * This block of codes is copied from Beej's book and modified, mainly "memset()";
 */
void hintsinitialiazation(){
    
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_DGRAM;
}


/**
 * @brief This function is for construction a UDP listening socket for serverS.
 * This block of codes is copied from Beej's book and modified, mainly "getaddrinfo(),socket(),bind(),freeaddrinfo()";
 */
void constructlistensocket(){
    getaddrinfo(localhost,PORTNUMBER,&hints,&res);
    if((listensocketT=socket(res->ai_family,res->ai_socktype,res->ai_protocol))==-1){
        printf("%s\n",strerror(errno));
    }
    if(bind(listensocketT,res->ai_addr,res->ai_addrlen)==-1){
        printf("%s\n",strerror(errno));
    }
    freeaddrinfo(res);
}



/**
 * @brief This function is for receiving data from serverC and extract target names from the data.
 * This function contains codes copied from Beej's book and modified, mainly "recvfrom()";
 */
void recvandextracttargetnames(){
    string namestring;
    string nameB;
    
    buf=new char[MAX_SENDING_PACKET_SIZE+1];
    numbytesrecv=recvfrom(listensocketT,buf,MAX_SENDING_PACKET_SIZE,0,(struct sockaddr*)&client_addr, &addr_len);
    buf[numbytesrecv]='\0';
    printf("The ServerT received a request from Central to get the topology.\n");
    
    namestring=buf;
    istringstream namestream(namestring);
    namestream>>nameA;
    while(namestream>>nameB){
        nameBs.push_back(nameB);
    }
    delete[] buf;
}



/**
 * @brief This function is for converting an integer to a string
 * @param i the integer to be converted.
 * @return string the string after the conversion.
 */
string intToString(int i){
    char temp[100];
    sprintf(temp,"%d",i);
    string tempstring=temp;
    return tempstring;
}



/**
 * @brief This function is for checking if any nameBs (input from clientB) is in the same component as input from clientA
 * (which means can find path to nameA). If so, add the nameB's ID to the sent back data, otherwise, mark the position with
 * a "NULL" in the sent back data.
 * @param myGraph
 * @return string
 */
string searchforconnection(graph myGraph){
    string result;
    nameAID=myGraph.getnameID(nameA);
    result+=(intToString(nameAID)+DELIMITER_FOR_ITEM);
    nameAcomponentIndex=myGraph.getcomponent(nameAID);
    for(int i=0;i<nameBs.size();i++){
        if(!myGraph.find(nameBs[i])){
            result+="NULL";
            result+=DELIMITER_FOR_ITEM;
        }else{
            int nameBID=myGraph.getnameID(nameBs[i]);
            if(nameAcomponentIndex!=myGraph.getcomponent(nameBID)){
                result+="NULL";
                result+=DELIMITER_FOR_ITEM;
            }else{
                foundconnection=true;
                result+=(intToString(nameBID)+DELIMITER_FOR_ITEM);
            }
        }
    }
    return result;
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
 * @brief This function is for sending data to serverC
 * This function contains codes copied from Beej's book and modified, mainly "getaddrinfo(),sendto(),freeaddrinfo()";
 * @param result contains information sent back to serverC.
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
            if(sendto(listensocketT,result.data(),result.size(),0,res->ai_addr,res->ai_addrlen)==-1){
                printf("%s\n",strerror(errno));
            }          //copied from Beej's book
        }else{
            if(sendto(listensocketT,result.data(),MAX_SENDING_PACKET_SIZE,0,res->ai_addr,res->ai_addrlen)==-1){
                printf("%s\n",strerror(errno));
            }          //copied from Beej's book
            result=result.substr(MAX_SENDING_PACKET_SIZE);
        }
        sendingtimes--;
    }
    printf("The ServerT finished sending the topology to Central.\n");
    freeaddrinfo(res);
}



/**
 * @brief This function is for resetting global variables at the beginning of each iteration of the while loop.
 */
void resetvariables(){
    vector<string>().swap(nameBs);
    foundconnection=false;
}





int main(){
    
    //construct a graph object initialized by the provided edgelist file.
    graph myGraph(FILENAME);
    
    //set up a UDP listening socket for serverT
    hintsinitialiazation();
    constructlistensocket();
    printf("The ServerT is up and running using UDP on port %s.\n",PORTNUMBER);
    
    while(true){
        try{
            string result;
            string relatedIDnames;
            
            //receive data from serverC and extract target names from the data
            resetvariables();
            recvandextracttargetnames();
            
            //if the name from clientA (nameA) is not even in the database graph. then the sent back data
            //contains only a "NULL".
            //otherwise check if any name from clientB is in the same component as nameA, if none, the sent
            //back data would contains corresponding more "NULL".
            //otherwise, add IDs of the target name and adjacent lists of all IDs in the target componnet and
            //a corresponding ID-to-name mapping.
            if(!myGraph.find(nameA)){
                result+="NULL";
                result+=DELIMITER_FOR_SECTION;
            }
            else{
                result=searchforconnection(myGraph);
                result+=DELIMITER_FOR_SECTION;
                if(foundconnection){
                    for(int i=0;i<myGraph.getnamecount();i++){
                        if(myGraph.getcomponent(i)==nameAcomponentIndex){
                            relatedIDnames+=(intToString(i)+DELIMITER_FOR_ITEM);
                            relatedIDnames+=(myGraph.getIDname(i)+DELIMITER_FOR_ITEM);
                            result+=(intToString(i)+DELIMITER_FOR_ITEM);
                            for(int j=0;j<myGraph.getadjacentlistsize(i);j++){
                                result+=(intToString(myGraph.getadjacentnameID(i,j))+DELIMITER_FOR_ITEM);
                            }
                            result+=DELIMITER_FOR_BLOCK;
                        }
                    }
                    result+=(DELIMITER_FOR_SECTION+relatedIDnames);
                }
            }
            //send the result data back to serverC.
            sendtocentral(result);
        }
        catch(int a){
            continue;
        }
    }
    close(listensocketT);
    return 0;
}










