//serverP.cpp

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<vector>
#include<climits>
#include<sstream>
#include<string.h>
#include<float.h>
#include "calculator.h"


using namespace std;

#define localhost "127.0.0.1"
#define PORTNUMBER "23810"
#define SERVERC_PORTNUMBER "24810"
#define MAX_SENDING_PACKET_SIZE 8000
#define LENGTH_PADDING_TO 8
#define DELIMITER_FOR_ITEM " "
#define DELIMITER_FOR_SECTION ";"
#define DELIMITER_FOR_BLOCK "|"

struct addrinfo hints, *res;
int listensocketP=0;
char *buf;
int numbytesrecv=0;
struct sockaddr_storage client_addr;
socklen_t addr_len=sizeof(client_addr);
int biggestID=0;
vector<int> targetIDs;
vector<int> *adjacentlists;
double *IDscoremap;
string *IDnamemap;
int namecount;


/**
 * @brief This function is for setting up the addrinfo variable.
 * This block of codes is copied from Beej's book and modified, mainly "memset()";
 */
void hintsinitialization(){
    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_DGRAM;
}



/**
 * @brief This function is for constructing the UDP listening socket for serverP.
 * This block of codes is copied from Beej's book and modified, mainly "getaddrinfo(),socket(),bind(),freeaddrinfo()"
 */
void constructlistensocket(){
    getaddrinfo(localhost,PORTNUMBER,&hints,&res);
    if((listensocketP=socket(res->ai_family,res->ai_socktype, res->ai_protocol))==-1){
        printf("%s\n",strerror(errno));
    }
    if(bind(listensocketP, res->ai_addr, res->ai_addrlen)==-1){
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
    
    numbytesrecv=recvfrom(listensocketP,buf,MAX_SENDING_PACKET_SIZE,0,(struct sockaddr*)&client_addr,&addr_len);   //copied from Beej's book
    buf[numbytesrecv]='\0';
    tempstring=buf;
    istringstream tempstream(tempstring);
    tempstream>>recvtimes;
    for(int i=LENGTH_PADDING_TO;i<=numbytesrecv;i++){
        buf[i-LENGTH_PADDING_TO]=buf[i];
    }
    while(recvtimes>1){
        char tempbuf[MAX_SENDING_PACKET_SIZE+1];
        numbytesrecv=recvfrom(listensocketP,tempbuf,MAX_SENDING_PACKET_SIZE,0,(struct sockaddr*)&client_addr,&addr_len);   //copied from Beej's book
        tempbuf[numbytesrecv]='\0';
        char *combinedArray=new char[strlen(buf)+strlen(tempbuf)+1];
        strcpy(combinedArray,buf);
        strcat(combinedArray,tempbuf);
        delete[] buf;
        buf=combinedArray;
        buf[strlen(buf)]='\0';
        recvtimes--;
    }
    printf("The ServerP received the topology and score information.\n");
}





/**
 * @brief This function is for getting the biggest ID out of the information sent from central server
 * @param relatedIDscores contains a mapping from ID to score.
 */
void getbiggestID(string relatedIDscores){
    istringstream datastream(relatedIDscores);
    datastream>>biggestID;
    int discarded=0;
    int IDtemp=0;
    datastream>>discarded;
    while(datastream>>IDtemp){
        datastream>>discarded;
        if(IDtemp>biggestID){
            biggestID=IDtemp;
        }
    }
}


/**
 * @brief This function is for extracting target IDs from data sent from serverC.
 * @param targetIDstring contains all target IDs in the data sent from serverC.
 */
void extractTargetIDs(string targetIDstring){
    istringstream tempstream(targetIDstring);
    int tempID=0;
    while(tempstream>>tempID){
        targetIDs.push_back(tempID);
        namecount++;
    }
}



/**
 * @brief This function is fot extracting adjacentlists from data sent frome serverC.
 * @param adjacentlistsrowdata contains all related adjacent lists.
 */
void extractAdjacentlists(char* adjacentlistsrow){
    adjacentlists=new vector<int>[biggestID+1];
    char* adjacentlist=strtok(adjacentlistsrow,DELIMITER_FOR_BLOCK);
    while(adjacentlist!=NULL){
        string adjacentliststring=adjacentlist;
        istringstream adjacentliststream(adjacentliststring);
        int headID=0;
        adjacentliststream>>headID;
        int adjacentID=0;
        while(adjacentliststream>>adjacentID){
            adjacentlists[headID].push_back(adjacentID);
        }
        adjacentlist=strtok(NULL,DELIMITER_FOR_BLOCK);
    }
}


/**
 * @brief This function is for extracting related ID and its scores and building a array for mapping.
 * @param relatedIDscores contains a mapping of ID and score.
 */
void constructIDscoresMapping(string relatedIDscores){
    istringstream relatedIDscoresstream(relatedIDscores);
    IDscoremap=new double[biggestID+1];
    int ID=0;
    while(relatedIDscoresstream>>ID){
        double score;
        relatedIDscoresstream>>score;
        IDscoremap[ID]=score;
    }
}


/**
 * @brief This function is for constructing a mapping from ID to name.
 * @param relatedIDnames contains a mapping from ID to name in the form of a string
 */
void constructIDNameMapping(string relatedIDnames){
    int ID=0;
    string name;
    
    IDnamemap=new string[biggestID+1];
    istringstream temp(relatedIDnames);
    while(temp>>ID){
        temp>>name;
        IDnamemap[ID]=name;
    }
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
            if(sendto(listensocketP,result.data(),result.size(),0,res->ai_addr,res->ai_addrlen)==-1){
                printf("%s\n",strerror(errno));
            }          //copied from Beej's book
        }else{
            if(sendto(listensocketP,result.data(),MAX_SENDING_PACKET_SIZE,0,res->ai_addr,res->ai_addrlen)==-1){
                printf("%s\n",strerror(errno));
            }          //copied from Beej's book
            result=result.substr(MAX_SENDING_PACKET_SIZE);
        }
        sendingtimes--;
    }
    printf("The ServerT finished sending the results to the Central.\n");
    freeaddrinfo(res);
}




/**
 * @brief This function if for converting a double to a string.
 * @param i contain the double to be converted.
 * @return string the string converted from the input double.
 */
string doubleToString(double i){
    char temp[100];
    string tempstring;
    sprintf(temp,"%.2lf",i);
    tempstring=temp;
    return tempstring;
}




/**
 * @brief This function is for resetting global variable at the begining of each iteration of the while loop.
 */
void resetvariables(){
    vector<int>().swap(targetIDs);
    namecount=0;
}




int main(){
    
    //set up the UDP listening socket for serverP
    hintsinitialization();
    constructlistensocket();
    printf("The ServerP is up and running using UDP on port %s.\n",PORTNUMBER);
    
    while(true){
        resetvariables();
        
        //receive data from serverC and extract all sections from the data. There are four sections in the data.
        recvfromcentral();
        string targetIDstring=strtok(buf,DELIMITER_FOR_SECTION);
        char *adjacentlistsrowdata=strtok(NULL,DELIMITER_FOR_SECTION);
        string relatedIDscores=strtok(NULL,DELIMITER_FOR_SECTION);
        string relatedIDnames=strtok(NULL,DELIMITER_FOR_SECTION);
        
        //further extract all blocks or items from the sections of data.
        getbiggestID(relatedIDscores);
        extractTargetIDs(targetIDstring);
        extractAdjacentlists(adjacentlistsrowdata);
        constructIDscoresMapping(relatedIDscores);
        constructIDNameMapping(relatedIDnames);
        
        //construct a calculator object using all the information extracted from the data from serverC
        // and compute the result.
        calculator myCalculator(adjacentlists,biggestID+1,IDscoremap,IDnamemap);
        string result;
        for(int i=1;i<namecount;i++){
            double matchingGap=myCalculator.getMatchingGap(targetIDs[0],targetIDs[i]);
            result+=("Found compatibility for "+IDnamemap[targetIDs[0]]+" and "+IDnamemap[targetIDs[i]]+": \n");
            result+=(myCalculator.getBestPathInString(true)+" \n");
            result+=("Matching Gap : "+doubleToString(matchingGap)+DELIMITER_FOR_BLOCK);
            
            result+=("Found compatibility for "+IDnamemap[targetIDs[i]]+" and "+IDnamemap[targetIDs[0]]+": \n");
            result+=(myCalculator.getBestPathInString(false)+" \n");
            result+=("Matching Gap : "+doubleToString(matchingGap)+DELIMITER_FOR_BLOCK);
        }

        //send the final result to serverC.
        sendtocentral(result);
        delete[] buf;
        delete[] adjacentlists;
        delete[] IDscoremap;
        delete[] IDnamemap;
    }
    close(listensocketP);
    return 0;
}






