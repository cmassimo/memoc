CC = g++
CPPFLAGS = -g -Wall -O2 -DIL_STD
LDFLAGS =

CPX_INCDIR  = /Users/cmass/Applications/IBM/ILOG/CPLEX_Studio_Preview126/cplex/include
CPX_INCCONCERTDIR  = /Users/cmass/Applications/IBM/ILOG/CPLEX_Studio_Preview126/concert/include
CPX_LIBDIR  = /Users/cmass/Applications/IBM/ILOG/CPLEX_Studio_Preview126/cplex/lib/x86-64_osx/static_pic
CPX_CONCERTDIR  = /Users/cmass/Applications/IBM/ILOG/CPLEX_Studio_Preview126/concert/lib/x86-64_osx/static_pic
CPX_LDFLAGS = -lconcert -lilocplex -lcplex -m64 -lm -lpthread -framework CoreFoundation -framework IOKit -stdlib=libstdc++

OBJ = model.o instance.o

%.o: %.cpp
		$(CC) $(CPPFLAGS) -I$(CPX_INCDIR) -I$(CPX_INCCONCERTDIR) -c $^ -o $@

main: $(OBJ)
		$(CC) $(CPPFLAGS) $(OBJ) -o esercitazione01 -L$(CPX_LIBDIR) -L $(CPX_CONCERTDIR) $(CPX_LDFLAGS)

clean:
		rm -rf $(OBJ) esercitazione01

.PHONY: clean
