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
            else{
            	treeNode[nodeId]->parentAdd = treeNode[nodeId];
            	treeNode[nodeId]->parentId = nodeId;
            }
        }
        
        if(treeNode[nodeId] == nullptr){continue;}
        //treeNode[nodeid]->nodeMass = treeNode[nodeid]->dataPointIndex.size(); //Not required at time of tree creation.

        treeNode[nodeId]->nodeHeight = (int)(log2((double)(nodeId+1)));
        //cout<<"nodeId"<<nodeid<<endl;

        if(treeNode[nodeId]->dataPointIndex.size() <= 1 || treeNode[nodeId]->nodeHeight == maxHeight){
        	treeNode[nodeId]->isLeaf = bool(1);
        	treeNode[nodeId]->isActive = bool(1);
                //treeNode[nodeid]->parentAdd = treeNode[(nodeid-1)/2];
            
            continue;
        }

        //treeNode[nodeId]->splitAttribute = rand()%origDataset.getnumAttributes();
        treeNode[nodeId]->splitValue = treeNode[nodeId]->splitPointOfChoosenAttribute((origDataset));

        treenode *left = new treenode(2*nodeId+1);
        treenode *right = new treenode(2*nodeId+2);
        for(int i=0; i<treeNode[nodeId]->dataPointIndex.size(); i++){
            if(origDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]].attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
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

    //cout<<"complete dataset"<< completeDataset.getnumInstances()<<endl;
    for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){
    	if(treeNode[nodeId] == nullptr){continue;}
    	//cout<<"node"<<nodeId<<endl;
        //treeNode[nodeId]->addedPointIndex.resize((0));
        
        if(nodeId == 0){
            treeNode[nodeId]->addedPointIndex.resize(sample.size());
            for(int in = 0; in < sample.size(); in++){
                treeNode[nodeId]->addedPointIndex[in] = sample.at(in);
            }
        }
       // cout<<nodeId<<" hhhh"<<endl;
        //treeNode[nodeId]->nodeMass = treeNode[nodeId]->addedPointIndex.size();

        treeNode[nodeId]->nodeHeight = (int)(log2((double)(nodeId+1)));
        if(treeNode[nodeId]->addedPointIndex.size() > 1){
            if(treeNode[nodeId]->isLeaf && treeNode[nodeId]->nodeHeight < maxHeight){
		
                treeNode[nodeId]->isLeaf = bool(0);
                treeNode[nodeId]->splitAttribute = rand()%additionDataset.getnumAttributes();
                //cout<<"split attribute"<<treeNode[nodeId]->splitAttribute<<endl;
                treeNode[nodeId]->splitValue = treeNode[nodeId]->splitPointOfChoosenAttribute((completeDataset));
		     treenode *left = new treenode(2*nodeId+1);
		     treenode *right = new treenode(2*nodeId+2);
                    for(int i=0; i<treeNode[nodeId]->dataPointIndex.size(); i++){
                        //treeNode[2*nodeId+2]->dataPointIndex;//.push_back(1);
                        //cout<< "fffff  "<<treeNode[2*nodeId+2]->dataPointIndex.size()<<endl;
			
                        if(completeDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]].attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
                            //treeNode[2*nodeId+1]->dataPointIndex.resize(0);
                            left->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
                        }
                        else{
                            //treeNode[2*nodeId+2]->dataPointIndex.resize(0);
                            //cout<< "fffff  "<<treeNode[2*nodeId+2]->dataPointIndex[0]<<endl;
                            right->dataPointIndex.push_back(treeNode[nodeId]->dataPointIndex[i]);
                            //cout<<"i= "<<nodeId<<endl;
                        }
                    }
		     left->parentAdd = treeNode[nodeId];
		     left->parentId = nodeId;
		     treeNode[nodeId]->lChildAdd = left;
		     treeNode[nodeId]->lChildId = 2*nodeId+1;
		     treeNode[2*nodeId+1] = left;
		     treeNode[2*nodeId+1]->isLeaf = bool(1); ///to be cheked
		     treeNode[2*nodeId+1]->isActive = bool(1);
                    
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
                if(additionDataset.dataPoints[treeNode[nodeId]->addedPointIndex[i]].attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
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

void itree::delIncUpdateTree(vector<int> delPoints){
//cout<<"ttttttttrrrrrrrrrrreeeeeeeeeeee"<<endl;
    pointToNode.clear();
    //cout<<"deleteTree"<<endl;
    for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){ 	
    	
    	if(treeNode[nodeId] == nullptr || treeNode[nodeId]->deletionProcessed){continue;}
    	treeNode[nodeId]->nodeHeight = (int)(log2((double)(nodeId+1)));
        if(nodeId==0){
            for(int i=0;i<delPoints.size();i++){
            	//cout<<"deleted point "<<delPoints[i]<<" ";
                treeNode[nodeId]->deletedPointIndex.push_back(delPoints[i]);
                //cout<<"deleted point "<<treeNode[nodeId]->deletedPointIndex[i]<<endl;
            }
            //cout<<"del point at node 0"<<treeNode[nodeId]->deletedPointIndex.size()<<endl;
            pointToNode.resize((treeNode[nodeId]->dataPointIndex.size())-(treeNode[nodeId]->deletedPointIndex.size()));
            //cout<<pointToNode.size()<<"no of points"<<endl;
            treeNode[nodeId]->deletionHappened = bool(1);
        }
       
        cout<<"nodeId"<<nodeId<<" "<<treeNode[nodeId]->dataPointIndex.size()<<" "<<treeNode[nodeId]->deletedPointIndex.size()<<endl;
        treeNode[nodeId]->deletionProcessed = bool(1);
        //cout<<"del point at node "<<treeNode[nodeId]->deletedPointIndex.size()<<endl;
        if(treeNode[nodeId]->deletedPointIndex.size() == 0){continue;}
        if(treeNode[nodeId]->deletedPointIndex.size() == treeNode[nodeId]->dataPointIndex.size()){
        		if(treeNode[nodeId]->parentAdd->parentAdd->rChildAdd == treeNode[nodeId]->parentAdd){
        			if(treeNode[nodeId]->parentAdd->lChildAdd == treeNode[nodeId]){
        			//cout<<"1111"<<endl;
        				treeNode[nodeId]->parentAdd->parentAdd->rChildAdd = treeNode[nodeId]->parentAdd->rChildAdd;
        				//treeNode[nodeId]->parentAdd->rChildAdd->updateIds(treeNode[nodeId]->parentAdd->nodeId);
        				treeNode[nodeId]->parentAdd->rChildAdd->parentAdd = treeNode[nodeId]->parentAdd->parentAdd;
        				treenode * ref = treeNode[nodeId]->parentAdd->parentAdd->rChildAdd;
        				delete treeNode[(nodeId-1)/2];
        				treeNode[(nodeId-1)/2] = ref;
        				deleteBranch(nodeId);
        				queue<int> qu;
        				queue<int> & refqu = qu;
        				refqu.push((nodeId-1)/2);
        				while(!refqu.empty()){
	        				updateIds(refqu, refqu.front());
	        				refqu.pop();
        				}
        				nodeId = (nodeId-1)/2;
        				nodeId--;
        				//cout<<"jjjjjjjjjjjjjjjjjjjjjuuuuummmmmpppppeeedddd from "<<nodeId<<" to "<<(nodeId-1)/2<<endl;
        				
        			}
        			else{
        			//cout<<"222222"<<endl;
        				treeNode[nodeId]->parentAdd->parentAdd->rChildAdd = treeNode[nodeId]->parentAdd->lChildAdd;
        				//treeNode[nodeId]->parentAdd->lChildAdd->updateIds(treeNode[nodeId]->parentAdd->nodeId);
        				treeNode[nodeId]->parentAdd->lChildAdd->parentAdd = treeNode[nodeId]->parentAdd->parentAdd;
        				treenode * ref = treeNode[nodeId]->parentAdd->parentAdd->rChildAdd;
        				//cout<<ref->nodeId<<" "<<ref->parentAdd->nodeId<<" "<<treeNode[nodeId]->parentAdd->parentAdd->nodeId<<" "<<treeNode[nodeId]->parentAdd->nodeId<<endl;
        				delete treeNode[(nodeId-1)/2];
        				treeNode[(nodeId-1)/2] = ref;
        				//cout<<treeNode[(nodeId-1)/2]->nodeId<<" "<<ref->nodeId<<endl;
        				deleteBranch(nodeId);
        				queue<int> qu;
        				queue<int> & refqu = qu;
        				refqu.push((nodeId-1)/2);
        				//cout<<"222222"<<endl;
        				while(!refqu.empty()){
        					//cout<<"222222"<<endl;
	        				updateIds(refqu, refqu.front());
	        				//cout<<"222222"<<endl;
	        				refqu.pop();
        				}
        			}
        		}
        		else{
        		
        			if(treeNode[nodeId]->parentAdd->lChildAdd == treeNode[nodeId]){
        			//cout<<"33333"<<endl;
        				treeNode[nodeId]->parentAdd->parentAdd->lChildAdd = treeNode[nodeId]->parentAdd->rChildAdd;
        				//treeNode[nodeId]->parentAdd->rChildAdd->updateIds(treeNode[nodeId]->parentAdd->nodeId);
        				treeNode[nodeId]->parentAdd->rChildAdd->parentAdd = treeNode[nodeId]->parentAdd->parentAdd;
        				treenode * ref = treeNode[nodeId]->parentAdd->parentAdd->lChildAdd;
        				delete treeNode[(nodeId-1)/2];
        				treeNode[(nodeId-1)/2] = ref;
        				deleteBranch(nodeId);
        				queue<int> qu;
        				queue<int> & refqu = qu;
        				refqu.push((nodeId-1)/2);
        				while(!refqu.empty()){
	        				updateIds(refqu, refqu.front());
	        				refqu.pop();
        				}
        				nodeId =(nodeId-1)/2;
        				nodeId--;
        				cout<<"jjjjjjjjjjjjjjjjjjjjjuuuuummmmmpppppeeedddd from "<<nodeId<<" to "<<(nodeId-1)/2<<endl;
        			}
        			else{
        			//cout<<"44444"<<endl;
        				treeNode[nodeId]->parentAdd->parentAdd->lChildAdd = treeNode[nodeId]->parentAdd->lChildAdd;
        				//treeNode[nodeId]->parentAdd->lChildAdd->updateIds(treeNode[nodeId]->parentAdd->nodeId);
        				treeNode[nodeId]->parentAdd->lChildAdd->parentAdd = treeNode[nodeId]->parentAdd->parentAdd;
        				treenode * ref = treeNode[nodeId]->parentAdd->parentAdd->lChildAdd;
        				delete treeNode[(nodeId-1)/2];
        				treeNode[(nodeId-1)/2] = ref;
        				deleteBranch(nodeId);
        				queue<int> qu;
        				queue<int> & refqu = qu;
        				refqu.push((nodeId-1)/2);
        				while(!refqu.empty()){
        				//cout<<"rrrrrrr"<<endl;
        				//cout<<refqu.front()<<"front elememt"<<endl;
	        				updateIds(refqu, refqu.front());
	        				refqu.pop();
        				}
        			}
        		}
        		continue;	
        	}
        
      // cout<<"nodeId"<<nodeId<<endl;
        for(int i=0;i<treeNode[nodeId]->deletedPointIndex.size();i++){
        	//remove(treeNode[nodeId]->dataPointIndex.begin(), treeNode[nodeId]->dataPointIndex.end(), treeNode[nodeId]->deletedPointIndex[i]);
        	//cout<<"removed point"<<treeNode[nodeId]->deletedPointIndex[i]<<endl;
        	
        	treeNode[nodeId]->dataPointIndex.erase(remove(treeNode[nodeId]->dataPointIndex.begin(), treeNode[nodeId]->dataPointIndex.end(), treeNode[nodeId]->deletedPointIndex[i]));
        }
        //cout<<"nodeId"<<nodeId<<endl;
        treeNode[nodeId]->nodeMass = treeNode[nodeId]->dataPointIndex.size();//-treeNode[nodeId]->deletedPointIndex.size();
        //cout<<"nodeIdMAss"<<treeNode[nodeId]->nodeMass<<endl;
        if(treeNode[nodeId]->isLeaf){
            //cout<<"nodeId"<<nodeId<<" is leaf of size"<<treeNode[nodeId]->dataPointIndex.size()<<endl;
            for(int in = 0; in < treeNode[nodeId]->dataPointIndex.size(); in++){
                pointToNode[treeNode[nodeId]->dataPointIndex[in]] = nodeId;
                //cout<<"ffff"<<treeNode[nodeId]->dataPointIndex[in]<<" ";
            }
            continue;
        }
//cout<<"nodeId"<<nodeId<<endl;
//if(treeNode[2*nodeId+1] == nullptr){cout<<"left null"<<endl;}

//if(treeNode[2*nodeId+2] == nullptr){cout<<"right null"<<endl;}
        for(int dPt=0; dPt<treeNode[nodeId]->deletedPointIndex.size(); dPt++){
        //cout<<"split  deletePointIndex "<<dPt<<endl;
            /*if(treeNode[2*nodeId+1] != nullptr ){
            //cout<<"leefffttt"<<endl;*/
            	if(find(treeNode[2*nodeId+1]->dataPointIndex.begin(), treeNode[2*nodeId+1]->dataPointIndex.end(), treeNode[nodeId]->deletedPointIndex[dPt]) !=treeNode[2*nodeId+1]->dataPointIndex.end()){
                	treeNode[2*nodeId+1]->deletedPointIndex.push_back(treeNode[nodeId]->deletedPointIndex[dPt]);
                }
            	else //(find(treeNode[2*nodeId+2]->dataPointIndex.begin(), treeNode[2*nodeId+2]->dataPointIndex.end(), treeNode[nodeId]->deletedPointIndex[dPt]) !=treeNode[2*nodeId+1]->dataPointIndex.end())
            	{
                	treeNode[2*nodeId+2]->deletedPointIndex.push_back(treeNode[nodeId]->deletedPointIndex[dPt]);
                } 
            //}
        }
        //cout<<"nodeId"<<nodeId<<endl;
        //treeNode[2*nodeId+1] = treeNode[nodeId]->lChildAdd;
        //treeNode[2*nodeId+2] = treeNode[nodeId]->rChildAdd;
        
    }
    /*sort(delPoints.begin(), delPoints.end());
    for(int i=delPoints.size()-1; i>=0; i--){
    	find(pointToNode.begin(), pointToNode.end(), del)
    	pointToNode.erase(j);
    }*/
}





void itree::deleteBranch(int nodeId){
	if(treeNode[nodeId]->isLeaf){delete treeNode[nodeId]; treeNode[nodeId] = nullptr;}
	else{
		treenode *left = treeNode[nodeId]->lChildAdd;
		treenode *right = treeNode[nodeId]->rChildAdd;
		delete treeNode[nodeId];
		treeNode[nodeId] = nullptr;
		deleteBranch(left->nodeId);
		deleteBranch(right->nodeId);
	}
}

void itree :: updateIds(queue<int> & refqu, int cn){
	//cout<<treeNode[cn]->isLeaf<<endl;
	//cout<<"111111updateIIDDSSS"<<endl;
	if(treeNode[cn]->isLeaf){treeNode[treeNode[cn]->nodeId] = nullptr;treeNode[cn]->nodeId = cn;}
	else{
		//cout<<"updateIIDDSSS"<<endl;
		//cout<<"cn="<<cn<<"treenode[cn]->nodeID"<<treeNode[cn]->nodeId<<endl;
		
		//treeNode[treeNode[cn]->nodeId] = nullptr;
		//cout<<"updateIIDDSSS"<<endl;
		treeNode[cn]->nodeId = cn;
		
		//cout<<treeNode[cn]->lChildAdd->nodeId << "means left child is null"<<endl;
		//cout<<treeNode[cn]->rChildAdd->nodeId << "means right child is null"<<endl;
		
		treeNode[cn]->parentId = (cn-1)/2;
		
		treeNode[cn]->lChildId = 2*cn+1;
		
		treeNode[cn]->rChildId = 2*cn+2;
		
		refqu.push(2*cn+1);
		refqu.push(2*cn+2);
		//cout<<treeNode
		//cout<<treeNode[cn]->lChildAdd->nodeId << "means left child is null"<<endl;
		//cout<<treeNode[cn]->rChildAdd->nodeId << "means right child is null"<<endl;
		/*if(treeNode[cn]->lChildAdd == nullptr){cout<<treeNode[cn]->lChildId << "means left child is null"<<endl;}
		if(treeNode[cn]->rChildAdd == nullptr){cout<<treeNode[cn]->rChildId << "means right child is null"<<endl;}*/
		treeNode[2*cn+1] = treeNode[cn]->lChildAdd;
		treeNode[2*cn+2] = treeNode[cn]->rChildAdd;//cout<<"updateIIDDSSS"<<endl;
		treeNode[treeNode[cn]->lChildAdd->nodeId] = nullptr;//cout<<"updateIIDDSSS"<<endl;
		treeNode[treeNode[cn]->rChildAdd->nodeId] = nullptr;
	}
}




















void itree::computeNodeMassforTree(dataset &origDataset){
    pointToNode.resize(origDataset.getnumInstances());
    
    for(int in = 0; in<maxNumberOfNodes;in++){
    	if(treeNode[in] == nullptr){continue;}
        treeNode[in]->dataPointIndex.resize(0);
    }
    cout<<"max no of nodes"<<maxNumberOfNodes<<endl;
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
                pointToNode[treeNode[nodeId]->dataPointIndex[in]] = nodeId;
            }
            //treeNode[nodeId]->nodeMass = treeNode[nodeId]->dataPointIndex.size();

            continue;
        }
//cout<<"nodeID"<<nodeId<<endl;
//cout<<"nodeisLeaf"<<treeNode[nodeId]->isLeaf<<endl;
        for(int i=0; i<treeNode[nodeId]->dataPointIndex.size(); i++){
            
            if(origDataset.dataPoints[treeNode[nodeId]->dataPointIndex[i]].attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
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


void itree::additionIncComputeNodeMassforEachTree(dataset &additionDataset, dataset &completeDataset){
    int numOfInstancesInPointToNode = pointToNode.size();
    pointToNode.resize(completeDataset.getnumInstances());

    for(int in = 0; in<maxNumberOfNodes;in++){
    	if(treeNode[in] == nullptr){continue;}
        treeNode[in]->addedPointIndex.resize(0);
    }


    for(int nodeId = 0; nodeId < maxNumberOfNodes; nodeId++){
         //cout<<"print"<<nodeId<<endl;
         if(treeNode[nodeId] == nullptr){
            continue;
        }



        if(nodeId == 0){
            treeNode[nodeId]->addedPointIndex.resize(additionDataset.getnumInstances());
            for(int in = 0; in < additionDataset.getnumInstances(); in++){
                treeNode[nodeId]->addedPointIndex[in] = in;
                //cout<<treeNode[nodeId]->dataPointIndex[in]<<" ";
            }
        }

        //cout<<"cccooppppp"<<nodeId<<" "<<treeNode[nodeId]->nodeMass<<"  "<<treeNode[nodeId]->dataPointIndex.size()<<"  "<<treeNode[nodeId]->addedPointIndex.size()<<endl;

        for(int i= 0; i<treeNode[nodeId]->addedPointIndex.size(); i++){
            treeNode[nodeId]->dataPointIndex.push_back((treeNode[nodeId]->addedPointIndex[i])+numOfInstancesInPointToNode);
            //cout<<(treeNode[nodeId]->addedPointIndex[i])<<"ddddd"<<endl;
            //cout<<numOfInstancesInPointToNode<<"ddddd"<<endl;
        }

        treeNode[nodeId]->nodeMass = treeNode[nodeId]->dataPointIndex.size();
        //cout<<"mass of tree nodes "<<nodeId<<" "<<treeNode[nodeId]->nodeMass<<endl;
        if(treeNode[nodeId]->isLeaf){
            for(int in = 0; in < treeNode[nodeId]->dataPointIndex.size(); in++){
                pointToNode[treeNode[nodeId]->dataPointIndex[in]] = nodeId;
                //cout<<"ffff"<<treeNode[nodeId]->dataPointIndex[in]<<" ";
            }
            //treeNode[nodeId]->nodeMass += treeNode[nodeId]->dataPointIndex.size();
            //cout<<"In tree - leaf Node"<<nodeId<<":"<<treeNode[nodeId].nodeMass<<endl;
            //treeNode[nodeId]->addedPointIndex.clear();
            continue;
        }



        for(int i=0; i<treeNode[nodeId]->addedPointIndex.size(); i++){
            if(additionDataset.dataPoints[treeNode[nodeId]->addedPointIndex[i]].attributes[treeNode[nodeId]->splitAttribute]<treeNode[nodeId]->splitValue){
                treeNode[2*nodeId + 1]->addedPointIndex.push_back(treeNode[nodeId]->addedPointIndex[i]);
            }
            else{
                treeNode[2*nodeId + 2]->addedPointIndex.push_back(treeNode[nodeId]->addedPointIndex[i]);
            }
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



