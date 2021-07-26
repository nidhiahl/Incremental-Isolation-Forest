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
IncrementalAddition:   incremental addition [# trees] [sampling factor] [minSampleSize] [base dataFile name] 
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


vector<int> removedPoints;
vector<int> getDeletedIndecies(const string &existingDataFile, const string &deletionInputFile){
    
    ifstream delFile(deletionInputFile.c_str(), ios::in | ios::binary);
    if(!delFile)
    {
      cout<<"Cannot open the deletion input file: "<<deletionInputFile<<endl;
      exit(0);
    }
    ifstream existingFile(existingDataFile.c_str(), ios::in | ios::binary);
    if(!delFile)
    {
      cout<<"Cannot open the deletion input file: "<<existingDataFile<<endl;
      exit(0);
    }
    string delPoint;
    string everyPoint;
    getline(delFile, delPoint);
    while(getline(delFile, delPoint)){
        int deletedPointIndex=-1;
        while(getline(existingFile, everyPoint)){
            if(delPoint==everyPoint){
                removedPoints.push_back(deletedPointIndex);
                existingFile.seekg(0);
                break;
            }
            deletedPointIndex++;
        }
    }
    delFile.close(); existingFile.close();
    return removedPoints;
}


int main(int argc, char* argv[]){   
    srand(time(0));
    int ramUsed = 0;
    int vMemUsed = 0;
    const string &typeOfRun = argv[1];       //static or incremental
    if(typeOfRun=="static"){
	const int &numiTree = atoi(argv[2]);
	const double &samplingFactor = atof(argv[3]);
	const int &minSampleSize = atoi(argv[4]);

        struct timespec start_input,end_input;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_input);
        const string &inputFileStaticRun = argv[5];
        dataset *origDataset = new dataset(inputFileStaticRun);
        dataset &reforigDataset = *origDataset;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_input);
        double inputTime =  (((end_input.tv_sec - start_input.tv_sec) * 1e9)+(end_input.tv_nsec - start_input.tv_nsec))*1e-9;
        //cout << "Time taken input from file: " << fixed << inputTime<<"sec"<<endl;
        ramUsed = max(getValue(1),ramUsed);
        vMemUsed = max(getValue(2),vMemUsed);
        //cout<< "ram used"<<getValue(1);

        struct timespec start_forest,end_forest;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_forest);
        iforest *staticForest = new iforest(numiTree);
        staticForest->createiForest(reforigDataset, samplingFactor, minSampleSize);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_forest);
        double forestTime =  (((end_forest.tv_sec - start_forest.tv_sec) * 1e9)+(end_forest.tv_nsec - start_forest.tv_nsec))*1e-9;
        //cout << "Time taken to create forest: " << fixed << forestTime<<"sec"<<endl;
	ramUsed = max(getValue(1),ramUsed);
        vMemUsed = max(getValue(2),vMemUsed);
        //cout<< "ram used"<<getValue(1);

        struct timespec start_nodeMass,end_nodeMass;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_nodeMass);
        staticForest->computeNodeMass(reforigDataset);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_nodeMass);
        double nodeMassTime =  (((end_nodeMass.tv_sec - start_nodeMass.tv_sec) * 1e9)+(end_nodeMass.tv_nsec - start_nodeMass.tv_nsec))*1e-9;
        //cout << "Time taken to compute nodeMass: " << fixed << nodeMassTime<<"sec"<<endl;
        ramUsed = max(getValue(1),ramUsed);
        vMemUsed = max(getValue(2),vMemUsed);
        //cout<< "ram used"<<getValue(1);

        struct timespec start_massMatrix,end_massMatrix;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_massMatrix);
        //staticForest->massMatrixComputation();
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_massMatrix);
        double massMatrixTime =  (((end_massMatrix.tv_sec - start_massMatrix.tv_sec) * 1e9)+(end_massMatrix.tv_nsec - start_massMatrix.tv_nsec))*1e-9;
        //cout << "Time taken to compute massMatrix: " << fixed << massMatrixTime<<"sec"<<endl;
        //cout<< "ram used"<<getValue(1);
        ramUsed = max(getValue(1),ramUsed);
        vMemUsed = max(getValue(2),vMemUsed);
	cout <<fixed << inputTime<<" "<<forestTime<<" "<<nodeMassTime<<" "<<massMatrixTime<<" "<<inputTime+forestTime+nodeMassTime<<" "<<ramUsed<<" "<<vMemUsed<<endl;
        staticForest->storeNodesAndTreesInFile();
	delete origDataset;
        delete staticForest;

    }





    else if(typeOfRun == "incremental"){
        const int &numiTree = atoi(argv[3]);
        const double &samplingFactor = atof(argv[4]);
        const int &minSampleSize = atoi(argv[5]);
        const string &staticInputFileIncrementalRun = argv[6];
        const string &incInputFileIncrementalRun = argv[7];
        const string &typeOfUpdate = argv[2];   //addition or deletion or both
        if(typeOfUpdate == "addition"){


            struct timespec start_input,end_input;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_input);
            dataset *completeDataset = new dataset(staticInputFileIncrementalRun);
            dataset &refcompleteDataset = *completeDataset;
            dataset *additionDataset = new dataset(incInputFileIncrementalRun);
            dataset &refadditionDataset = *additionDataset;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_input);
            double inputTime =  (((end_input.tv_sec - start_input.tv_sec) * 1e9)+(end_input.tv_nsec - start_input.tv_nsec))*1e-9;
            //cout << "Time taken input from file: " << fixed << inputTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            //cout<< "ram used"<<getValue(1);

            struct timespec start_forest,end_forest;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_forest);
            iforest *incrementalForest = new iforest(numiTree);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_forest);
            double forestTime =  (((end_forest.tv_sec - start_forest.tv_sec) * 1e9)+(end_forest.tv_nsec - start_forest.tv_nsec))*1e-9;
            //cout << "Time taken to create forest: " << fixed << forestTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            //cout<< "ram used"<<getValue(1);


            struct timespec start_loadHistory,end_loadHistory;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_loadHistory);
            incrementalForest->readNodesAndTreesfromFile();
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_loadHistory);
            double loadHistoryTime =  (((end_loadHistory.tv_sec - start_loadHistory.tv_sec) * 1e9)+(end_loadHistory.tv_nsec - start_loadHistory.tv_nsec))*1e-9;
            //cout << "Time taken to read forest form file: " << fixed << loadHistoryTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            //cout<< "ram used"<<getValue(1);


            struct timespec start_forestupdate,end_forestupdate;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_forestupdate);
            incrementalForest->additionIncForestUpdate(refadditionDataset, refcompleteDataset, samplingFactor, minSampleSize);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_forestupdate);
            double forestUpdateTime =  (((end_forestupdate.tv_sec - start_forestupdate.tv_sec) * 1e9)+(end_forestupdate.tv_nsec - start_forestupdate.tv_nsec))*1e-9;
            //cout << "Time taken to create forest: " << fixed << forestTime+forestUpdateTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            //cout<< "ram used"<<getValue(1);

            struct timespec start_nodeMass,end_nodeMass;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_nodeMass);
            incrementalForest->additionIncComputeNodeMassforForest(refadditionDataset, refcompleteDataset);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_nodeMass);
            double nodeMassTime =  (((end_nodeMass.tv_sec - start_nodeMass.tv_sec) * 1e9)+(end_nodeMass.tv_nsec - start_nodeMass.tv_nsec))*1e-9;
            //cout << "Time taken to compute nodeMass: " << fixed << nodeMassTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            //cout<< "ram used"<<getValue(1);

            struct timespec start_massMatrix,end_massMatrix;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_massMatrix);
            //incrementalForest->massMatrixComputation();
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_massMatrix);
            double massMatrixTime =  (((end_massMatrix.tv_sec - start_massMatrix.tv_sec) * 1e9)+(end_massMatrix.tv_nsec - start_massMatrix.tv_nsec))*1e-9;
            //cout << "Time taken to compute massMatrix: " << fixed << massMatrixTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            //cout<< "ram used"<<getValue(1);
            
            struct timespec start_storeHistory,end_storeHistory;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_storeHistory);
            incrementalForest->storeNodesAndTreesInFile();
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_storeHistory);
            double storeHistoryTime =  (((end_storeHistory.tv_sec - start_storeHistory.tv_sec) * 1e9)+(end_storeHistory.tv_nsec - start_storeHistory.tv_nsec))*1e-9;
            //cout << "Time taken to compute massMatrix: " << fixed << storeHistoryTime<<"sec"<<endl;
            

	    cout <<fixed << inputTime<<" "<<forestTime<<" "<<loadHistoryTime<<" "<<forestUpdateTime<<" "<<nodeMassTime<<" "<<massMatrixTime<<" "<<storeHistoryTime<<" "<<inputTime+loadHistoryTime+forestTime+forestUpdateTime+nodeMassTime+storeHistoryTime<<" "<<ramUsed<<" "<<vMemUsed<<" "<<incrementalForest->avgNumTotalNodes<<" "<<incrementalForest->avgNumUnchangedNodes<<endl;


            delete incrementalForest;
            delete additionDataset;
            delete completeDataset;


            
        }
        else if(typeOfUpdate == "deletion"){
        /****************Preprocessing for deletion************/
          

            struct timespec start_delIndexFind,end_delIndexFind;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_delIndexFind);
            vector<int> delPoints = getDeletedIndecies(staticInputFileIncrementalRun, incInputFileIncrementalRun);
	    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_delIndexFind);
            double delIndexFindTime =  (((end_delIndexFind.tv_sec - start_delIndexFind.tv_sec) * 1e9)+(end_delIndexFind.tv_nsec - start_delIndexFind.tv_nsec))*1e-9;
            //cout << "Time taken to find delIndecies: " << fixed << delIndexFindTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            //cout<< "ram used"<<getValue(1);

	    struct timespec start_delDatasetCreate,end_delDatasetCreate;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_delDatasetCreate);
	    dataset *deletePoint = new dataset(incInputFileIncrementalRun, delPoints);
	    dataset &refdelPoints = *deletePoint;
	    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_delDatasetCreate);
            double delDatasetCreateTime =  (((end_delDatasetCreate.tv_sec - start_delDatasetCreate.tv_sec) * 1e9)+(end_delDatasetCreate.tv_nsec - start_delDatasetCreate.tv_nsec))*1e-9;
            //cout << "Time taken to find delIndecies: " << fixed << delIndexFindTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            
            //cout<< "ram used"<<getValue(1);

	    
	    
	    
	    struct timespec start_forest,end_forest;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_forest);
            iforest *incrementalForest = new iforest(numiTree);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_forest);
            double forestTime =  (((end_forest.tv_sec - start_forest.tv_sec) * 1e9)+(end_forest.tv_nsec - start_forest.tv_nsec))*1e-9;
            //cout << "Time taken to create forest: " << fixed << forestTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            
            //cout<< "ram used"<<getValue(1);


            struct timespec start_loadHistory,end_loadHistory;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_loadHistory);
            incrementalForest->readNodesAndTreesfromFile();
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_loadHistory);
            double loadHistoryTime =  (((end_loadHistory.tv_sec - start_loadHistory.tv_sec) * 1e9)+(end_loadHistory.tv_nsec - start_loadHistory.tv_nsec))*1e-9;
            //cout << "Time taken to read forest form file: " << fixed << loadHistoryTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            
            //cout<< "ram used"<<getValue(1);
            
            
            struct timespec start_forestupdate,end_forestupdate;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_forestupdate);
            incrementalForest->delIncForestUpdate(refdelPoints);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_forestupdate);
            double forestUpdateTime =  (((end_forestupdate.tv_sec - start_forestupdate.tv_sec) * 1e9)+(end_forestupdate.tv_nsec - start_forestupdate.tv_nsec))*1e-9;
           // cout << "Time taken to create forest: " << fixed << forestTime+forestUpdateTime<<"sec"<<endl;
            //cout<< "ram used"<<getValue(1);
            
            
            struct timespec start_massMatrix,end_massMatrix;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_massMatrix);
            //incrementalForest->massMatrixComputation();
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_massMatrix);
            double massMatrixTime =  (((end_massMatrix.tv_sec - start_massMatrix.tv_sec) * 1e9)+(end_massMatrix.tv_nsec - start_massMatrix.tv_nsec))*1e-9;
            //cout << "Time taken to compute massMatrix: " << fixed << massMatrixTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            
            //cout<< "ram used"<<getValue(1);
            
            struct timespec start_storeHistory,end_storeHistory;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_storeHistory);
            incrementalForest->storeNodesAndTreesInFile();
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_storeHistory);
            double storeHistoryTime =  (((end_storeHistory.tv_sec - start_storeHistory.tv_sec) * 1e9)+(end_storeHistory.tv_nsec - start_storeHistory.tv_nsec))*1e-9;
            //cout << "Time taken to compute massMatrix: " << fixed << storeHistoryTime<<"sec"<<endl;
            ramUsed = max(getValue(1),ramUsed);
            vMemUsed = max(getValue(2),vMemUsed);
            
		
	    cout <<fixed << delIndexFindTime<<" "<<forestTime<<" "<<loadHistoryTime<<" "<<forestUpdateTime<<" "<<massMatrixTime<<" "<<storeHistoryTime<<" "<<delDatasetCreateTime+loadHistoryTime+forestTime+forestUpdateTime+storeHistoryTime<<" "<<ramUsed<<" "<<vMemUsed<<" "<<incrementalForest->avgNumTotalNodes<<" "<<incrementalForest->avgNumUnchangedNodes<<" "<<delDatasetCreateTime<<endl;
	    
	    delete incrementalForest;
            delete deletePoint;
        }

    }

    return 0;
}



 




























