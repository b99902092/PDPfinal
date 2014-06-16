FLAG = -O2 -Wall
PROG = main.o MoveGem.o
TARGET = main
.PHONY: clean

$(TARGET): $(PROG)
	$(CXX) $(PROG) -o $(TARGET)

%.o: %.cpp
	$(CXX) $< $(FLAG) -c
clean:
	rm -rf *.o $(TARGET)
run: $(TARGET)
	./$(TARGET) input1.txt
