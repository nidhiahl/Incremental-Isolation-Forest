# Incremental-Isolation-Forest
Isolation forest for dynamic datasets
Isquare Forest directory consists of all the c++ code requred for reproducing the results along with one of the dataset that We have used for our experiments. You can run the code on any data of numeric type, how to prepare dataset for experiments is given in below description.

# Installation Details
Installing Boost c++ library for serialization.
```
sudo apt-get install libboost-all-dev
```

# Compilation details
```
g++ filename.cpp -lboos_serailiztion
```
# Repository description
Isquare Forest consists of the following code files and sub directories containing preapred dataset for demonstration. 

1. D31ds: this sub-directory consists of 10 different versions for each increment size and there are 19 different increment sizes we have in the range [0.1%, 0.9%] and [1%, 10%]. All the versions of each increment size  has two files, for example  "D31Inc_0.000001_ver0" and "D31Static_0.000001_ver0". This subdirectory is used while we handle insertion operations.

2. D31deleteds: this sub-directory consists of 10 different versions for each increment size and there are 19 different increment sizes we have in the range [0.1%, 0.9%] and [1%, 10%]. All the versions of each increment size  has two files, for example  "D31Inc_0.000001_ver0" and "D31Static_0.000001_ver0". This subdirectory is used while we handle deletion operations.


Following Python file for analysis and getting consolidated output : Running following four python scripts once are sufficient for getting all the results corresponding to any dataset.
 
3. analysisExcludingErrorForAddition.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output without computing error in pairwise mass based similarity score.

4. analysisExcludingErrorForDeletion.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output without computing error in pairwise mass based similarity score.

5. analysisIncludingErrorForAddition.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output including pairwise mass based similarity score error computation.

6. analysisIncludingErrorFordeletion.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output including pairwise mass based similarity score error computation.

7. compareMassFiles.cpp: code to compare the computed the mass based similarity score value of each pair across multiple static runs to compute static error and for incrmental error this code compares the similarity score values computed using static and incremental algorithm as well.

8. dataset.cpp and dataset.h : consists of the code to process the provided dataset file and create a dataset objects to be used in the implementation.

9. iforest.cpp and iforest.h : consists of the functionality related to iforest creation and other operations to performed over the iforest object.

10. itree.cpp and itree.h : consists of the functionality related to itree creation and other operations to performed over the itree object. 

11. treenode.cpp and treenode.h : consists of the functionality related to treenode creation and operations over the treenode object.

12. split_for_addition : this code splits the original dataset file in the desired format (shown in D31ds sub-directory) for addition operation.

13. split_for_deletion : this code splits the original dataset file in the desired format (shown in D31deleteds sub-directory) for deletion operation.
