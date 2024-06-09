#compiler
CC = g++ -std=c++11

#compiler flags
CCFLAGS = -c -w -O2

#normal compilations
all: solver

#--------------------------------------------------------------------------------

solver: solver.cpp Instance.o Individual.o MA.o intensify.o getDistance.o crossoverCX.o utils.o initializeHeuristic.o evaluate.o intensify_edges.o utils.h Individual.h Problem.h ProblemLL.h Instance.h 
	$(CC) solver.cpp Instance.o Individual.o MA.o intensify.o getDistance.o crossoverCX.o utils.o initializeHeuristic.o evaluate.o intensify_edges.o -o $@

utils.o: utils.cpp utils.h
	$(CC) $(CCFLAGS) utils.cpp -o $@

evaluate.o: evaluate.cpp Individual.h Problem.h ProblemLL.h Instance.h
	$(CC) $(CCFLAGS) evaluate.cpp -o $@

initializeHeuristic.o: initializeHeuristic.cpp Individual.h Problem.h ProblemLL.h Instance.h
	$(CC) $(CCFLAGS) initializeHeuristic.cpp -o $@

crossoverCX.o: crossoverCX.cpp Individual.h utils.h Problem.h ProblemLL.h Instance.h
	$(CC) $(CCFLAGS) crossoverCX.cpp -o $@

getDistance.o: getDistance.cpp Individual.h Problem.h ProblemLL.h Instance.h
	$(CC) $(CCFLAGS) getDistance.cpp -o $@

intensify.o: intensify.cpp Individual.h Problem.h ProblemLL.h Instance.h
	$(CC) $(CCFLAGS) intensify.cpp -o $@

intensify_edges.o: intensify_edges.cpp Individual.h Problem.h ProblemLL.h Instance.h
	$(CC) $(CCFLAGS) intensify_edges.cpp -o $@

Instance.o: Instance.cpp Instance.h
	$(CC) $(CCFLAGS) Instance.cpp -o $@

Individual.o: Individual.cpp Individual.h
	$(CC) $(CCFLAGS) Individual.cpp  -o $@

MA.o: MA.cpp MA.h
	$(CC) $(CCFLAGS) MA.cpp  -o $@




#--------------------------------------------------------------------------------

clean:
	rm -f *.o

cleanall:
	rm -f *.o solver
