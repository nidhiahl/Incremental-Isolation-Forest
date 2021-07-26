#ifndef DATASET_H
#define DATASET_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

typedef struct point{
    double* attributes;
    int label;
}point;


class dataset
{
    public:
        dataset();
        dataset(const string&);
        dataset(const string&, const vector<int> &);
        virtual ~dataset();

        int getnumInstances();
        void setnumInstances(int);
        int getnumAttributes();
        vector<int> &getSample(int);
        map<int, point *> dataPoints;

    protected:

    private:
    int numInstances;
    int numAttributes;
};

#endif // DATASET_H
