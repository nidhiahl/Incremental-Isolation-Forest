#include "treenode.h"
#include "itree.h"

treenode::treenode()
{
    splitAttribute = -1;
    isLeaf = bool(0);
    //isActive = bool(0);
    nodeId = -1;
    /*parentId = -1;
    lChildId = -1;
    rChildId = -1;*/
    nodeMass = 0;
    dataPointIndex.resize(0);
    /*parentAdd = nullptr;
    lChildAdd = nullptr;
    rChildAdd = nullptr;*/
}

treenode::treenode(int id){
    splitAttribute = -1;
    isLeaf = bool(0);
    //isActive = bool(0);
    nodeId = id;
    /*parentId = -1;
    lChildId = -1;
    rChildId = -1;*/
    nodeMass = 0;
    dataPointIndex.resize(0);
   /* parentAdd = nullptr;
    lChildAdd = nullptr;
    rChildAdd = nullptr;*/
}

treenode::~treenode()
{
    //dtor
}


double treenode::splitPointOfChoosenAttribute(dataset &origDataset){
	int maxVal = -999999;
	int minVal = 999999;
	int attempts = 0;
	while(attempts < 10){
		splitAttribute = rand()%origDataset.getnumAttributes();
		for( int i = 0; i < dataPointIndex.size(); i++){
			/*if(maxVal < origDataset.dataPoints[(dataPointIndex[i])].attributes[splitAttribute]){
				maxVal = origDataset.dataPoints[(dataPointIndex[i])].attributes[splitAttribute];
			}
        		if(minVal > origDataset.dataPoints[(dataPointIndex[i])].attributes[splitAttribute]){
        			minVal = origDataset.dataPoints[(dataPointIndex[i])].attributes[splitAttribute];
        		}*/
        		
        		if(maxVal < origDataset.dataPoints[(dataPointIndex[i])]->attributes[splitAttribute]){
				maxVal = origDataset.dataPoints[(dataPointIndex[i])]->attributes[splitAttribute];
			}
        		if(minVal > origDataset.dataPoints[(dataPointIndex[i])]->attributes[splitAttribute]){
        			minVal = origDataset.dataPoints[(dataPointIndex[i])]->attributes[splitAttribute];
        		}
        		
        		
        	}
        	attempts = attempts + 1;
        	double dataDiff = maxVal - minVal;
		if(dataDiff >= 0.0000000000000001){
			attempts = 10;
		}
	}
	//cout<<"diiffffeeerreeennncccee = "<<maxVal-minVal<<endl;
	return (minVal + ((double)rand()/RAND_MAX)*(maxVal-minVal));
}




































