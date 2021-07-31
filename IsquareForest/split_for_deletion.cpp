#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <cstring>
#include <cassert>
#include <string>
#include <queue>
#include <list>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <unistd.h>
#include <ios>
#include <sstream>
#include <queue>
#include <sys/time.h>
#include <malloc.h>
#include <dirent.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include<bits/stdc++.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <string.h>
/**************************Takes command line argument as the dataset file**********************/
using namespace std;

int main(int argc, char* argv[])
{
 srand(time(0));
 string line;
 string word;
 string infile = argv[1];                //Complete data file.
 int numVersion = argv[2]; 
 ifstream df(infile.c_str());
 if(!df)
 {
   cout<<"Cannot open the input file: "<<infile<<endl;
   exit(0);
 }
 
 int numInst=0;
 int numAttr=0;
 
 int no_of_lines = 0;
 list<int> random_numbers;
 int blank;
 
 df>>numInst>>numAttr>>blank;
 
 cout <<fixed;
 cout <<setprecision(1);
 vector<float> increments = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};

for(int increment = 0; increment<increments.size(); increment++){
	for(int version = 0; version < numVersion; version++){
		df.seekg(0);
		getline(df,line);
		string iFile = infile+"Inc_"+to_string(increments[increment])+"_ver"+to_string(version);
		ofstream iF(iFile.c_str());
		int sizeIF = increments[increment] * numInst/100;
		iF<< sizeIF<<" "<<numAttr<<endl;
		string sFile = infile+"Static_"+to_string(increments[increment])+"_ver"+to_string(version);
		ofstream sF(sFile.c_str());
		int sizeSF = numInst - sizeIF;
		sF<<sizeSF <<" "<<numAttr<<endl;
		
		while(random_numbers.size()<sizeIF){
			int rn=rand()%numInst+1;
			random_numbers.remove(rn);
			random_numbers.push_back(rn);
		}
		random_numbers.sort();
		no_of_lines=0;
		while(getline(df,line)){
			no_of_lines++; 
			if(no_of_lines == random_numbers.front()){
				iF<<line<<endl;
				random_numbers.pop_front();
			}
			else{
				sF<<line<<endl;
			}
		}
		sF.close(); iF.close();
		df.clear(); df.seekg(0);
	}
}
return 0;
}

























