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
        //treenode(int ni, int pi, int li,int ri,treenode *pa, treenode *la, treenode *ra, vector<int> dpi, vector<int> depi, vector<int> api, int sa, double sv,int nm, int nh,bool il, bool ia, bool dh, bool nv): nodeId(ni),parentId(pi), lChildId(li), rChildId(ri), parentAdd(pa), lChildAdd(la), rChildAdd(ra), dataPointIndex(dpi), deletedPointIndex(depi), addedPointIndex(api), splitAttribute(sa), splitValue(sv), nodeMass(nm), nodeHeight(nh), isLeaf(il), isActive(ia), deletionHappened(dh), nodeVanishes(nv){}
        virtual ~treenode();
        void updateIds(int);

        int nodeId;
        //int parentId;
        //int lChildId;
        //int rChildId;
        //treenode *parentAdd;
        //treenode *lChildAdd;
        //treenode *rChildAdd;
        vector<int> dataPointIndex;
        int splitAttribute;
        double splitValue;
        int nodeMass;
        int nodeHeight;
        bool isLeaf;
        //bool isActive;
        double splitPointOfChoosenAttribute(dataset &);


    protected:

    private:
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & nodeId;
            //ar & parentId;
            //ar & lChildId;
            //ar & rChildId;
            //ar & parentAdd;
            //ar & lChildAdd;
            //ar & rChildAdd;
            /*ar & parentId;
            ar & lChildId;
            ar & rChildId;*/
            ar & BOOST_SERIALIZATION_NVP(dataPointIndex);
            //ar & BOOST_SERIALIZATION_NVP(deletedPointIndex);
            //ar & BOOST_SERIALIZATION_NVP(addedPointIndex);
            /*ar & dataPointIndex;
            ar & deletedPointIndex;
            ar & addedPointIndex;*/
            ar & splitAttribute;
            ar & splitValue;
            ar & nodeMass;
            ar & nodeHeight;
            ar & isLeaf;
            //ar & isActive;

        }

};

#endif // TREENODE_H
