1. The main_alltest-1.cpp file includes "datatype.h" and "parser_dimacs.h" files
2. datatype.h contains some datatypes used for the dpll algorithm
3. parser_dimacs.h file contains the parser code for parsing the cnf file in dimacs format.
4. main_alltest-1.cpp contains all the other data structures and the core algorithm for the dpll implementation along with the heuristic
5. All the files are in the same directory only so need not include any path while compiling
. Compilation is pretty simple for example:
g++ -std=c++11 main_alltest-1.cpp -o main
6. run cmd for the code:
./main filename_with_correct_path
for example if you want to run 01.cnf which is is sat_50_218 directory then the run command would look like as follows:
./main ./sat_50_218/01.cnf
7.The benchmarks are in seperate directories in the zip folder
8. Each benchmark folder has three cnf files of name 01.cnf, 02.cnf,03.cnf
9. The name of the benchmark folders are kept as such that it indicates whther the cnf files in it are satisfiable or not along with the number of variables and clauses in all those cnf files under that directory.
10. For example the folder name sat_50_218 tells that all the 3 cnf files under it has 50 variables and 218 clauses and all of them are satisfiable.
the folder name unsat_50_218  tells that all the 3 cnf files under it has 50 variables and 218 clauses and all of them are not satisfiable.


/************important note*******************/

The result of the sat solver whenever a given input is satisfiable is printed out as follows:
RESULT: SAT
ASSIGNMENT:
1=0 2=0 6=0 3=1….45=1…..so on.
What this means is that as the minimum number of variables in our test benchmark is 50 we are naming each variable with its serial number and assigning it to 0 or 1;
For example from the above output sequence we get 1=0 which means variable 1 is assigned to 0; 45=1 means variable 45 is assigned to 1. The output sequence is not sorted, for example in the above sequence we can see the assignment for the 6th variable (6=0) is done before 3rd variable 
(3=1). But it can be using simple standard Vector sort function provided by C++ STL libraries.

When the CNF formula is not satisfied then it just prints the following.
RESULT: UNSAT
