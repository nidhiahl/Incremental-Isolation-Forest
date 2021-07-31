#ifndef DATASET_H
#define DATASET_H
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

typedef struct point{
    //double* attributes;
    double* attributes;
    int label;
}point;


class dataset
{
    public:
        dataset();
        dataset(const string&);
        virtual ~dataset();

        int getnumInstances();
        int getnumAttributes();
        vector<int> &getSample(int);
        //point* dataPoints;
	map<int, point *> dataPoints;

    protected:

    private:
    int numInstances;
    int numAttributes;
};

#endif // DATASET_H
