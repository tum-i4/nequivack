#!/bin/bash

# $1: the parameters, passed to ant
runProgramTenTimes() {
	local parameters=$1

	for i in {1..10}; do
		# run programs in parallel
		ant $parameters &
	done

	wait
}

# prepares the parameters, that will be passed to ant
# $1: path to the output folder
# $2: path to the original file
# $3: path to the transformed file
# $4: transformation command
# one of $3 or $4 must be empty!
runProgram() {
	local buildFile="-buildfile run.xml"
	local outputFolder="-DoutputFolder=$1"
	local originalFile="-DoriginalFile=$2"

	if [ -n "$3" ]
		then transformedFile="-DtransformedFile=$3"
		else transformedFile=""
	fi
	if [ -n "$4" ]
		then transformationTool="-DtransformationTool=\"$4\""
		else transformationTool=""
	fi

	# run each program ten times with the built parameters
	runProgramTenTimes "$buildFile $outputFolder $originalFile $transformedFile $transformationTool"
}

# prepares the paths for input files and output directory
# $1: name of folder that contains the category folders
# $2: name of program category the programs belong to
# $3: an array of programs, that are inside this category
# $4: name of output folder
# $5: if set, this is the prefix of a transformed file 
# $6: transformation command
runPrograms() {
	local evaluationFolder="$1"
	local programCategory="$2"
	local programs=("${!3}")
	local resultFolderName="$4"
	local transformationPrefix="$5"
	local transformation="$6"

	# the results should be in the same structure, the inputs are
	local programFolder="$evaluationFolder/$programCategory"
	local resultFolder="$resultFolderName/$programCategory"

	# for each program in a category
	for program in ${programs[@]}; do
		local outputFolder="$resultFolder/$program" # path for the ten output folders
		local originalFile="$programFolder/$program.c" # path to the original file
		local transformedOrOriginalFile="$programFolder/$transformationPrefix$program.c" # if prefix given: path to transformed file, otherwise path to the original file

		# if there is a transformation command, use this instead of the transformed file
		if [ -z "$transformation" ]
			then runProgram "$outputFolder" "$originalFile" "$transformedOrOriginalFile" ""
			else runProgram "$outputFolder" "$originalFile" "" "$transformation"
		fi		
	done
}

##### define directories and program names #####

results="evaluation-results" # basic folder name, where the results go in
inputs="evaluation" # name of folder that contains the category folders

categories=( basic math sorting structs ) # all categories in input folder
basic=( Branching Looping ) # all names of basic programs
math=( Divide Factorial Fibonacci Gcd Multiply Power PrimePalindrome Primes SimpleMath Sum ) # all names of math programs
sorting=( Bubble Insertion Merge Quick Selection ) # all names of sorting programs
structs=( ComplexNumber ) # all names of struct programs

##### define used transformations #####

transformation1="tigress --Transform=Virtualize --VirtualizeSuperOpsRatio=3.0 --Functions=* --out=%OUT.c -o %OUT.o %IN"
transformation2="tigress --Transform=Virtualize --Functions=* --out=%OUT.c -o %OUT.o %IN"
transformation3="tigress --Transform=Virtualize --VirtualizeOperands=registers --Functions=* --out=%OUT.c -o %OUT.o %IN"
transformation4="tigress --Transform=InitOpaque --Functions=* --Transform=Virtualize --Functions=* --Transform=Flatten --Functions=* --FlattenObfuscateNext=true --out=%OUT.c -o %OUT.o %IN"

##### remove old evaluation results #####

rm -rf $results*

##### transformed file defective #####

for category in ${categories[@]}; do
	runPrograms "$inputs" "$category" "$category[@]" "$results-defective" "Defective_" ""
done

##### original & transformed file identical #####

for category in ${categories[@]}; do
	runPrograms "$inputs" "$category" "$category[@]" "$results-identical" "" ""
done

##### transformations #####

for category in ${categories[@]}; do
	runPrograms "$inputs" "$category" "$category[@]" "$results-transformation1" "" "$transformation1"
done

for category in ${categories[@]}; do
	runPrograms "$inputs" "$category" "$category[@]" "$results-transformation2" "" "$transformation2"
done

for category in ${categories[@]}; do
	runPrograms "$inputs" "$category" "$category[@]" "$results-transformation3" "" "$transformation3"
done

for category in ${categories[@]}; do
	runPrograms "$inputs" "$category" "$category[@]" "$results-transformation4" "" "$transformation4"
done

##### archive results #####

time=$( date +%Y%m%d_%H%M%S )
tar vcfz "$time"_$results.tgz $results*
