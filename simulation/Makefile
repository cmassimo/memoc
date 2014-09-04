CC = g++
CPPFLAGS = -g -Wall -O2 -std=c++0x -U__STRICT_ANSI__
LDFLAGS =

MEMOC_PATH = /home/cmass/universita/laurea_magistrale/memoc
CPX_INCDIR  = $(MEMOC_PATH)/CPLEX_Studio/cplex/include
CPX_LIBDIR  = $(MEMOC_PATH)/CPLEX_Studio/cplex/lib/x86-64_linux/static_pic
CPX_LDFLAGS = -lcplex -lm -pthread

OBJ = Solution.o Solver.o main.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -I$(CPX_INCDIR) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o main  -L$(CPX_LIBDIR) $(CPX_LDFLAGS)
		
clean:
		rm -rf $(OBJ) main

.PHONY: clean
