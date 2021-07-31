# Incremental-Isolation-Forest

Isquare Forest directory consists of all the c++ code required for reproducing the results along with one of the dataset that We have used for our experiments. You can run the code on any data of numeric type, how to prepare dataset for experiments is given in below description. File name "D31" is the original dataset file. Maintain your original dataset file in the the same format.

Note: Replace D31 everywhere in the repository by any dataset name you are using.

# Installation Details
Installing Boost c++ library for serialization.
```
sudo apt-get install libboost-all-dev
```

# Compilation details
```
g++ filename.cpp -lboost_serailiztion
```
# Repository description
Isquare Forest consists of the following code files and sub directories containing preapred dataset for demonstration. 

1. D31ds: this sub-directory consists of 10 different versions for each increment size and there are 19 different increment sizes we have in the range [0.1%, 0.9%] and [1%, 10%]. All the versions of each increment size  has two files, for example  "D31Inc_0.000001_ver0" and "D31Static_0.000001_ver0". This subdirectory is used while we handle insertion operations.

2. D31deleteds: this sub-directory consists of 10 different versions for each increment size and there are 19 different increment sizes we have in the range [0.1%, 0.9%] and [1%, 10%]. All the versions of each increment size  has two files, for example  "D31Inc_0.000001_ver0" and "D31Static_0.000001_ver0". This subdirectory is used while we handle deletion operations.


Following Python file for analysis and getting consolidated output : Running following four python scripts once are sufficient for getting all the results corresponding to any dataset. These python scripts writes decriptive results for every individual run of the experiment in the text files inside the subdirectory named as "ResultFiles" and writes down the averaged final output into the console.
 
3. analysisExcludingErrorForAddition.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output without computing error in pairwise mass based similarity score.
Command to execute along with the required command line inputs.
```
python3 analysisExcludingErrorForAddition.py baseD31 D31Static D31Inc 50 0.01 50 256 10 D31ds
```

4. analysisExcludingErrorForDeletion.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output without computing error in pairwise mass based similarity score.
Command to execute along with the required command line inputs.
```
python3 analysisExcludingErrorForDeletion.py D31 D31Static D31Inc 50 0.01 256 10 D31deleteds
```

5. analysisIncludingErrorForAddition.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output including pairwise mass based similarity score error computation.
Command to execute along with the required command line inputs.
```
python3 analysisIncludingErrorForAddition.py baseD31 D31Static D31Inc 50 0.01 256 3 D31ds
```

6. analysisIncludingErrorForDeletion.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output including pairwise mass based similarity score error computation.
Command to execute along with the required command line inputs.
```
python3 analysisIncludingErrorForDeletion.py D31 D31Static D31Inc 50 0.01 256 3 D31deleteds
```

7. compareMassFiles.cpp: code to compare the computed the mass based similarity score value of each pair across multiple static runs to compute static error and for incrmental error this code compares the similarity score values computed using static and incremental algorithm as well.

8. dataset.cpp and dataset.h : consists of the code to process the provided dataset file and create a dataset objects to be used in the implementation.

9. iforest.cpp and iforest.h : consists of the functionality related to iforest creation and other operations to performed over the iforest object. Contains methods to write/read iForest and itree related information into/from the secondary storage. All the infromation is written in the binary files inside the sub-directory named as "IntermediateFiles"

10. itree.cpp and itree.h : consists of the functionality related to itree creation and other operations to performed over the itree object. 

11. treenode.cpp and treenode.h : consists of the functionality related to treenode creation and operations over the treenode object.

12. split_for_addition : this code splits the original dataset file in the desired format (shown in D31ds sub-directory) for addition operation. Takes two inputs, one original data file and another number of versions required per version. Provides output files similar to the files in sub directory D31ds.

13. split_for_deletion : this code splits the original dataset file in the desired format (shown in D31deleteds sub-directory) for deletion operation. Takes two inputs, one original data file and another number of versions required per version. Provides output files similar to the files in sub directory D31deleteds.


# Execution Sequence
Prepare the required versions and subsets of original dataset using the "split_for_addition.cpp" and "split_for_deletion.cpp" to get data for addition and deletion operation respectively and stire the results in sub-directories D31ds and D31deleteds. Note: replace D31 with your own datset name. 

Execute the python scripts mentioned in above text. 
```
python3 [Above mentioned script name.py] [basesetting data file name] [dataset name]Static [dataset name]Inc [number of iTrees] [smapling factor] [minimum sample size] [number of runs/versions] [sub-directory name containing the prepared data]
```
Follow the points 3,4,5,6 in the above text for examples of particular execution commands. Note replace the D31 with your own datset name.
