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
    if( board.readInput(argv[1]) == -1) return 0;
    //swap(board.board[0][2], board.board[0][3]);
    
    fprintf(stderr, "show initial board:\n");
    board.showBoard();
    fprintf(stderr, "max possible Combo = %d, calcBoardCombo = %d\n", board.maxCombo(), board.calcBoardCombo());
    //Path path = board.solve();
    //path.printReadablePath();

    return 0;
}
