#include <iostream>
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include "dataset.h"
#include "iforest.h"
#include <string>
#include <cmath>

int main(int argc, char* argv[]){
    const string staticMassFile = argv[1];
    const string incrementalMassFile = argv[2];
    vector<double> staticPairwiseMassfromFile;
    vector<double> incrementalPairwiseMassfromFile;
    ifstream staticMassStream(staticMassFile.c_str(),ios::in|ios::binary);
    if(!staticMassStream)
    {
      cout<<"Cannot open the input file: "<<staticMassFile<<endl;
      exit(0);
    }
    ifstream incrementalMassStream(incrementalMassFile.c_str(),ios::in|ios::binary);
    if(!incrementalMassStream)
    {
      cout<<"Cannot open the input file: "<<incrementalMassFile<<endl;
      exit(0);
    }
    int i = 0;
    double staticMean = 0;
    double stdDeviation = 0;
    double variance = 0;
    int numOfPairs = 0;
/*
    {
    boost::archive::text_iarchive staticMassFileAr(staticMassStream);
    staticMassFileAr & BOOST_SERIALIZATION_NVP(i);
    for(int point1 = 0; point1<i; point1++){
    //while(!(massMatrixFile == "\n")){
        staticMassFileAr & BOOST_SERIALIZATION_NVP(staticPairwiseMassfromFile);
        for(int point2 = 0; point2<=point1; point2++){
            //cout<<"dfdfdfdfdfdfdfddf"<<point1<<" "<<point2<<endl;
            staticMean += staticPairwiseMassfromFile[point2];
            numOfPairs++;
        }
        staticPairwiseMassfromFile.clear();
    }
    staticMean /= numOfPairs;
    }
    {
    
    boost::archive::text_iarchive incrementalMassFileAr(incrementalMassStream);
    incrementalMassFileAr & BOOST_SERIALIZATION_NVP(i);
    //cout<<"meeaannnn"<<staticMean<<endl;
    
    for(int point1 = 0; point1<i; point1++){
    	    incrementalMassFileAr & BOOST_SERIALIZATION_NVP(incrementalPairwiseMassfromFile);

            for(int point2 = 0; point2<=point1; point2++){
            	 variance += pow(incrementalPairwiseMassfromFile[point2] - staticMean, 2);
                //cout<<point1<<" "<<point2<<": "<< pairwiseMassfromFile[point2]<<endl;
            }
            incrementalPairwiseMassfromFile.clear();
    }
    variance /= numOfPairs;
    stdDeviation = sqrt(variance);
    }*/
    
    /*{
    boost::archive::binary_iarchive staticMassFileAr(staticMassStream);
    staticMassFileAr & BOOST_SERIALIZATION_NVP(i);
    for(int point1 = 0; point1<i; point1++){
    //while(!(massMatrixFile == "\n")){
        staticMassFileAr & BOOST_SERIALIZATION_NVP(staticPairwiseMassfromFile);
        for(int point2 = 0; point2<=point1; point2++){
            //cout<<"dfdfdfdfdfdfdfddf"<<point1<<" "<<point2<<endl;
            staticMean += staticPairwiseMassfromFile[point2];
            numOfPairs++;
        }
        staticPairwiseMassfromFile.clear();
    }
    staticMean /= numOfPairs;
    }*/
    {
    
    boost::archive::binary_iarchive incrementalMassFileAr(incrementalMassStream);
    incrementalMassFileAr & BOOST_SERIALIZATION_NVP(i);
    boost::archive::binary_iarchive staticMassFileAr(staticMassStream);
    staticMassFileAr & BOOST_SERIALIZATION_NVP(i);
    
    //cout<<"meeaannnn"<<staticMean<<endl;
    
    for(int point1 = 0; point1<i; point1++){
    	    incrementalMassFileAr & BOOST_SERIALIZATION_NVP(incrementalPairwiseMassfromFile);
    	    staticMassFileAr & BOOST_SERIALIZATION_NVP(staticPairwiseMassfromFile);
        

            for(int point2 = 0; point2<=point1; point2++){
            	 variance += abs(((incrementalPairwiseMassfromFile[point2] - staticPairwiseMassfromFile[point2])/staticPairwiseMassfromFile[point2])*100);
                //cout<<point1<<" "<<point2<<": "<< pairwiseMassfromFile[point2]<<endl;
                numOfPairs++;
            }
            incrementalPairwiseMassfromFile.clear();
            staticPairwiseMassfromFile.clear();
    }
    variance /= numOfPairs;
    }
    
    
    
    
    cout<<variance<<endl;

    return 0;

}







