//Calculator.cpp

#include<vector>
#include<climits>
#include<float.h>
#include<stdlib.h>
#include<stdlib.h>
#include<time.h>
#include "calculator.h"

using namespace std;

/**
 * @brief This helper function is for using Dijkstra algorithm to find the shortest (since the path from "start"
 * to "target" is a sum up of the matching gap of the edges along the path, so if we view the matching gap of 
 * the edge as it length, we can turn this problem into a shortest path problem) path from "start" to "target".
 * @param start from which ID we run the Dijkstra algorithm
 * @param target the other end of the path we are looking for.
 */
void calculator::Dijkstra(int start,int target){
    int nextAdd;
    int pathPos;
    
            for(int i=0;i<size;i++){
        smallestMG[i]=maxMG;
    }

    nextAdd=start;
    smallestMG[nextAdd]=0.0;
    isAdded[start]=true;
    while(nextAdd!=target){
        // IDupdated.push_back(nextAdd);
        updateSmallest(nextAdd);
        nextAdd=getSmallest();
    }
    
    pathPos=target;
    while(pathPos!=start){
        bestpath.push_back(pathPos);
        pathlength++;
        pathPos=previousID[pathPos];
    }
    bestpath.push_back(start);
    pathlength++;
}


/**
 * @brief This helper function is for calculating the matching gap of a particular edge with the provided head and tail
 * @param head the head provided
 * @param tail the tail provided
 * @return double the matching gap of the requested edge
 */
double calculator::calculateMatchinggap(int head, int tail){
    double headscore=scores[head];
    double tailscore=scores[tail];
    double difference=(headscore>tailscore)?(headscore-tailscore):(tailscore-headscore);
    double sum=headscore+tailscore;
    double matchinggap=difference/sum;
    return matchinggap;
}


/**
 * @brief This helper function is for updating the smallest matching gap array after a new node is added to 
 * the set that containing all nodes whose shortest "distance" from "start" is already determined.
 * @param nextAdd the lateset added ID
 */
void calculator::updateSmallest(int nextAdd){
    for(int i=0;i<adjacentlists[nextAdd].size();i++){
        int ID=adjacentlists[nextAdd][i];
        if(!isAdded[ID]){
            double candidateMG=(smallestMG[nextAdd]+matchingGapMatric[nextAdd][ID]);
            if(smallestMG[ID]>candidateMG){
                smallestMG[ID]=candidateMG;
                previousID[ID]=nextAdd;
            }
        }
    }
}


/**
 * @brief This helper function is for looking for ID with the smallest "distance" from "start"
 * @return int the ID with the smallest "distance" from the "start".
 */
int calculator::getSmallest(){
    double tempSmallest=maxMG;
    int tempID=size+1;
    for(int i=0;i<size;i++){
        if(!isAdded[i]){
            if(tempSmallest>smallestMG[i]){
                tempSmallest=smallestMG[i];
                tempID=i;
            }
        }
    }
    isAdded[tempID]=true;
    return tempID;
}



/**
 * @brief This helper function is for computing all the edges' matching gap and store the calues in a matric.
 */
void calculator::constructMatchingGapMatric(){
    maxMG=0.0;
    for(int i=0;i<size;i++){
        for(int j=0;j<adjacentlists[i].size();j++){
            double MG=calculateMatchinggap(i,adjacentlists[i][j]);
            matchingGapMatric[i][adjacentlists[i][j]]=MG;
            maxMG+=MG;
        }
    }
    maxMG+=1;
}


/**
 * @brief Construct a new calculator::calculator object
 * @param adjacentlists the provided adjacent lists
 * @param size the size of the provided arrays.
 * @param scores the provided ID score mapping array
 * @param IDnamemap the provided ID name mapping array
 */
calculator::calculator(vector<int> *adjacentlists,int size,double *scores,string *IDnamemap){
    this->adjacentlists=new vector<int>[size];
    this->IDnamemap=new string[size];
    this->scores=new double[size];
    this->size=size;
    smallestMG=new double[size];
    previousID=new int[size];
    for(int i=0;i<size;i++){
        this->adjacentlists[i]=adjacentlists[i];
        this->IDnamemap[i]=IDnamemap[i];
        this->scores[i]=scores[i];
    }
    matchingGapMatric=new double*[size];
    for(int i=0;i<size;i++){
        matchingGapMatric[i]=new double[size];
    }
    constructMatchingGapMatric();
}



/**
 * @brief This getter function is to calculate the smallest matching gap of the "start" and "target" and return the value.
 * @param start one of the requested end for the calculation
 * @param target the other end for the calculation
 * @return double the smallest matching gap
 */
double calculator::getMatchingGap(int start,int target){
    double smallestmatchinggap;
    if(start==target){
        pathlength=2;
        bestpath.push_back(target);
        bestpath.push_back(start);
        smallestmatchinggap=0.0;
        return smallestmatchinggap;
    }else{
        vector<int>().swap(bestpath);
            isAdded=new bool[size];
    for(int i=0;i<size;i++){
        isAdded[i]=false;
    }
        pathlength=0;
        Dijkstra(start,target);
        smallestmatchinggap=smallestMG[target];
        return ((int)(smallestmatchinggap*100)+1)/100.0;  //the "+1" is for round UP in all cases.
    }
}


/**
 * @brief This getter function is for getting the path in string form of the previously calculated matching gap.
 * So this function is called after the getMatchingGap function.
 * @param isforward indicates whether to return the path from "start" to "target" or the other way around.
 * @return string the string containing the path
 */
string calculator::getBestPathInString(bool isbackward){
    string pathInString;
    if(!isbackward){
        for(int i=0;i<pathlength;i++){
            pathInString+=IDnamemap[bestpath[i]];
            if(i!=pathlength-1){
                pathInString+="---";
            }
        }
    }else{
        for(int i=pathlength-1;i>=0;i--){
            pathInString+=IDnamemap[bestpath[i]];
            if(i!=0){
                pathInString+="---";
            }
        }
    }
    return pathInString;
}







