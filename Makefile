FLAG = -O2 -Wall
PROG = main.o MoveStone.o
.PHONY: clean

main: $(PROG)
	$(CXX) $(PROG) -o main
%.o: %.cpp
	$(CXX) $< $(FLAG) -c
clean:
	rm -rf *.o main
run:
	./main exampleInput.txt
