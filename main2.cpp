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
    if( board.readInput(argv[1]) == -1 ) return 0;
    //swap(board.board[0][2], board.board[0][3]);
    
    fprintf(stderr, "show initial board:\n");
    board.showBoard();
    fprintf(stderr, "max possible Combo = %d, calcBoardCombo = %d\n", board.maxCombo(), board.calcBoardCombo());
    Path path ; 
    path.dirLen = 0;
    char input_dir;
    int x,y;
    fprintf(stderr,"Select start point: ");
    scanf("%d%d",&x,&y);
    path.startX = x;
    path.startY = y;
    fprintf(stderr,"Select direction: ");
    while(scanf("%c\n",&input_dir)!=EOF){
    	if(input_dir == 'u'){
		path = board.turnOne(x,y,8,path);
    	}else if(input_dir == 'd'){
		path = board.turnOne(x,y,2,path);
    	}else if(input_dir == 'l'){
		path = board.turnOne(x,y,4,path);
    	}else if(input_dir == 'r'){
		path = board.turnOne(x,y,6,path);
	}
	path.printReadablePath();
	fprintf(stderr,"Select direction: ");
    }

    return 0;
}
