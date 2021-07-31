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


#include <sys/types.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#include <string.h>

using namespace std;
void showlist(list <int> g) 
{ 
    list <int> :: iterator it; 
    for(it = g.begin(); it != g.end(); ++it) 
        cout << '\t' << *it; 
    cout << '\n'; 
} 
int main(int argc, char* argv[])
{
 srand(time(0));
 string line;
 string word;
 string infile = argv[1];                    //Complete data file.
 double splitPercentage = atof(argv[2]);     // Increment Percentage required.
 int versions = atoi(argv[3]);               // Number of different random versions required for the specified increment percentage.
 
 for(int i = 1; i <= versions; i++){
 ifstream df(infile.c_str());
 string outfilepart1 = to_string(splitPercentage)+"Inc"+infile+to_string(i)/*+".txt"*/;
 string outfilepart2 = to_string(100 - splitPercentage)+"Static"+infile+to_string(i)/*+".txt"*/;
 // "+to_string(100 - splitPercentage)+"%";
 
 ofstream of1(outfilepart1.c_str());
 ofstream of2(outfilepart2.c_str());
 
 if(!df)
 {
   cout<<"Cannot open the input file: "<<infile<<endl;
   exit(0);
 }
 
 int no_of_Instances=0;
 int no_of_attributes=0;
 int part1size=0;
 int no_of_lines = 0;
 list<int> random_numbers;
 int blank;
 
 df>>no_of_Instances>>no_of_attributes>>blank;
 part1size = no_of_Instances * splitPercentage/100;
 of1<<part1size<<" "<<no_of_attributes<<endl;
 of2<<no_of_Instances - part1size<<" "<<no_of_attributes<<endl;
 
 while(random_numbers.size()<part1size){
  int rn=rand()%no_of_Instances+1;
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
}
 
return 0;
}











