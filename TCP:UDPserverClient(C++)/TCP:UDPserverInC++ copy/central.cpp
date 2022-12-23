//central.cpp
// for serverC

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<netdb.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sstream>
#include<vector>
#include<string.h>
#include<cstring>
#include<sys/wait.h>


using namespace std;

#define localhost "127.0.0.1"
#define SERVERT_PORTNUMBER "21810"
#define SERVERS_PORTNUMBER "22810"
#define SERVERP_PORTNUMBER "23810"
#define UDP_PORTNUMBER "24810"
#define CLIENTA_PORTNUMBER "25810"
#define CLIENTB_PORTNUMBER "26810"
#define MAX_SENDING_PACKET_SIZE 8000
#define LENGTH_PADDING_TO 8
#define DELIMITER_FOR_SECTION ";"  // these three delimiters has a top down hierarchy. The topmost level
#define DELIMITER_FOR_BLOCK "|"    //the middle level
#define DELIMITER_FOR_ITEM " "     //the lowest level
#define BACKLOG 10


// global variables
//variables related to socket programming
struct addrinfo hints, *res;    // for storing a socket's type and address information altogether
int listensocketforA=0;
int listensocketforB=0;
int listensocketUDP=0;           // for all UDP related information exchanged (for all serverT, serverS, serverP communication)
int acceptsocketforA=0;          // only needed when accepting a TCP connection
int acceptsocketforB=0;          // only needed when accepting a TCP connection
struct sockaddr_storage client_addr;
socklen_t addr_len=sizeof(client_addr);
int numbytesrecv=0;
char* buf;

//variables related to data processing.
string nameA;
vector<string> targetnames;
vector<string> targetIDs;
bool foundconnection=false;
vector<bool> found;
string adjacentlists;
string relatednames;
int nameBcount=0;


/**
 * @brief
 * This global function is for setting up IP address version and socket type for the addrinfo variable hints.
 * This block of codes is copied form Beej's book and modified, mainly "memset()"
 * @param protocalname indicates whether to set up a addrinfo structure for TCP or UDP.
 */
void hintsinitialization(const char* protocalname){
    
    memset(&hints,0,sizeof(hints));   //copied
    hints.ai_family=AF_UNSPEC;        //copied
    if(strcmp(protocalname,"TCP")==0){
        hints.ai_socktype=SOCK_STREAM;  //copied
    }else if(strcmp(protocalname,"UDP")==0){
        hints.ai_socktype=SOCK_DGRAM;    //copied
    }
}


/**
 * @brief This global function is for building up TCP listening socket or UDP listening socket.
 * This block of codes is copied form Beej's book and modified, mainly "getaddrinfo(),socket(),bind(),listen(),freeaddrinfo()";
 * @param clientname indicates whether to build up a listening socket for client A or client B or a UDP socket.
 */
void listensocketconstruction(const char* clientname){
    if(strcmp(clientname,"A")==0){
        getaddrinfo(localhost,CLIENTA_PORTNUMBER,&hints,&res);    //copied
        if((listensocketforA=socket(res->ai_family,res->ai_socktype,res->ai_protocol))==-1){
            printf("%s\n",strerror(errno));
        }   //copied
        if(bind(listensocketforA,res->ai_addr,res->ai_addrlen)==-1){
            printf("%s\n",strerror(errno));
        }    //copied
        if(listen(listensocketforA,BACKLOG)==-1){
            printf("%s\n",strerror(errno));
        }                     //copied
    }else if(strcmp(clientname,"B")==0){
        getaddrinfo(localhost, CLIENTB_PORTNUMBER, &hints, &res);    //copied
        if((listensocketforB=socket(res->ai_family,res->ai_socktype,res->ai_protocol))==-1){
            printf("%s\n",strerror(errno));
        }  //copied
        if(bind(listensocketforB, res->ai_addr, res->ai_addrlen)==-1){
            printf("%s\n",strerror(errno));
        }          //copied
        if(listen(listensocketforB, BACKLOG)==-1){
            printf("%s\n",strerror(errno));
        }                            //copied
    }else if(strcmp(clientname,"UDP")==0){
        getaddrinfo(localhost,UDP_PORTNUMBER,&hints,&res);            //copied
        if((listensocketUDP=socket(res->ai_family,res->ai_socktype,res->ai_protocol))==-1){
            printf("%s\n",strerror(errno));
        }      //copied
        if(bind(listensocketUDP,res->ai_addr,res->ai_addrlen)==-1){
            printf("%s\n",strerror(errno));
        }             //copied
    }
    freeaddrinfo(res);                                                 //copied
}




/**
 * @brief This function is for accepting TCP connection from clients and extract inputs from received data.
 * This block of codes is copied from Beej's book and modified, mainly "accept(),recv()";
 * @param clientname indicates whether to accept TCP connetion from client A or client B.
 * @return string contains names sent from client B.
 */
string acceptTCPandextractnames(const char* clientname){
    string nameBs;
    buf=new char[MAX_SENDING_PACKET_SIZE+1];
    string temp;
    
    if(strcmp(clientname,"A")==0){
        if((acceptsocketforA=accept(listensocketforA, (struct sockaddr*)&client_addr, &addr_len))==-1){
            printf("%s\n",strerror(errno));
        }  //copied
        numbytesrecv=recv(acceptsocketforA,buf,MAX_SENDING_PACKET_SIZE,0);      //copied
        buf[numbytesrecv]='\0';                                //copied
        printf("The Central server received input=\"%s\" from the client using TCP over port %s.\n",buf,CLIENTA_PORTNUMBER);
        nameA=strtok(buf,DELIMITER_FOR_ITEM);
        targetnames.push_back(nameA);
    }else{
        if((acceptsocketforB=accept(listensocketforB,(struct sockaddr*)&client_addr,&addr_len))==-1){
            printf("%s\n",strerror(errno));
        }   //copied
        numbytesrecv=recv(acceptsocketforB,buf,MAX_SENDING_PACKET_SIZE,0);           //copied
        buf[numbytesrecv]='\0';                                   //copied
        printf("The Central server received input=\"%s\" from the client using TCP over port %s.\n",buf,CLIENTB_PORTNUMBER);
        string nameBstring=buf;
        istringstream nameBstream(nameBstring);
        while(nameBstream>>temp){
            targetnames.push_back(temp);
            nameBcount++;
            nameBs+=(temp+DELIMITER_FOR_ITEM);
        }
    }
    delete[] buf;
    return nameBs;
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
 * @brief Thisfunction is for sending information to servers.
 * This block of codes is copied from Beej's book and modified, mainly "getaddrinfo(),sendto(),freeaddrinfo()";
 * @param SERVER_PORTNUMBER indicates the portnumber of the receiving server.
 * @param data is a string containing the data to be sent to the server.
 * @param servername inidicates which server to send to, chosen from T, S, and P.
 */
void sendtoserver(const char* SERVER_PORTNUMBER,string result,const char* servername){
    getaddrinfo(localhost,SERVER_PORTNUMBER,&hints,&res);          //copied
    if(strcmp(servername,"T")==0){
        if(sendto(listensocketUDP,result.data(),result.size(),0,res->ai_addr,res->ai_addrlen)==-1){
            printf("%s\n",strerror(errno));
        }   //copied from Beej's book
        printf("The Central server sent a request to Backend-Server %s.\n",servername);
        
    }else{
        int resultsize=result.size()+LENGTH_PADDING_TO;
        int sendingtimes=resultsize/MAX_SENDING_PACKET_SIZE;
        
        if(MAX_SENDING_PACKET_SIZE*sendingtimes<resultsize){
            sendingtimes++;
        }
        result=addHeader(intToString(sendingtimes),result);
        
        while(sendingtimes>0){
            if(sendingtimes==1){
                if(sendto(listensocketUDP,result.data(),result.size(),0,res->ai_addr,res->ai_addrlen)==-1){
                    printf("%s\n",strerror(errno));
                }   //copied from Beej's book
            }else{
                if(sendto(listensocketUDP,result.data(),MAX_SENDING_PACKET_SIZE,0,res->ai_addr,res->ai_addrlen)==-1){
                    printf("%s\n",strerror(errno));
                }   //copied from Beej's book
                result=result.substr(MAX_SENDING_PACKET_SIZE);
            }
            sendingtimes--;
        }
        if(strcmp(servername,"S")==0){
            printf("The Central server sent a request to Backend-Server %s.\n",servername);
        }else{
            printf("The Central server sent a processing request to Backend-Server P.\n");
        }
    }
    freeaddrinfo(res);                                               //copied
}



/**
 * @brief Thisfunction is for receiving data from servers
 * This block of codes is copied from Beej's book and modified, mainly "recvfrom()";
 * @param servername inidicates which server the data is received from.
 * @param SERVER_PORTNUMBER indicates the portnumber of the server sending back information.
 */

void recvfromserver(const char* servername,const char* SERVER_PORTNUMBER){
    buf=new char[MAX_SENDING_PACKET_SIZE+1];
    
    string tempstring;
    int recvtimes;
    
    numbytesrecv=recvfrom(listensocketUDP,buf,MAX_SENDING_PACKET_SIZE,0,(struct sockaddr*)&client_addr,&addr_len);   //copied from Beej's book
    buf[numbytesrecv]='\0';
    tempstring=buf;
    istringstream tempstream(tempstring);
    tempstream>>recvtimes;
    for(int i=LENGTH_PADDING_TO;i<=numbytesrecv;i++){
        buf[i-LENGTH_PADDING_TO]=buf[i];
    }
    while(recvtimes>1){
        char tempbuf[MAX_SENDING_PACKET_SIZE+1];
        numbytesrecv=recvfrom(listensocketUDP,tempbuf,MAX_SENDING_PACKET_SIZE,0,(struct sockaddr*)&client_addr,&addr_len);   //copied from Beej's book
        tempbuf[numbytesrecv]='\0';
        char *combinedArray=new char[strlen(buf)+strlen(tempbuf)+1];
        strcpy(combinedArray,buf);
        strcat(combinedArray,tempbuf);
        delete[] buf;
        buf=combinedArray;
        buf[strlen(buf)]='\0';
        recvtimes--;
    }    
    if(strcmp(servername,"T")==0||strcmp(servername,"S")==0){
        printf("The Central server received information from Backend-Server %s using UDP over port %s.\n",servername,SERVER_PORTNUMBER);
    }else {
        printf("The Central server received the results from backend server P.\n");
    }
    
}



/**
 * @brief This function "extract target IDs from serverT data" is for extracting
 * the first section of information sent from serverT which are the IDs of the clients' input names.
 */
void extracttargetIDs(){
    string targetIDstring;
    targetIDstring=strtok(buf,DELIMITER_FOR_SECTION);
    istringstream targetIDstream(targetIDstring);
    string nameID;
    while(targetIDstream>>nameID){
        targetIDs.push_back(nameID);
    }
}



/**
 * @brief This function "search if any nameB connected to nameA" is for scanning the
 * the target IDs to see if if any client B's input name is in the same componet as client A's input
 * in the database graph of serverT. If it is, mark the position (memorize which ID).
 * @return string contains the IDs that are in the same component of the graph in serverT's databse.
 */
string searchforconnection(){
    string targetIDnotnull;
    for(int i=1;i<targetIDs.size();i++){
        if(targetIDs[i]!="NULL"){
            found.push_back(true);
            foundconnection=true;
            targetIDnotnull+=(targetIDs[i]+DELIMITER_FOR_ITEM);
        }else{
            found.push_back(false);
        }
    }
    return targetIDnotnull;
}




/**
 * @brief This function build up a string containing ouput results for the clients.
 * @param hint indicates whether input names from clients have any two of them that are in the
 * same component of the graph in the serverT's databse.
 * @return string as part of the result that is to be sent back to the clients.
 */
string* constructresultforclients(bool hint){
    string resultforclientA;
    string resultforclientB;
    vector<string> datablocks;
    int blockIndex=0;
    string temp;
    char* block;
    
    block=strtok(buf,DELIMITER_FOR_BLOCK);
    while(block!=NULL){
        temp=block;
        datablocks.push_back(temp);
        block=strtok(NULL,DELIMITER_FOR_BLOCK);
    }
    
    if(hint){
        for(int i=0;i<found.size();i++){
            if(found[i]==false){
                resultforclientA+=("Found no compatibility for "+nameA+" and "+targetnames[i+1]+" \n");
                resultforclientB+=("Found no compatibility for "+targetnames[i+1]+" and "+nameA+" \n");
            }else{
                resultforclientA+=(datablocks[blockIndex++]+" \n");
                resultforclientB+=(datablocks[blockIndex++]+" \n");
            }
        }
    }else{
        for(int i=0;i<nameBcount;i++){
            resultforclientA+=("Found no compatibility for "+nameA+" and "+targetnames[i+1]+" \n");
            resultforclientB+=("Found no compatibility for "+targetnames[i+1]+" and "+nameA+" \n");
        }
    }
    string *combinedresults=new string[2];
    combinedresults[0]=resultforclientA;
    combinedresults[1]=resultforclientB;
    return combinedresults;
}


/**
 * @brief This function is for sending the final result output to the clients.
 * This block of codes is copied from Beej's book and modified, mainly "send(),close()";
 * @param hint indicates whether input names from clients have any two of them that are in the
 * same component of the graph in the serverT's databse. If none, hint=False, otherwise True.
 */
void sendtoclients(bool hint){
    string resultforclientA;
    string resultforclientB;
    string *combinedresult;
    combinedresult=constructresultforclients(hint);
    resultforclientA=combinedresult[0];
    resultforclientB=combinedresult[1];
    delete[] combinedresult;
    
    if(send(acceptsocketforA,resultforclientA.data(),resultforclientA.size(),0)==-1){
        printf("%s\n",strerror(errno));
    }          //copied
    printf("The Central server sent the results to client A.\n");
    if(send(acceptsocketforB,resultforclientB.data(),resultforclientB.size(),0)==-1){
        printf("%s\n",strerror(errno));
    }          //copied
    printf("The Central server sent the results to client B.\n");
    close(acceptsocketforA);                               //copied
    close(acceptsocketforB);                               //copied
}



/**
 * @brief This function is for resetting some global variables
 * since the server is in a while loop.
 */
void resetvariables(){
    vector<string>().swap(targetnames);
    vector<string>().swap(targetIDs);
    vector<bool>().swap(found);
    nameBcount=0;
    foundconnection=false;
}





int main(){
    
    //set up two TCP listening sockets and one UDP listening scoket.
    hintsinitialization("TCP");
    listensocketconstruction("A");
    listensocketconstruction("B");
    hintsinitialization("UDP");
    listensocketconstruction("UDP");
    printf("The Central server is up and running.\n");
    
    while(true){
        try{
            string nameBs;
            string targetIDnotnull;
            string namestring;
            string relatedIDscores;
            string dataforserverP;
            
            resetvariables();
            
            //accept TCP connections from clients and extract nameA and nameBs.
            acceptTCPandextractnames("A");
            nameBs=acceptTCPandextractnames("B");
            
            //send all names from the clients to serverT and extract the IDs of the clients input
            //from the received information
            namestring=nameA+DELIMITER_FOR_ITEM+nameBs;
            sendtoserver(SERVERT_PORTNUMBER,namestring,"T");
            recvfromserver("T",SERVERT_PORTNUMBER);
            extracttargetIDs();
            
            //process information from serverT.
            //if clientA's input is not in the database graph, none of nameBs will be found path to nameA
            //therefore, no data will be sent to serverS and serverP, and final result is ready to be constructed and sent to cients.
            //else if clientA's input is in the graph, but none of the nameBs is found path to nameA, same result as the previous siuation.
            //else, extract IDs of nameBs that is found path to nameA, and sent useful data to serverS and serverP.
            //also, mark those IDs that found no path to nameA.
            if(targetIDs[0]=="NULL"){
                sendtoclients(false);
            }else{
                targetIDnotnull+=(targetIDs[0]+DELIMITER_FOR_ITEM);
                targetIDnotnull+=searchforconnection();
                if(!foundconnection){
                    sendtoclients(false);
                }else{
                    adjacentlists=strtok(NULL,DELIMITER_FOR_SECTION);
                    relatednames=strtok(NULL,DELIMITER_FOR_SECTION);
                    sendtoserver(SERVERS_PORTNUMBER,relatednames,"S");
                    recvfromserver("S",SERVERS_PORTNUMBER);
                    
                    relatedIDscores=buf;
                    dataforserverP=targetIDnotnull+DELIMITER_FOR_SECTION+adjacentlists+DELIMITER_FOR_SECTION+relatedIDscores+DELIMITER_FOR_SECTION+relatednames;
                    sendtoserver(SERVERP_PORTNUMBER,dataforserverP,"P");
                    recvfromserver("P",SERVERP_PORTNUMBER);
                    sendtoclients(true);
                }
            }
            delete[] buf;
        }
        catch(int a){
            close(acceptsocketforA);
            close(acceptsocketforB);
            continue;
        }
        
    }
    close(listensocketUDP);
    close(listensocketforA);
    close(listensocketforB);
    return 0;
}









