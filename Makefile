FLAG = -O2 -Wall
PROG = main.o MoveStone.o
TARGET = main
MAIN2= main
PROG2 = $(MAIN2).o MoveStone.o
.PHONY: clean

$(TARGET): $(PROG)
	$(CXX) $(PROG) -o $(TARGET)
$(MAIN2): $(PROG2)
	$(CXX) $(PROG2) -o $(MAIN2)

%.o: %.cpp %.h
	$(CXX) $< $(FLAG) -c
clean:
	rm -rf *.o $(TARGET)
run: $(TARGET)
	./$(TARGET) input1.txt
run4: $(MAIN2)
	./$(MAIN2) input4.txt
