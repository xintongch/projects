//graph.cpp



#include<fstream>
#include<vector>
#include<map>
#include<queue>
#include "graph.h"


using namespace std;


/**
 * @brief This helper function is to construct a name ID map using the information from the provided file.
 * @param filename the name of the file containing edge lists.
 */
void graph::constructnameIDmap(const char* filename){
    ifstream edgelistdata(filename);
    string name;
    namecount=0;
    while(edgelistdata>>name){
        if(nameIDmap.find(name)==nameIDmap.end()){
            nameIDmap[name]=namecount;
            namecount++;
        }
    }
    edgelistdata.close();
}



/**
 * @brief This helper funciton is for constructing the adjacent lists using the provided file.
 * @param filename the name of the provided file containing the edge lists.
 */
void graph::constructadjacentlists(const char* filename){
    adjacentlists=new vector<int>[namecount];
    ifstream edgelistdata(filename);
    string name;
    while(edgelistdata>>name){
        int headID=nameIDmap[name];
        edgelistdata>>name;
        int tailID=nameIDmap[name];
        adjacentlists[headID].push_back(tailID);
        adjacentlists[tailID].push_back(headID);
    }
    edgelistdata.close();
}



/**
 * @brief This helper funciton is to find all components in a graph (in form of adjacent lists)
 * and sort each point to its component.
 */
void graph::constructcomponentarray(){
    components=new int[namecount];
    bfs();
}



/**
 * @brief This helper function is to use breadth first search to find all components of a graph and attach each point to its component.
 */
void graph::bfs(){
    bool visited[namecount];
    for(int i=0;i<namecount;i++){
        visited[i]=false;
    }
    int componentID=0;
    queue<int> bfsqueue;
    for(int nameID=0;nameID<namecount;nameID++){
        if(!visited[nameID]){
            componentID++;
            components[nameID]=componentID;
            visited[nameID]=true;
            bfsqueue.push(nameID);
            while(!bfsqueue.empty()){
                int currentnameID=bfsqueue.front();
                bfsqueue.pop();
                for(int i=0;i<adjacentlists[currentnameID].size();i++){
                    int adjacentnameID=adjacentlists[currentnameID][i];
                    if(!visited[adjacentnameID]){
                        visited[adjacentnameID]=true;
                        bfsqueue.push(adjacentnameID);
                        components[adjacentnameID]=componentID;
                    }
                }
            }
        }
    }
}




/**
 * @brief This helper function is to construct an ID name mapping array.
 */
void graph::constructIDnamearray(){
    IDnamearray=new string[namecount];
    map<string,int>::iterator iter;
    for(iter=nameIDmap.begin();iter!=nameIDmap.end();iter++){
        IDnamearray[nameIDmap[iter->first]]=iter->first;
    }
}




/**
 * @brief Construct a new graph::graph object
 * @param filename the name of the file containing egde lists for building a graph
 */
graph::graph(const char* filename){
    constructnameIDmap(filename);
    constructadjacentlists(filename);
    constructcomponentarray();
    constructIDnamearray();
}


/**
 * @brief The getter function for getting the ID of an input name
 * @param name the input name
 * @return int the corresponding ID
 */
int graph::getnameID(string name){
    return nameIDmap[name];
}


/**
 * @brief The getter function for getting the name of an input ID
 * @param ID the input ID
 * @return string the name of the input ID
 */
string graph::getIDname(int ID){
    return IDnamearray[ID];
}


/**
 * @brief The getter function for getting the component index of an input ID
 * @param ID the input ID
 * @return int the component index of the input ID
 */
int graph::getcomponent(int ID){
    return components[ID];
}


/**
 * @brief The getter function for getting the adjacent ID in some position of an input ID 
 * @param ID the ID whose adjacent ID is requested
 * @param adjacentindex the position of the requested adjacent ID 
 * @return int the adjacent ID requested 
 */
int graph::getadjacentnameID(int ID, int adjacentindex){
    return adjacentlists[ID][adjacentindex];
}


/**
 * @brief The getter function for getting the total number of the name stored in the graph object
 * @return int the number of the names in the graph object
 */
int graph::getnamecount(){
    return namecount;
}



/**
 * @brief The function for checking whether a particular name is stored in the graph object
 * @param name the name to be searched for
 * @return true the boolean vale indicating the input name is in the graph
 * @return false the boolean value indicating the input name is not in the graph
 */
bool graph::find(string name){
    if(nameIDmap.find(name)==nameIDmap.end()){
        return false;
    }else{
        return true;
    }
}


/**
 * @brief The getter function for getting the size of the adjacent list of an input ID
 * @param ID the ID whose adjacent list's size is requested
 * @return int the size of the adjacent list of the input ID
 */
int graph::getadjacentlistsize(int ID){
    return adjacentlists[ID].size();
}
