CC = g++
CPPFLAGS = -g -Wall -O2 -DIL_STD
LDFLAGS =

MEMOC_PATH = /home/cmass/universita/laurea_magistrale/memoc/
CPX_INCDIR  = $(MEMOC_PATH)/CPLEX_Studio/cplex/include
CPX_LIBDIR  = $(MEMOC_PATH)/CPLEX_Studio/cplex/lib/x86-64_linux/static_pic
CPX_LDFLAGS = -lcplex -lm -pthread

OBJ = model.o instance.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -I$(CPX_INCDIR) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o esercitazione01 -L$(CPX_LIBDIR) $(CPX_LDFLAGS)

clean:
		rm -rf $(OBJ) esercitazione01

.PHONY: clean
