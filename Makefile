CC = g++
FLAG = -O2 -Wall
PROG = main.o MoveStone.o
.PHONY: clean

main: $(PROG)
	$(CC) $(PROG) -o ComboSolver
%.o: %.cpp
	$(CC) $< $(FLAG) -c
%.o: %.cc
	$(CC) $< $(FLAG) -c
clean:
	rm -rf *.o ComboSolver
