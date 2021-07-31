#include <iostream>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include "dataset.h"
#include "iforest.h"
#include <string>
using namespace std;
/*
Static run:            static [# trees] [sampling factor] [minSampleSize] [static dataset name]
IncrementalAddition:   incremental addition [# trees] [sampling factor] [minSampleSize] [base dataFile name] [added pointsFile name] 
IncrementalDeletion:   incremental deletion [# trees] [sampling factor] [minSampleSize] [base dataFile nmae] [deleted pointsFile name] 
*/

int parseLine(char *line){
    int i = strlen(line);
    const char *p = line;
    while(*p <'0' || *p >'9')
	p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(int par){
    FILE *file = fopen("/proc/self/status","r");
    int result = -1;
    char line[128];

    while(fgets(line,128,file) != NULL){
	if(par == 1){               //Ram used
   	    if(strncmp(line, "VmRSS:", 6) == 0){
		result = parseLine(line);
		break;
	    }
	}
	if(par == 2){               //virtual memory used
   	    if(strncmp(line, "VmSize:", 7) == 0){
		result = parseLine(line);
		break;
	    }
	}
    }
    fclose(file);
    return result;
}


int main(int argc, char* argv[]){   
    srand(time(0));
	const int &numiTree = atoi(argv[1]);
	const double &samplingFactor = atof(argv[2]);
	const int &minSampleSize = atoi(argv[3]);
	int ramUsed = 0;
	int vMemUsed = 0;

        struct timespec start_input,end_input;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_input);
        const string &inputFileStaticRun = argv[4];
        dataset *origDataset = new dataset(inputFileStaticRun);
        dataset &reforigDataset = *origDataset;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_input);
        double inputTime =  (((end_input.tv_sec - start_input.tv_sec) * 1e9)+(end_input.tv_nsec - start_input.tv_nsec))*1e-9;
        //cout << "Time taken input from file: " << fixed << inputTime<<"sec"<<endl;
        ramUsed = max(getValue(1), ramUsed);
        vMemUsed = max(getValue(2), vMemUsed);

        struct timespec start_forest,end_forest;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_forest);
        iforest *staticForest = new iforest(numiTree);
        staticForest->createiForest(reforigDataset, samplingFactor, minSampleSize);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_forest);
        double forestTime =  (((end_forest.tv_sec - start_forest.tv_sec) * 1e9)+(end_forest.tv_nsec - start_forest.tv_nsec))*1e-9;
        //cout << "Time taken to create forest: " << fixed << forestTime<<"sec"<<endl;
        ramUsed = max(getValue(1), ramUsed);
        vMemUsed = max(getValue(2), vMemUsed);

        struct timespec start_nodeMass,end_nodeMass;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_nodeMass);
        staticForest->computeNodeMass(reforigDataset);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_nodeMass);
        double nodeMassTime =  (((end_nodeMass.tv_sec - start_nodeMass.tv_sec) * 1e9)+(end_nodeMass.tv_nsec - start_nodeMass.tv_nsec))*1e-9;
        //cout << "Time taken to compute nodeMass: " << fixed << nodeMassTime<<"sec"<<endl;
        ramUsed = max(getValue(1), ramUsed);
        vMemUsed = max(getValue(2), vMemUsed);
        //cout<< "ram used"<<getValue(1);

        struct timespec start_massMatrix,end_massMatrix;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_massMatrix);
        //staticForest->massMatrixComputation();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_massMatrix);
        double massMatrixTime =  (((end_massMatrix.tv_sec - start_massMatrix.tv_sec) * 1e9)+(end_massMatrix.tv_nsec - start_massMatrix.tv_nsec))*1e-9;
        //cout << "Time taken to compute massMatrix: " << fixed << massMatrixTime<<"sec"<<endl;
        //cout<< "ram used"<<getValue(1);
        ramUsed = max(getValue(1), ramUsed);
        vMemUsed = max(getValue(2), vMemUsed);
	cout <<fixed << inputTime<<" "<<forestTime<<" "<<nodeMassTime<<" "<<massMatrixTime<<" "<<inputTime+forestTime+nodeMassTime<<" "<<ramUsed<<" "<<vMemUsed<<endl;
        delete origDataset;
        delete staticForest;

    
    return 0;
}



 




























