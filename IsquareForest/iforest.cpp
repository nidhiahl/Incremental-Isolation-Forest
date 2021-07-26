#include "iforest.h"
#include <math.h>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <cmath>
#include <iostream>

iforest::iforest(){}


iforest::iforest(const int &numitrees){
    numiTrees = numitrees;
    avgNumUnchangedNodes = 0;
    avgNumTotalNodes = 0;
}
iforest::~iforest(){}



int iforest::getnumiTrees(){
    return this->numiTrees;
}


void iforest::createiForest(dataset &origDataset, const double &samplingFactor, const int &minSampleSize){
    int sampleSize = ((origDataset.getnumInstances()) * samplingFactor) < minSampleSize ? minSampleSize : ((origDataset.getnumInstances())* samplingFactor);
    sampleSize = origDataset.getnumInstances() < sampleSize ? origDataset.getnumInstances() : sampleSize;
    treeHeight = (int)log2(sampleSize);
    maxNumOfNodes = (int)pow(2.0,(double)(treeHeight+1))-1;
    iTrees.resize(numiTrees);
    for(int treeId = 0; treeId < this->numiTrees; treeId++){
        iTrees[treeId] = new itree();
        vector<int> &sample = origDataset.getSample(sampleSize);
        iTrees[treeId]->createTree(origDataset, sample, treeHeight);
    }
}


void iforest::additionIncForestUpdate(dataset &additionDataset,dataset &completeDataset, const double &samplingFactor, const int &minSampleSize){
    int sampleSize = ((additionDataset.getnumInstances())*samplingFactor) <minSampleSize ? minSampleSize : ((additionDataset.getnumInstances())*samplingFactor);
    sampleSize = additionDataset.getnumInstances()<sampleSize ? additionDataset.getnumInstances():sampleSize;
    

    for(int treeId = 0; treeId < numiTrees; treeId++){
    	
        vector<int> &sample = additionDataset.getSample(sampleSize);
        iTrees[treeId]->additionIncUpdateTree(additionDataset, completeDataset, sample);

    }
}

void iforest::delIncForestUpdate(dataset &delPoints){
int unchangedNodes = 0;
int totalNodes = 0;
    for(int treeId = 0; treeId < numiTrees; treeId++){
    	iTrees[treeId]->delIncUpdateTree(delPoints);
        unchangedNodes += iTrees[treeId]->numUnchangedNodes;
        totalNodes += iTrees[treeId]->numTotalNodes;
    }
    avgNumUnchangedNodes = unchangedNodes/numiTrees;
    avgNumTotalNodes = totalNodes/numiTrees;
}



void iforest::computeNodeMass(dataset &origDataset){
    for(int treeId = 0; treeId < numiTrees; treeId++){
        iTrees[treeId]->computeNodeMassforTree(origDataset);
    }
}

void iforest::additionIncComputeNodeMassforForest(dataset &additionDataset, dataset &completeDataset){
int unchangedNodes = 0;
int totalNodes = 0;
    for(int treeId = 0; treeId < numiTrees; treeId++){
    	iTrees[treeId]->additionIncComputeNodeMassforEachTree(additionDataset, completeDataset);
        unchangedNodes += iTrees[treeId]->numUnchangedNodes;
        totalNodes += iTrees[treeId]->numTotalNodes;
    }
    avgNumUnchangedNodes = unchangedNodes/numiTrees;
    avgNumTotalNodes = totalNodes/numiTrees;
}

void iforest::massMatrixComputation(){
    double tempMass = 0;
    ofstream massMatrixFile("massMatrix/massMatrixFile",ios::out|ios::binary);
    if(!massMatrixFile){
        cout<<"Cannot open the output file massMatrix/massMatrixFile "<<endl;
        cout<<"here in mass matrix computation"<<endl;
        exit(0);
    }
    int i ;
    boost::archive::binary_oarchive massMatrixFileAr(massMatrixFile);
    vector<int> pointsInTree;
    
    for(auto const & it : iTrees[0]->pointToNode){
    	
    }
    i = pointsInTree.size();
    massMatrixFileAr & BOOST_SERIALIZATION_NVP(i);
    for(int p1 = 0; p1 < pointsInTree.size(); p1++){
        int point1 = pointsInTree[p1];
        pairwiseMass.resize(0);
        
        for(int p2 = 0; p2 <= p1; p2++){
	    
	    int point2 = pointsInTree[p2];
	    tempMass=0;
            for(int treeId = 0; treeId < iTrees.size(); treeId++){
                int leafNodeforPoint1 = iTrees[treeId]->pointToNode[point1];
                int leafNodeforPoint2 = iTrees[treeId]->pointToNode[point2];
                int LCAnodeforPoint1_Pint2 = computeLCA(leafNodeforPoint1,leafNodeforPoint2);
                tempMass += iTrees[treeId]->treeNode[LCAnodeforPoint1_Pint2]->nodeMass;
            }
            tempMass = tempMass/numiTrees;
            tempMass = tempMass/i;
            pairwiseMass.push_back(tempMass);
            
        }
        massMatrixFileAr & BOOST_SERIALIZATION_NVP(pairwiseMass);
        pairwiseMass.clear();
    }
    massMatrixFile.close();
}

int iforest::computeLCA(int node1, int node2){
    while(node1!=node2){
        if(node1>node2){node1 = node1%2==0 ? (node1/2)-1 : (node1-1)/2;}
        else{node2 = node2%2==0 ? (node2/2)-1 : (node2-1)/2;}
    }
    return node1;
}





double iforest::massMatrixComputation(int i, int j, int N){
	double tempMass = 0;
	for(int treeId = 0; treeId < iTrees.size(); treeId++){
		int leafNodeforI = iTrees[treeId]->pointToNode[i];
                int leafNodeforJ = iTrees[treeId]->pointToNode[j];
                int LCAnodeforIJ = computeLCA(leafNodeforI,leafNodeforJ);
                tempMass += iTrees[treeId]->treeNode[LCAnodeforIJ]->nodeMass;
                
	}
	tempMass = tempMass/numiTrees;
	tempMass = tempMass/N;
        return tempMass;    
}

void iforest::readMassFile(dataset & origDataset){
    vector<double> pairwiseMassfromFile;
        ifstream massMatrixFile("massMatrix/massMatrixFile",ios::in);
        if(!massMatrixFile){
            cout<<"Cannot open the output file "<<endl;
            exit(0);
        }
        int i = origDataset.getnumInstances();
        boost::archive::binary_iarchive massMatrixFileAr(massMatrixFile);
        massMatrixFileAr & BOOST_SERIALIZATION_NVP(i);
        for(int point1 = 0; point1<origDataset.getnumInstances(); point1++){
            massMatrixFileAr & BOOST_SERIALIZATION_NVP(pairwiseMassfromFile);

            
            pairwiseMassfromFile.clear();
        }
}


void iforest::storeNodesAndTreesInFile(){
    ofstream storeNodes("IntermediateFiles/incStaticTreeNodes",ios::out|ios::binary);
    if(!storeNodes){
            cout<<"Cannot open the output file IntermediateFiles/staticTreeNodes"<<endl;
            exit(0);
        }
    boost::archive::binary_oarchive storeNodesArchive(storeNodes);
    ofstream storeTrees("IntermediateFiles/incStaticiTrees",ios::out|ios::binary);
    if(!storeTrees){
            cout<<"Cannot open the output file IntermediateFiles/staticiTrees"<<endl;
            exit(0);
        }
    boost::archive::binary_oarchive storeTreesArchive(storeTrees);
    int inActiveNode = 1;
    for(int treeId = 0; treeId < numiTrees; treeId++){
        for(int nodeId = 0; nodeId < iTrees[treeId]->getMaxNumberOfNodes(); nodeId++){
            if(iTrees[treeId]->treeNode[nodeId] != nullptr){
            iTrees[treeId]->treeNode[nodeId]->parentAdd = nullptr;
            iTrees[treeId]->treeNode[nodeId]->lChildAdd = nullptr;
            iTrees[treeId]->treeNode[nodeId]->rChildAdd = nullptr;
            storeNodesArchive << *(iTrees[treeId]->treeNode[nodeId]);
            delete iTrees[treeId]->treeNode[nodeId];
            }
            else{
            	//storeNodesArchive & BOOST_SERIALIZATION_NVP(inActiveNode);
	    }
        }
        storeTreesArchive << *(iTrees[treeId]);
        delete iTrees[treeId];
    }
    storeNodes.close();
    storeTrees.close();
}


void iforest::readNodesAndTreesfromFile(){

    ifstream readNodes("IntermediateFiles/staticTreeNodes",ios::in|ios::binary);
    if(!readNodes){
            cout<<"Cannot open the input file IntermediateFiles/staticTreeNodes"<<endl;
            exit(0);
        }
    boost::archive::binary_iarchive readNodesArchive(readNodes);
    ifstream readTrees("IntermediateFiles/staticiTrees",ios::in|ios::binary);
    if(!readTrees){
            cout<<"Cannot open the input file IntermediateFiles/staticiTrees"<<endl;
            exit(0);
        }
    boost::archive::binary_iarchive readTreesArchive(readTrees);
    iTrees.resize(numiTrees);
    for(int treeId = 0; treeId < numiTrees; treeId++){
    int total=0;
    	//cout<<"----------"<<treeId<<"-------------"<<endl;
    	iTrees[treeId] = new itree();
        readTreesArchive >> *(iTrees[treeId]);
        iTrees[treeId]->treeNode.resize(iTrees[treeId]->getMaxNumberOfNodes());
        
        for(int nodeId = 0; nodeId < iTrees[treeId]->getMaxNumberOfNodes(); nodeId++){
            iTrees[treeId]->treeNode[nodeId] = nullptr;
        }
        int noNodes = 1;
        for(int node = 0;node < noNodes; node++)
        {
        	//cout<<"----------"<<node<<"-------------"<<endl;
        	treenode *currentNode = new treenode();
        	readNodesArchive >> *(currentNode);
        	if(currentNode->lChildId > 0){noNodes++;}
        	if(currentNode->rChildId > 0){noNodes++;}
        	iTrees[treeId]->treeNode[currentNode->nodeId] = currentNode;
        	currentNode->parentAdd = node == 0 ? currentNode : iTrees[treeId]->treeNode[((currentNode->nodeId)-1)/2];
        	if(node !=0){
        		if(iTrees[treeId]->treeNode[(currentNode->nodeId)]->nodeId % 2 ==0){
        			iTrees[treeId]->treeNode[currentNode->nodeId]->parentAdd->rChildAdd = iTrees[treeId]->treeNode[currentNode->nodeId];
        		}
        		else{
        			iTrees[treeId]->treeNode[currentNode->nodeId]->parentAdd->lChildAdd = iTrees[treeId]->treeNode[currentNode->nodeId];
        		}
        	}
        }
    }
    readNodes.close();
    readTrees.close();

}
