#include "iforest.h"
#include <math.h>
//#include <boost>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <cmath>
#include <iostream>

iforest::iforest()
{
    //ctor
}


iforest::iforest(const int &numitrees){
    numiTrees = numitrees;
}
iforest::~iforest()
{
    //dtor
}



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
    //cout<<"--------------"<<treeId<<"----------------"<<endl;
        iTrees[treeId] = new itree();
        vector<int> &sample = origDataset.getSample(sampleSize);
        iTrees[treeId]->createTree(origDataset, sample, treeHeight);
    }
}

void iforest::computeNodeMass(dataset &origDataset){
    for(int treeId = 0; treeId < numiTrees; treeId++){
        iTrees[treeId]->computeNodeMassforTree(origDataset);
    }
}


void iforest::massMatrixComputation(){
    double tempMass = 0;
    //ofstream massMatrixFile("massMatrixFile",ios::in|ios::binary);
    ofstream massMatrixFile("massMatrix/massMatrixFile",ios::out|ios::binary);
    if(!massMatrixFile){
        cout<<"Cannot open the output file massMatrix/massMatrixFile "<<endl;
        cout<<"here in mass matrix computation"<<endl;
        exit(0);
    }
    int i ;
    boost::archive::binary_oarchive massMatrixFileAr(massMatrixFile);
    //boost::archive::text_oarchive massMatrixFileAr(massMatrixFile);
   
    //for(int p1 = 0; p1 < origDataset.getnumInstances(); p1++){
    vector<int> pointsInTree;
    
    for(auto const & it : iTrees[0]->pointToNode){
    	pointsInTree.push_back(it.first);
    	//cout<<it.first<<" ";
    }
    i = pointsInTree.size();
    massMatrixFileAr & BOOST_SERIALIZATION_NVP(i);
    //cout<<"size of pointToNode"<<i<<endl;
    for(int p1 = 0; p1 < pointsInTree.size(); p1++){
        int point1 = pointsInTree[p1];
        //int point1 = 3099;
        pairwiseMass.resize(0);
        //for(int p2 = 0; p2 <= point1; po2++){
	
        for(int p2 = 0; p2 <= p1; p2++){
	    
	    int point2 = pointsInTree[p2];
	    //cout<<point1<<" "<<point2<<endl;	
            tempMass=0;
            for(int treeId = 0; treeId < iTrees.size(); treeId++){
                int leafNodeforPoint1 = iTrees[treeId]->pointToNode[point1];
                //cout<<"leaf for point1"<<leafNodeforPoint1<<" ";
                int leafNodeforPoint2 = iTrees[treeId]->pointToNode[point2];
                //cout<<"leaf for point2"<<leafNodeforPoint2<<" ";
                int LCAnodeforPoint1_Pint2 = computeLCA(leafNodeforPoint1,leafNodeforPoint2);
                //cout<<LCAnodeforPoint1_Pint2<<" "<<endl;
                
                //cout<<"NodeMAss"<<iTrees[treeId]->treeNode[LCAnodeforPoint1_Pint2]->nodeMass<<endl;
                tempMass += iTrees[treeId]->treeNode[LCAnodeforPoint1_Pint2]->nodeMass;
            }
            tempMass = tempMass/numiTrees;
            tempMass = tempMass/i;
            pairwiseMass.push_back(tempMass);
            //cout<<point1<<" "<<point2<<": "<< pairwiseMass[p2]<<endl;
	    
        }
        //cout<<endl;
        massMatrixFileAr & BOOST_SERIALIZATION_NVP(pairwiseMass);
        //cout<<"point "<<point1<<" saved to file"<<endl;
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

void iforest::readMassFile(dataset & origDataset){
    vector<double> pairwiseMassfromFile;
        ifstream massMatrixFile("massMatrix/massMatrixFile",ios::in);
        if(!massMatrixFile){
            cout<<"Cannot open the output file "<<endl;
            exit(0);
        }
        int i = origDataset.getnumInstances();
        //boost::archive::text_iarchive massMatrixFileAr(massMatrixFile);
        boost::archive::binary_iarchive massMatrixFileAr(massMatrixFile);
        massMatrixFileAr & BOOST_SERIALIZATION_NVP(i);
        for(int point1 = 0; point1<origDataset.getnumInstances(); point1++){
            massMatrixFileAr & BOOST_SERIALIZATION_NVP(pairwiseMassfromFile);

            for(int point2 = 0; point2<=point1; point2++){
                //cout<<point1<<" "<<point2<<": "<< pairwiseMassfromFile[point2]<<endl;
            }
            pairwiseMassfromFile.clear();
        }
}




