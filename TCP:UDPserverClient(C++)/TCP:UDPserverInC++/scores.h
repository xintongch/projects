//serverS.h

#include<fstream>
#include<map>


using namespace std;
#ifndef _SCORES_H_
#define _SCORES_H_



class scores{
private:
    map<string,string> namescoremap;
    
    
public:
    scores(const char* filename);
    
    string getScore(string name);
};



#endif


