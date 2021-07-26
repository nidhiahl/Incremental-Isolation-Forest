#include "dataset.h"
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

dataset::dataset()
{
    //ctor
}


dataset::dataset(const string& inputfileName, const vector<int> & delPoints){
    ifstream inFile(inputfileName.c_str(), ios::in|ios::binary);
    if(!inFile){
        cout<<"Cannot open the input file: "<<inputfileName<<endl;
        
        exit(0);
    }
    inFile>>numInstances>>numAttributes;
    for(int instance = 0; instance < numInstances; instance++){
    	
        point *rpt = new point();
        rpt->attributes = new double[numAttributes];
        
        for(int attribute = 0; attribute < numAttributes; attribute++){
           
            inFile>>rpt->attributes[attribute];
            
        }
        inFile>>rpt->label;
        dataPoints[delPoints[instance]] = rpt;
    }
    inFile.close();
}

dataset::dataset(const string& inputfileName){
    ifstream inFile(inputfileName.c_str(), ios::in|ios::binary);
    if(!inFile){
        cout<<"Cannot open the input file: "<<inputfileName<<endl;
        
        exit(0);
    }
    inFile>>numInstances>>numAttributes;
    for(int instance = 0; instance < numInstances; instance++){
    	
        point *rpt = new point();
        
        for(int attribute = 0; attribute < numAttributes; attribute++){
            inFile>>rpt->attributes[attribute];
            
        }
        inFile>>rpt->label;
        dataPoints[instance] = rpt;
        
    }
    inFile.close();
}

dataset::~dataset()
{
    //dtor
}


int dataset::getnumAttributes(){
    return numAttributes;
}

void dataset::setnumInstances(int numIn){
   numInstances = numIn;
}

int dataset::getnumInstances(){
    return numInstances;
}


vector<int> &dataset::getSample(int sampleSize){
            vector<int>* sample = new vector<int>;
            vector<int> &refSample = *sample;
            for(int i=0;i<numInstances; i++){
                (*sample).push_back(i);
            }
            std::random_shuffle((*sample).begin(),(*sample).end());
            (*sample).erase((*sample).begin()+sampleSize,(*sample).begin()+numInstances);
            return refSample;

        }
