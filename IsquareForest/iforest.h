#ifndef IFOREST_H
#define IFOREST_H
#include "dataset.h"
#include "itree.h"
class iforest
{
    public:
    	int avgNumUnchangedNodes;
    	int avgNumTotalNodes;
        iforest();
        iforest(const int &);
        virtual ~iforest();

        void createiForest(dataset &, const double &, const int &);
        void additionIncForestUpdate(dataset &, dataset &, const double &, const int &);
        void delIncForestUpdate(dataset & );
        void computeNodeMass(dataset &);
        void additionIncComputeNodeMassforForest(dataset &, dataset &);
        void massMatrixComputation();
        double massMatrixComputation(int,int,int);
        int computeLCA(int,int);
        vector<double> pairwiseMass;
        vector<itree*> iTrees;
        int getnumiTrees();
        void storeNodesAndTreesInFile();
        void readNodesAndTreesfromFile();
        void readMassFile(dataset & origDataset);

    protected:

    private:
    int numiTrees;
    int maxNumOfNodes;
    int treeHeight;
};

#endif // IFOREST_H
