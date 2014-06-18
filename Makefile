FLAG = -O2 -Wall
PROG = main.o MoveGem.o
HEADER = MoveGem.h
TARGET = main
.PHONY: clean

$(TARGET): $(PROG) 
	$(CXX) $(PROG) $(FLAG) -o $(TARGET)

%.o: %.cpp %.h
	$(CXX) $< $(FLAG) -c
clean:
	rm -rf *.o $(TARGET)
run: $(TARGET)
	./$(TARGET) input1.txt
