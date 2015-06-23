# nequivack

nequivack is a non-equivalence checker for C programs. With the use of the symbolic exeution engine [KLEE](http://klee.github.io), this tool analyzes C programs symbolically and tries to find counterexamples for their equivalence.


## Files & Directories

* **build.xml** Builds the parts of the tool.
* **run.xml** Runs the parts of the tool.
* **eqCheck.properties** Contains properties for both files above.
* **Evaluation.sh** Script, that we used for evaluation. Shows example multi-threaded usage of the tool.

* **build** Generated in build process. Contains the running binaries for all parts of the tool.
* **evaluation** Contains small example programs, we used for evaluation.
* **src** Contains the source code of the tool.


## Requirements

The Non-Equivalence Checker has some software requirements. They are listed below.

* Apache Ant (at least version 1.8.0) and thereby a Java Runtime (at least Java 7)
* Clang and libclang with the following packages: `clang-3.4 libclang-3.4-dev libclang-common-3.4-dev libclang-dev libclang1-3.4:amd64`
* KLEE with LLVM 3.4 as described here: [http://klee.github.io/experimental/](http://klee.github.io/experimental/)
* If you plan to rerun our evaluation script, you will also need [Tigress](http://tigress.cs.arizona.edu/download.html)


## Building

First, open a Terminal and change to the directory `nequivack`.
Simply run `ant`. This is sufficient to build the tool.

## Running

First, open a Terminal and change to the directory `nequivack`.
For running, you can decide between **(A)** passing an already transformed file or **(B)** a command to perform a transformation with a tool. You always need to provide an original file. Independent from the option you choose, you have to set the buildfile to `run.xml`. Additionally, you can define an output folder that will gather all the output from the tool.
You have to annotate the input files before passing them to the tool. You must at least place a comment `// @klee` in the line above the function definition of the function, that should be checked for equivalence by the tool. Otherwise, the tool will not recognize the function.

**(A)** Using an already transformed file
ant -buildfile run.xml -DoriginalFile=path/to/original/file.c -DtransformedFile=path/to/transformed/file.c [-DoutputFolder=folder]
                                                                                                            
**(B)** Using command to perform a transformation with a tool
ant -buildfile run.xml -DoriginalFile=path/to/original/file.c -DtransformationTool="command" [-DoutputFolder=folder]

Within the command, you can use three different placeholders:
* `%PRE` transformation prefix (must be in front of method names)
* `%IN` file that has to be transformed
* `%OUT` output files of transformation (endings have to be specified)


## KLEE parameters

If you want to change the parameters, the tool calls KLEE with, you can easily do this, by changing the `klee.parameters` property in the `eqCheck.properties` file.
