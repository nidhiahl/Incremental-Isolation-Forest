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
 double splitPercentage = 10; 
 string outfilepart1 = "non-base"+infile;
 string outfilepart2 = "base"+infile;
 ifstream df(infile.c_str());
 ofstream of1(outfilepart1.c_str());
 ofstream of2(outfilepart2.c_str());

 if(!df)
 {
   cout<<"Cannot open the input file: "<<infile<<endl;
   exit(0);
 }
 
 int no_of_Inst=0;
 int no_of_attributes=0;
 int part1size=0;
 int no_of_lines = 0;
 list<int> random_numbers;
 int blank;
 
 df>>no_of_Inst>>no_of_attributes>>blank;
 //cout<<no_of_Inst<<"ddd";
 part1size = no_of_Inst * splitPercentage/100;
 of1<<part1size<<" "<<no_of_attributes<<endl;
 of2<<no_of_Inst - part1size<<" "<<no_of_attributes<<endl;
 
 while(random_numbers.size()<part1size){
  int rn=rand()%no_of_Inst+1;
  random_numbers.remove(rn);
  random_numbers.push_back(rn);
 }
 //showlist(random_numbers);
 random_numbers.sort();
 //showlist(random_numbers);
 while(getline(df,line)){
  no_of_lines++;
  if(no_of_lines==random_numbers.front()){
   of1<<line<<endl;
   //of1<<endl;
   random_numbers.pop_front();
  }
  else{
   
   of2<<line<<endl;
   //of1<<endl;
  }
 }
 df.close(); of1.close(); of2.close();
 
 
 
ifstream in10(outfilepart1);
 if(!in10)
 {
   cout<<"Cannot open the input file: "<<infile<<endl;
   exit(0);
 }
 int numInst10,numAttr10;
 in10>>numInst10>>numAttr10;
 
ifstream in90(outfilepart2);
 if(!in90)
 {
   cout<<"Cannot open the input file: "<<infile<<endl;
   exit(0);
 }
 int numInst90,numAttr90;
 in90>>numInst90>>numAttr90;
 
int numVersion = 5;
cout <<fixed;
cout <<setprecision(1);
vector<float> increments = {0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0};

for(int increment = 0; increment<increments.size(); increment++){
	for(int version = 0; version < numVersion; version++){
		string iFile = infile+"Inc_"+to_string(increments[increment])+"_ver"+to_string(version);
		ofstream iF(iFile.c_str());
		int sizeIF = 10 * increments[increment] * numInst10/100;
		iF<< sizeIF<<" "<<numAttr10<<endl;
		string sFile = infile+"Static_"+to_string(increments[increment])+"_ver"+to_string(version);
		ofstream sF(sFile.c_str());
		int sizeSF = numInst90+sizeIF;
		sF<<sizeSF <<" "<<numAttr90<<endl;
		getline(in90,line);
		while(getline(in90,line)){
			sF<<line<<endl;
		}
		
		random_numbers.clear();
		random_numbers.resize(0);
		cout<<random_numbers.size()<<" "<<sizeIF<<endl;
		while(random_numbers.size()<sizeIF){
			int rn=rand()%part1size+1;
			random_numbers.remove(rn);
			random_numbers.push_back(rn);
		}
		random_numbers.sort();
		no_of_lines=0;
		while(getline(in10,line)){
			no_of_lines++; 
			if(no_of_lines == random_numbers.front()){
				iF<<line<<endl;
				sF<<line<<endl;
				random_numbers.pop_front();
			}
		}
		sF.close(); iF.close();
		in10.clear(); in10.seekg(0);
		in90.clear(); in90.seekg(0);
	}
}

return 0;
}
























