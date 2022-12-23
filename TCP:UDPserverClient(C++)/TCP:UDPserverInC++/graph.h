//graph.h


#ifndef _GRAPH_H
#define _GRAPH_H

#include<fstream>
#include<vector>
#include<map>
#include<queue>



using namespace std;


class graph{
private:
    
    map<string,int> nameIDmap;
    int namecount;
    vector<int> *adjacentlists;
    int *components;
    string *IDnamearray;

    
    void constructnameIDmap(const char* filename);

    void constructadjacentlists(const char* filename);
  
    void constructcomponentarray();
   
    void bfs();
    
    void constructIDnamearray();
   
public:
    
    graph(const char* filename);
    
    int getnameID(string name);
    
    string getIDname(int ID);
    
    int getcomponent(int ID);
    
    int getadjacentnameID(int ID, int adjacentindex);
    
    int getnamecount();
    
    bool find(string name);
    
    int getadjacentlistsize(int ID);
};


#endif
