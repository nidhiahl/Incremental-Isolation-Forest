#python3 analysisExcludingErrorForDeletion.py D31 D31Static D31Inc 50 0.01 256 5 D31deleteds


import sys, os

dataset = str(sys.argv[1])   
datasetStatic = str(sys.argv[2])
datasetIncremental = str(sys.argv[3])
numOfTrees = str(sys.argv[4])
samplingFactor = str(sys.argv[5])
minSampleSize = str(sys.argv[6])
incPercentage = [0.100000,0.200000,0.300000,0.400000,0.500000,0.600000,0.700000,0.800000,0.900000,1.000000,2.000000,3.000000,4.000000,5.000000,6.000000,7.000000,8.000000,9.000000,10.000000]     
#incPercentage = [8.000000,9.000000,10.000000]     

numOfVersions = int(sys.argv[7])
datasetFolder = str(sys.argv[8])
#static run for Basic Environment Setting
os.system("g++ -o staticc static/main.cpp static/treenode.cpp static/itree.cpp static/iforest.cpp static/dataset.cpp -lboost_serialization")
os.system("g++ -o incremental main.cpp treenode.cpp itree.cpp iforest.cpp dataset.cpp -lboost_serialization")
os.system("g++ -o compare compareMassFiles.cpp -lboost_serialization")

os.system("rm -f ResultFiles/"+ dataset + "_BasicEnvironmentResults")
#os.system("g++ main.cpp treenode.cpp itree.cpp iforest.cpp dataset.cpp -lboost_serialization")
os.system("./incremental static "+ numOfTrees +" "+ samplingFactor +" "+ minSampleSize +" "+datasetFolder+"/"+ dataset +" >> ResultFiles/" + dataset + "_BasicEnvironmentResults")
os.system("mv "+ "IntermediateFiles/incStaticTreeNodes "+"IntermediateFiles/staticTreeNodes")
os.system("mv "+ "IntermediateFiles/incStaticiTrees "+"IntermediateFiles/staticiTrees")
print ("*******Environment setting is done for dataset", dataset, "**********")


for incrementPercent in incPercentage:
	incrementPercentage = format(incrementPercent, '.6f')
#Static run for complete dataset after updates
	print("\n\n***********Increment Percentage =", incrementPercentage,"***********")
	os.system("rm -f ResultFiles/"+ datasetStatic + "_result_"+str(incrementPercentage))
	#os.system("g++ main.cpp treenode.cpp itree.cpp iforest.cpp dataset.cpp -lboost_serialization")

	for i in range(numOfVersions):
		os.system("./staticc "+ numOfTrees +" "+ samplingFactor +" "+ minSampleSize +" "+datasetFolder+"/"+ datasetStatic +"_"+ str(incrementPercentage) +"_ver"+ str(i) +" >> ResultFiles/" + datasetStatic + "_result_"+str(incrementPercentage))
		#os.system("mv "+"massMatrix/massMatrixFile"+ " massMatrix/"+ datasetStatic +"_MassFile_"+ str(incrementPercentage)+"_ver"+ str(i))
		print ("---Version",i,"static run","done!	")


#compute avg time and avg space consumption for static run
	print("--------------Static Results-----------------")
	lines = open("ResultFiles/"+ datasetStatic + "_result_"+str(incrementPercentage)).readlines()
	timeSum = 0
	spaceSum = 0
	vSpaceSum = 0
	for i in range(numOfVersions):
		values = lines[i].split()
		timeSum += float(values[4])
		spaceSum += float(values[5])
		vSpaceSum += float(values[6])
	
	print ("Time = ",timeSum/numOfVersions,"	Space =", spaceSum/numOfVersions,"	VirtualSpace =",vSpaceSum/numOfVersions)


#incremetnal run for updated dataset
	print("---------------Incremental Run-------------")
	
	os.system("rm -f ResultFiles/"+ datasetIncremental + "_result_"+str(incrementPercentage))
	for i in range(numOfVersions):
		os.system("./incremental incremental deletion "+ numOfTrees +" "+ samplingFactor +" "+ minSampleSize +" "+datasetFolder+"/"+ dataset+" "+datasetFolder+"/"+ datasetIncremental +"_"+ str(incrementPercentage) +"_ver"+ str(i) +" >> ResultFiles/" + datasetIncremental + "_result_"+str(incrementPercentage))
		#os.system("mv "+"massMatrix/massMatrixFile"+ " massMatrix/"+ datasetIncremental +"_MassFile_"+ str(incrementPercentage)+"_ver"+ str(i))
		print ("---Version",i,"incremental run","done!")
		
		
		
#compute avg time and avg space consumption and avg standard deviation in pairwise mass value for incremetnal run
	print("-------------Incremental Results-----------------")
	
	lines = open("ResultFiles/"+ datasetIncremental + "_result_"+ str(incrementPercentage)).readlines()
	timeSum = 0
	spaceSum = 0
	vSpaceSum = 0
	totalNodes = 0
	unchangedNodes = 0
	for i in range(numOfVersions):
		values = lines[i].split()
		timeSum += float(values[6])
		spaceSum += float(values[7])
		vSpaceSum += float(values[8])
		totalNodes += int(values[9])
		unchangedNodes += int(values[10])

	print ("Time =",timeSum/numOfVersions,"	Space =", spaceSum/numOfVersions,"	VirtualSpace=", vSpaceSum/numOfVersions,"	TotalNodes=",int(totalNodes/numOfVersions),"	UnchangedNodes=",int(unchangedNodes/numOfVersions),"	%of Unchanged Nodes=",100*(int(unchangedNodes/numOfVersions))/(int(totalNodes/numOfVersions)))

	
	#print ("Time =",timeSum/numOfVersions,"	Space =", spaceSum/numOfVersions,"	VirtualSpace=", vSpaceSum/numOfVersions,"	TotalNodes=",int(totalNodes/numOfVersions),"	UnchangedNodes=",int(unchangedNodes/numOfVersions),"	%of Unchanged Nodes=",100*(int(unchangedNodes/numOfVersions))/(int(totalNodes/numOfVersions)),"	  Static Pairwise average %age error =", errorSum/numOfVersions)


	
















































