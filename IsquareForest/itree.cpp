#include "itree.h"
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include "treenode.h"
itree::itree()
{
    numUnchangedNodes = 0;
    numTotalNodes = 0;
}

itree::itree(int maxNodes){
    treeNode.resize(maxNodes);
    maxNumberOfNodes = maxNodes;
    for(int i = 0; i < maxNodes; i++){
        treeNode[i] = new treenode(i);
    }
}

itree::~itree(){}

void itree::initTree(int maxHeight, int maxNumberOfNodes){
    this->maxHeight = maxHeight;
    this->maxNumberOfNodes = maxNumberOfNodes;
}


void itree::createTree(dataset &origDataset, vector<int> &sample, int height){
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
            else{
            	treeNode[nodeId]->parentAdd = treeNode[nodeId];
            	treeNode[nodeId]->parentId = nodeId;
            }
        }
        
        if(treeNode[nodeId] == nullptr){continue;}
        treeNode[nodeId]->nodeHeight = (int)(log2((double)(nodeId+1)));
        if(treeNode[nodeId]->dataPointIndex.size() <= 1 || treeNode[nodeId]->nodeHeight == maxHeight){
        	treeNode[nodeId]->isLeaf = bool(1);
        	treeNode[nodeId]->isActive = bool(1);
                
            continue;
        }

        treeNode[nodeId]->splitValue = treeNode[nodeId]->splitPointOfChoosenAttribute((origDataset));

        treenode *left = new treenode(2*nodeId+1);
        treenode *right = new treenode(2*nodeId+2);
        for(int i=0; i<treeNode[nodeId]->dataPointIndex.size(); i++){
            
            if(origDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]]->attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
                left->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
            }
            else{
                right->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
            }

        }
     	left->parentAdd = treeNode[nodeId];
      	left->parentId = nodeId;
      	treeNode[nodeId]->lChildAdd = left;
      	treeNode[nodeId]->lChildId = 2*nodeId+1;
      	treeNode[2*nodeId+1] = left;
       
      	right->parentAdd = treeNode[nodeId];
      	right->parentId = nodeId;
      	treeNode[nodeId]->rChildAdd = right;
      	treeNode[nodeId]->rChildId = 2*nodeId+2;
      	treeNode[2*nodeId+2] = right;

        treeNode[nodeId]->isActive = bool(1);
    }
    
}

void itree::additionIncUpdateTree(dataset &additionDataset,dataset &completeDataset, vector<int> &sample){

    for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){
    	if(treeNode[nodeId] == nullptr){continue;}

    	
        if(nodeId == 0){
            treeNode[nodeId]->addedPointIndex.resize(sample.size());
            for(int in = 0; in < sample.size(); in++){
                treeNode[nodeId]->addedPointIndex[in] = sample.at(in);
            }
        }
       
        treeNode[nodeId]->nodeHeight = (int)(log2((double)(nodeId+1)));
        if(treeNode[nodeId]->addedPointIndex.size() > 1){
            if(treeNode[nodeId]->isLeaf && treeNode[nodeId]->nodeHeight < maxHeight){
		
                treeNode[nodeId]->isLeaf = bool(0);
                treeNode[nodeId]->splitAttribute = rand()%additionDataset.getnumAttributes();
                treeNode[nodeId]->splitValue = treeNode[nodeId]->splitPointOfChoosenAttribute((completeDataset));
		    treenode *left = new treenode(2*nodeId+1);
		     treenode *right = new treenode(2*nodeId+2);
                    for(int i=0; i<treeNode[nodeId]->dataPointIndex.size(); i++){
                         
                        if(completeDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]]->attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
                        
                            left->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
                        }
                        else{
                            
                            right->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
                            
                        }
                    }
                    left->nodeMass = left->dataPointIndex.size();
		     left->parentAdd = treeNode[nodeId];
		     left->parentId = nodeId;
		     treeNode[nodeId]->lChildAdd = left;
		     treeNode[nodeId]->lChildId = 2*nodeId+1;
		     treeNode[2*nodeId+1] = left;
		     treeNode[2*nodeId+1]->isLeaf = bool(1); ///to be cheked
		     treeNode[2*nodeId+1]->isActive = bool(1);
                    
                    right->nodeMass = right->dataPointIndex.size();
                    right->parentAdd = treeNode[nodeId];
                    right->parentId = nodeId;
                    treeNode[nodeId]->rChildAdd = right;
                    treeNode[nodeId]->rChildId = 2*nodeId+2;
                    treeNode[2*nodeId+2] = right;
                    treeNode[2*nodeId+2]->isLeaf = bool(1); ///to be cheked
                    treeNode[2*nodeId+2]->isActive = bool(1);
                    
            }
            else if(treeNode[nodeId]->nodeHeight == maxHeight){
                continue;
            }

            for(int i=0; i<treeNode[nodeId]->addedPointIndex.size(); i++){
               
                if(additionDataset.dataPoints[treeNode[nodeId]->addedPointIndex[i]]->attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
                    treeNode[2*nodeId+1]->addedPointIndex.push_back(treeNode[nodeId]->addedPointIndex[i]);
                }
                else{
                    treeNode[2*nodeId+2]->addedPointIndex.push_back(treeNode[nodeId]->addedPointIndex[i]);
                }
            }
        }
        else{
            continue;
        }

 
        }

    }



void itree::delIncUpdateTree(dataset &delPointsDataset){
	
    
     for(int in = 0; in<maxNumberOfNodes;in++){
    	if(treeNode[in] == nullptr){continue;}
        treeNode[in]->deletedPointIndex.resize(0);
    }
    
    for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){ 	
    	
    	if(treeNode[nodeId] == nullptr){continue;}
    	numTotalNodes++;
    	 
    	if(treeNode[nodeId]->nodeVanishes){
    		if(!treeNode[nodeId]->isLeaf){
        		if(treeNode[2*nodeId+1] != nullptr){treeNode[2*nodeId+1]->nodeVanishes = bool(1);}
        		if(treeNode[2*nodeId+2] != nullptr){treeNode[2*nodeId+2]->nodeVanishes = bool(1);}
        		
        	}
        	delete treeNode[nodeId];
        	treeNode[nodeId] = nullptr;
        	continue;
        }
        if(nodeId == 0){
    	    int noInst = delPointsDataset.getnumInstances();
            treeNode[nodeId]->deletedPointIndex.resize(noInst);
            map<int, point *>::iterator itr = delPointsDataset.dataPoints.begin();
            for(int in = 0; in < noInst; in++){
                treeNode[nodeId]->deletedPointIndex[in] = itr->first;
                itr++;
                
            }
        }
        treeNode[nodeId]->nodeHeight = (int)(log2((double)(nodeId+1)));
    	
    	if(treeNode[nodeId]->deletedPointIndex.size() == 0){
        	numUnchangedNodes++;
        	treeNode[nodeId]->isChanged = bool(0);
        	if(treeNode[nodeId]->isLeaf){
    			for(int in = 0; in < treeNode[nodeId]->dataPointIndex.size(); in++){
               	 	pointToNode[treeNode[nodeId]->dataPointIndex[in]] = nodeId;
            		}
    		}
        	continue;
        }
        if(treeNode[nodeId]->deletedPointIndex.size() == treeNode[nodeId]->dataPointIndex.size()){
        	
        	if(!treeNode[nodeId]->isLeaf){
        		if(treeNode[2*nodeId+1] != nullptr){treeNode[2*nodeId+1]->nodeVanishes = bool(1);}
        		if(treeNode[2*nodeId+2] != nullptr){treeNode[2*nodeId+2]->nodeVanishes = bool(1);}
        		
        	}
        	delete treeNode[nodeId];
        	treeNode[nodeId] = nullptr;
        	continue;
        }
        treeNode[nodeId]->nodeMass -= treeNode[nodeId]->deletedPointIndex.size(); 
    	
    	if(treeNode[nodeId]->isLeaf){
    		for(int i=treeNode[nodeId]->deletedPointIndex.size()-1 ; i >= 0;i--){
    			treeNode[nodeId]->dataPointIndex.erase(lower_bound(treeNode[nodeId]->dataPointIndex.begin(), treeNode[nodeId]->dataPointIndex.end(), treeNode[nodeId]->deletedPointIndex[i]));
			
    		}
    		
    		for(int in = 0; in < treeNode[nodeId]->dataPointIndex.size(); in++){
                	pointToNode[treeNode[nodeId]->dataPointIndex[in]] = nodeId;
            	}
            	continue;
    	}
    	
    	for(int i=0; i<treeNode[nodeId]->deletedPointIndex.size(); i++){
            if(delPointsDataset.dataPoints[treeNode[nodeId]->deletedPointIndex[i]]->attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
                treeNode[2*nodeId + 1]->deletedPointIndex.push_back(treeNode[nodeId]->deletedPointIndex[i]);
            }
            else{
                treeNode[2*nodeId + 2]->deletedPointIndex.push_back(treeNode[nodeId]->deletedPointIndex[i]);
            }
            
        }
       	
    }
  
   
    
}


void itree::computeNodeMassforTree(dataset &origDataset){
    
    for(int in = 0; in<maxNumberOfNodes;in++){
    	if(treeNode[in] == nullptr){continue;}
        treeNode[in]->dataPointIndex.resize(0);
    }
   for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){
         
    	 if(treeNode[nodeId] == nullptr){continue;}
    	 
        if(nodeId == 0){
            treeNode[nodeId]->dataPointIndex.resize(origDataset.getnumInstances());
            for(int in = 0; in < origDataset.getnumInstances(); in++){
                treeNode[nodeId]->dataPointIndex[in] = in;
            }
        }
        

        treeNode[nodeId]->nodeMass = treeNode[nodeId]->dataPointIndex.size();
	
        
        if(treeNode[nodeId]->isLeaf){
            for(int in = 0; in < treeNode[nodeId]->dataPointIndex.size(); in++){
                pointToNode[treeNode[nodeId]->dataPointIndex[in]] =nodeId;
                
            }
          
            continue;
        }
        for(int i=0; i<treeNode[nodeId]->dataPointIndex.size(); i++){
            
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
       
    }
}


void itree::additionIncComputeNodeMassforEachTree(dataset &additionDataset, dataset &completeDataset){
    int numOfInstancesInPointToNode = pointToNode.size();
    

    for(int in = 0; in<maxNumberOfNodes;in++){
    	if(treeNode[in] == nullptr){continue;}
        treeNode[in]->addedPointIndex.resize(0);
    }


    for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){
         if(treeNode[nodeId] == nullptr){continue;}
      	 numTotalNodes++;


        if(nodeId == 0){
            treeNode[nodeId]->addedPointIndex.resize(additionDataset.getnumInstances());
            for(int in = 0; in < additionDataset.getnumInstances(); in++){
                treeNode[nodeId]->addedPointIndex[in] = in;
                
            }
        }
        
        
        if(treeNode[nodeId]->addedPointIndex.size() == 0){
        	numUnchangedNodes++;
        	treeNode[nodeId]->isChanged = bool(0);
        	continue;
        }


        
       

        treeNode[nodeId]->nodeMass += treeNode[nodeId]->addedPointIndex.size();
        if(treeNode[nodeId]->isLeaf){
        	 for(int i= 0; i<treeNode[nodeId]->addedPointIndex.size(); i++){
            		treeNode[nodeId]->dataPointIndex.push_back((treeNode[nodeId]->addedPointIndex[i])+numOfInstancesInPointToNode);
                 }
            	for(int in = 0; in < treeNode[nodeId]->dataPointIndex.size(); in++){
              		pointToNode[treeNode[nodeId]->dataPointIndex[in]] = nodeId;
               	
            	}
           
            	continue;
        }



        for(int i=0; i<treeNode[nodeId]->addedPointIndex.size(); i++){
            
            if(additionDataset.dataPoints[treeNode[nodeId]->addedPointIndex[i]]->attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
                treeNode[2*nodeId + 1]->addedPointIndex.push_back(treeNode[nodeId]->addedPointIndex[i]);
            }
            else{
                treeNode[2*nodeId + 2]->addedPointIndex.push_back(treeNode[nodeId]->addedPointIndex[i]);
            }
        }
        
        if(treeNode[nodeId]->addedPointIndex.size() == 0){
        	numUnchangedNodes++;
        	treeNode[nodeId]->isChanged = bool(1);
        	continue;
        }

        treeNode[nodeId]->addedPointIndex.clear();

    }
}




int itree::getMaxHeight(){
    return maxHeight;
}


int itree::getMaxNumberOfNodes(){
    return maxNumberOfNodes;
}



