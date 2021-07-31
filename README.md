# Incremental-Isolation-Forest
Isolation forest for dynamic datasets
Isquare Forest directory consists of all the c++ code requred for reproducing the results along with one of the dataset that We have used for our experiments. You can run the code on any data of numeric type, how to prepare dataset for experiments is given in below description.

# Repository description
Isquare Forest consists of the following code files and sub directories containing preapred dataset for demonstration. 

D31ds: consists of 10 different versions for each increment size and there are 19 different increment sizes we have in the range [0.1%, 0.9%] and [1%, 10%]. All the versions of each increment size  has two files, for example  "D31Inc_0.000001_ver0" and "D31Static_0.000001_ver0". This subdirectory is used while we handle insertion operations.

D31deleteds: consists of 10 different versions for each increment size and there are 19 different increment sizes we have in the range [0.1%, 0.9%] and [1%, 10%]. All the versions of each increment size  has two files, for example  "D31Inc_0.000001_ver0" and "D31Static_0.000001_ver0". This subdirectory is used while we handle deletion operations.

  Python file for analysis and getting consolidated output
  
  analysisExcludingErrorForAddition.py: pytohn script to run the algorithm for all different versions of each increment size and provide the consolidated output.
