FLAG = -O2 -Wall -fopenmp
PROG = main.o MoveGem.o
HEADER = MoveGem.h
TARGET = main
.PHONY: clean

$(TARGET): $(PROG) 
	$(CXX) $(PROG) $(FLAG) -o $(TARGET)

%.o: %.cpp %.h
	$(CXX) $< $(FLAG) -c
server:
	gcc server.c -o server
clean:
	rm -rf *.o $(TARGET) server
run1: $(TARGET)
	time ./$(TARGET) input1.txt
run2: $(TARGET)
	time ./$(TARGET) input2.txt
run3: $(TARGET)
	time ./$(TARGET) input3.txt
run4: $(TARGET)
	time ./$(TARGET) input4.txt
run5: $(TARGET)
	time ./$(TARGET) input5.txt
