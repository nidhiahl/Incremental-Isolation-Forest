#include "itree.h"
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include "treenode.h"
itree::itree()
{
    //ctor
}

itree::itree(int maxNodes){
    treeNode.resize(maxNodes);
    maxNumberOfNodes = maxNodes;
    for(int i = 0; i < maxNodes; i++){
        treeNode[i] = new treenode(i);
    }
}

itree::~itree()
{
    //dtor
}

void itree::initTree(int maxHeight, int maxNumberOfNodes){
    this->maxHeight = maxHeight;
    this->maxNumberOfNodes = maxNumberOfNodes;
}


void itree::createTree(dataset &origDataset, vector<int> &sample, int height){
    //numInstancesStored = origDataset.getnumInstances();
    maxHeight = height;
    maxNumberOfNodes = (int)pow(2.0,(double)(maxHeight+1))-1;
    treeNode.resize(maxNumberOfNodes);
    for(int  nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){
    	treeNode[nodeId] = nullptr;
    }
    for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){

    //cout<<"--------------"<<nodeId<<"----------------"<<endl;
        if(nodeId == 0){
            treeNode[nodeId] = new treenode(nodeId);
            treeNode[nodeId]->dataPointIndex.resize(sample.size());
            for(int in = 0; in < sample.size(); in++){
                treeNode[nodeId]->dataPointIndex[in] = sample.at(in);
            }
            if(treeNode[nodeId]->dataPointIndex.size() == 0){
            	treeNode[nodeId] = nullptr;
            }
            /*else{
            	treeNode[nodeId]->parentAdd = treeNode[nodeId];
            	treeNode[nodeId]->parentId = nodeId;
            }*/
        }
        
        if(treeNode[nodeId] == nullptr){continue;}
        //treeNode[nodeid]->nodeMass = treeNode[nodeid]->dataPointIndex.size(); //Not required at time of tree creation.

        treeNode[nodeId]->nodeHeight = (int)(log2((double)(nodeId+1)));
        //cout<<"nodeId"<<nodeid<<endl;

        if(treeNode[nodeId]->dataPointIndex.size() <= 1 || treeNode[nodeId]->nodeHeight == maxHeight){
        	treeNode[nodeId]->isLeaf = bool(1);
        	//treeNode[nodeId]->isActive = bool(1);
                //treeNode[nodeid]->parentAdd = treeNode[(nodeid-1)/2];
            
            continue;
        }

        //treeNode[nodeId]->splitAttribute = rand()%origDataset.getnumAttributes();
        treeNode[nodeId]->splitValue = treeNode[nodeId]->splitPointOfChoosenAttribute((origDataset));

        treenode *left = new treenode(2*nodeId+1);
        treenode *right = new treenode(2*nodeId+2);
        for(int i=0; i<treeNode[nodeId]->dataPointIndex.size(); i++){
            //if(origDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]].attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
            
            if(origDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]]->attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
                left->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
            }
            else{
                right->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
            }

        }
     	/*left->parentAdd = treeNode[nodeId];
      	left->parentId = nodeId;
      	treeNode[nodeId]->lChildAdd = left;
      	treeNode[nodeId]->lChildId = 2*nodeId+1;*/
      	treeNode[2*nodeId+1] = left;
       
      	/*right->parentAdd = treeNode[nodeId];
      	right->parentId = nodeId;
      	treeNode[nodeId]->rChildAdd = right;
      	treeNode[nodeId]->rChildId = 2*nodeId+2;*/
      	treeNode[2*nodeId+2] = right;

        //treeNode[nodeId]->isActive = bool(1);
    }
    
}

void itree::computeNodeMassforTree(dataset &origDataset){
    //pointToNode.resize(origDataset.getnumInstances());
    
    for(int in = 0; in<maxNumberOfNodes;in++){
    	if(treeNode[in] == nullptr){continue;}
        treeNode[in]->dataPointIndex.resize(0);
    }
    //cout<<"max no of nodes"<<maxNumberOfNodes<<endl;
    for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){
         
    	 if(treeNode[nodeId] == nullptr){continue;}
    	 
        if(nodeId == 0){
            treeNode[nodeId]->dataPointIndex.resize(origDataset.getnumInstances());
            for(int in = 0; in < origDataset.getnumInstances(); in++){
                treeNode[nodeId]->dataPointIndex[in] = in;
            }
        }
        

        treeNode[nodeId]->nodeMass = treeNode[nodeId]->dataPointIndex.size();
	
        /*if(!(treeNode[nodeId]->isActive) || treeNode[nodeId].dataPointIndex.size()==0){
            continue;
        }*/

        if(treeNode[nodeId]->isLeaf){
            for(int in = 0; in < treeNode[nodeId]->dataPointIndex.size(); in++){
                pointToNode[treeNode[nodeId]->dataPointIndex[in]] =nodeId;
                //pointToNode.insert(make_pair(treeNode[nodeId]->dataPointIndex[in]),nodeId);
                //pointToNode[treeNode[nodeId]->dataPointIndex[in]] = nodeId;
            }
            //treeNode[nodeId]->nodeMass = treeNode[nodeId]->dataPointIndex.size();

            continue;
        }
//cout<<"nodeID"<<nodeId<<endl;
//cout<<"nodeisLeaf"<<treeNode[nodeId]->isLeaf<<endl;
        for(int i=0; i<treeNode[nodeId]->dataPointIndex.size(); i++){
            
            //if(origDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]].attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
            if(origDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]]->attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
            	
            	 if(treeNode[2*nodeId + 1] ==nullptr){
            	 	treeNode[2*nodeId + 1] = new treenode(2*nodeId + 1);
            	 	treeNode[2*nodeId + 1]->isLeaf = bool(1);
            	 }
                treeNode[2*nodeId + 1]->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
            }
            else{
                if(treeNode[2*nodeId + 2] ==nullptr){
                	treeNode[2*nodeId + 2] = new treenode(2*nodeId + 1);
                	treeNode[2*nodeId + 2]->isLeaf = bool(1);
                }
                treeNode[2*nodeId + 2]->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
            }
        }
        //cout<<"nodeID"<<nodeId<<endl;
        //treeNode[nodeId]->nodeMass = treeNode[nodeId]->dataPointIndex.size();
    }
}


int itree::getMaxHeight(){
    return maxHeight;
}


int itree::getMaxNumberOfNodes(){
    return maxNumberOfNodes;
}



