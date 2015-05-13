# Travelling Salesman Problem Solvers

Implemented two solvers for a classic NP-hard CS problem with combinatorial optimizations.

(1) A_Star_Search.cc was use A* search algorithm with a heuristic function.

(2) Local_Search.cc was use local search algorithm with annealing schedules.


for the input file like testfile0.txt it looks like:
---------------
4
A 56 87
B 30 56
C 80 41
D 15 24

which represent 4 citys and their coordinates, every city connect to each other.

The solvers' purpose is to find the optimal(shortest) path to walk through all cities from the origin(city A), return back to the origin at last.

The output shows the length of the optimal path and the order of cities through the path.

for example: 
-----------------------------------------
THE OPTIMAL PATH LENGTH: 194.872
Therefore, the path is: A C D B A


PS:
---
testfile0.txt use 4 cities
testfile1.txt use 8 cities
