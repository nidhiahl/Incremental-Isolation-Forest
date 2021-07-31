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
        itree();
        itree(int);
        virtual ~itree();
        void initTree(int, int);
        void createTree(dataset &, vector<int> &, int);
        void additionIncUpdateTree(dataset &, dataset &,vector<int> &);
        void delIncUpdateTree(vector<int> );
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
    //int numInstancesStored;
    int maxHeight;
    int maxNumberOfNodes;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        //ar & numInstancesStored
        ar & maxHeight;
        ar & maxNumberOfNodes;
        ar & BOOST_SERIALIZATION_NVP(pointToNode);
    }

};

#endif // ITREE_H
