#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "MoveStone.h"
using namespace std;

int main(int argc, char* argv[]) {
    Board board;
    if(argc < 1) {
        fprintf(stderr, "no input path parameter!\n");
        return 0;
    }
    readInput(argv[1], &board);
    showBoard(&board);
    return 0;
}
