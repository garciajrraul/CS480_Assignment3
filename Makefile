#Name: 	Raul Garcia Jr
#		Cat-Tuong Tu
#RedID: 819138431
#		822204923
#Class: CS480-01
#Assignemt 3

# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#       -std=c++11  C/C++ variant to use, e.g. C++ 2011
#       -g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c11 -g

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
pagingwithtlb : main.o tracereader.o output_mode_helper.o
        $(CXX) $(CXXFLAGS) -o pagingwithtlb $^

main.o: main.c

tracereader.o: tracereader.h tracereader.c

output_mode_helper.o: output_mode_helper.h output_mode_helper.c

clean:
        rm *.o pagingwithtlb