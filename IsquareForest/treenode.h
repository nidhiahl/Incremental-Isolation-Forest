#ifndef TREENODE_H
#define TREENODE_H
#include <vector>
#include <map>
#include <boost/serialization/list.hpp>
#include "dataset.h"
using namespace std;
class treenode
{
    public:
        treenode();
        treenode(int);
        virtual ~treenode();
        void updateIds(int);

        int nodeId;
        int parentId;
        int lChildId;
        int rChildId;
        treenode *parentAdd;
        treenode *lChildAdd;
        treenode *rChildAdd;
        vector<int> dataPointIndex;
        vector<int> deletedPointIndex;
        vector<int> addedPointIndex;
        int splitAttribute;
        double splitValue;
        int nodeMass;
        int nodeHeight;
        bool isLeaf;
        bool isActive;
        bool isChanged;
        bool deletionHappened;
        bool deletionProcessed;
        //bool nodeDeleted;
        bool nodeVanishes;
        double splitPointOfChoosenAttribute(dataset &);


    protected:

    private:
        friend class boost::serialization::access;
        

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & nodeId;
            ar & parentId;
            ar & lChildId;
            ar & rChildId;
            ar & parentAdd;
            ar & lChildAdd;
            ar & rChildAdd;
            ar & BOOST_SERIALIZATION_NVP(dataPointIndex);
            ar & splitAttribute;
            ar & splitValue;
            ar & nodeMass;
            ar & nodeHeight;
            ar & isLeaf;
            ar & isActive;
            ar & deletionHappened;
            ar & nodeVanishes;
        }

        

};

#endif // TREENODE_H
