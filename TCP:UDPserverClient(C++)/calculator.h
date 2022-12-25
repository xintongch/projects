//Calculator.h


#include<vector>
#include<climits>
#include<float.h>
#include<iostream>
#include<stdlib.h>


using namespace std;
#ifndef _CALCULATOR_H
#define _CALCULATOR_H



class calculator{
private:
    vector<int> *adjacentlists;
    double *scores;
    int size;
    vector<int> bestpath;
    int pathlength;
    string *IDnamemap;
    double **matchingGapMatric;
    double *smallestMG;
    // vector<int> IDupdated;
    int *previousID;
    bool *isAdded;
    double maxMG;
    
    
    void Dijkstra(int start, int target);
    
    double calculateMatchinggap(int head, int tail);

    void constructMatchingGapMatric();

    int getSmallest();
    
    void updateSmallest(int nextAdd);

public:
    calculator(vector<int> *adjacentlists,int size,double *scores,string *IDnamemap);
    
    double getMatchingGap(int start,int target);
    
    string getBestPathInString(bool isforward);

};

#endif
