//serverS.h

#include<fstream>
#include<map>
#include "scores.h"


using namespace std;


/**
 * @brief Construct a new scores::scores object
 * @param filename the name of the file containing the name and corresponding score information
 */
scores::scores(const char* filename){
    ifstream scorefile(filename);
    string name;
    string score;
    while(scorefile>>name){
        scorefile>>score;
        namescoremap[name]=score;
    }
    scorefile.close();
}


/**
 * @brief The getter function for getting the score of an input name
 * @param name the name whose score is requested
 * @return string the score of the input name in string form
 */
string scores::getScore(string name){
    if(namescoremap.find(name)!=namescoremap.end()){
        return namescoremap[name];
    }else{
        string null="NULL";
        return null;
    }
    
}


