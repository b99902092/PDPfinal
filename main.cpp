#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "MoveStone.h"
using namespace std;

int main(int argc, char* argv[]) {
    Board board;
    if(argc < 2) {
        fprintf(stderr, "no input path parameter!\n");
        return 0;
    }
    board.readInput(argv[1]);
    board.showBoard();
    board.calcBoardValue();
    printf("max Combo = %d\n", board.maxCombo());


    return 0;
}
