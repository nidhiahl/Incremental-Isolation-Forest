#ifndef ITREE_H
#define ITREE_H
#include "dataset.h"
#include "treenode.h"
#include <queue>
#include <map>
#include <boost/serialization/map.hpp>

class itree
{
    public:
    	int numUnchangedNodes;
    	int numTotalNodes;
        itree();
        itree(int);
        virtual ~itree();
        void initTree(int, int);
        void createTree(dataset &, vector<int> &, int);
        void additionIncUpdateTree(dataset &, dataset &,vector<int> &);
        void delIncUpdateTree(dataset & );
        void computeNodeMassforTree(dataset &);
        void additionIncComputeNodeMassforEachTree(dataset &,dataset &);
        void deleteBranch(int);
        void updateIds(queue<int> &, int);
        vector<treenode*> treeNode;
        map<int,int> pointToNode;
        int getMaxNumberOfNodes();
        int getMaxHeight();

    protected:

    private:
    int maxHeight;
    int maxNumberOfNodes;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & maxHeight;
        ar & maxNumberOfNodes;
        ar & BOOST_SERIALIZATION_NVP(pointToNode);
    }

};

#endif // ITREE_H
