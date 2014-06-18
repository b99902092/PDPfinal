#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "MoveGem.h"
using namespace std;

int main(int argc, char* argv[]) {
    Board board;
    if(argc < 2) {
        fprintf(stderr, "no input path parameter!\n");
        return 0;
    }
    if( board.readInput(argv[1]) == -1 ) return 0;
    //swap(board.board[0][2], board.board[0][3]);
    
    fprintf(stderr, "show initial board:\n");
    board.printBoard();
    int cmb;
    board.calcComboAndFallenBoard(&cmb);
    fprintf(stderr, "max possible Combo = %d, calcBoardCombo = %d\n", board.maxCombo(), cmb);
    Path path = board.solve();
    path.printReadablePath();
    path.print();
    //board.showTurnDetail(path);

    return 0;
}
