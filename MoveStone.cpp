#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "MoveStone.h"
using namespace std;

void errorMsg(const char *msg) { fprintf(stderr, "Error!, %s\n", msg); }

int Board::readInput(const char *path) {
    FILE *fp = fopen(path, "r");
    char str[100];
    for(int i=0; i<R; i++) {
        if( fscanf(fp, "%s", str) == -1 ) {
            errorMsg("file less than 5 rows");
            return -1;
        }
        if(strlen(str) != C) {
            errorMsg("1 row must contain exactly 6 numbers");
            return -1;
        }
        for(int j=0; j<C; j++) {
            if(str[j]<'1' || str[j] >'6') {
                errorMsg("input contain invalid number");
                return -1;
            }
            board[i][j] = str[j] - '0';
        }
    }
    return 0;
}

void Board::showBoard() const {
    fprintf(stderr, "====show board====\n");
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++)
            fprintf(stderr, "%d", board[i][j]);
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "==================\n");
}

void Board::eliminateElement(int x, int y, int clr, int mark[R][C]) {
    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    mark[x][y] = 0;
    board[x][y] = 0;
    for(int i=0; i<4; i++) {
        if(x+dx[i]<0 || x+dx[i]>R || y+dy[i]<0 || y+dy[i]>C) continue;
        if(mark[x+dx[i]][y+dy[i]] == clr)
            eliminateElement(x+dx[i], y+dy[i], clr, mark);
    }
}

int Board::calcBoardValue() const {
    Board b;
    memcpy(&b, this, sizeof(Board));
    int cmb = 0, flag = 0;
    while(1) {
        flag = 0;
        int mark[R][C] = {};
        /* calc connection */
        for(int i=0; i<R; i++) {
            for(int j=0; j<C; j++) {
                int l;

                for(l=0; l<R && b.board[i][j]==b.board[i+l][j]; l++);
                if(l>=3) for(int t=0; t<l; t++) mark[i+t][j] = b.board[i][j];

                for(l=0; l<C && b.board[i][j]==b.board[i][j+l]; l++);
                if(l>=3) for(int t=0; t<l; t++) mark[i][j+t] = b.board[i][j];
            }
        }

        /* eliminate */
        for(int i=0; i<R; i++)
            for(int j=0; j<C; j++)
                if(mark[i][j]) {
                    b.eliminateElement(i, j, mark[i][j], mark);
                    cmb++;
                    flag = 1;
                }

        if(flag == 0) break;

        /* drop */
        for(int j=0; j<C; j++) 
            for(int i=R-1, t=R-1; i>=0; i--) {
                if(b.board[i][j]) {
                    b.board[t--][j] = b.board[i][j];
                    b.board[i][j] = 0;
                }
            }
        //        b.showBoard();

    }
    return cmb;
}

int Board::maxCombo() const {
    int cnt[7] = {}, cmb=0;
    for(int i=0; i<R; i++)
        for(int j=0; j<C; j++)
            cnt[ board[i][j] ]++;

    for(int i=1; i<=6; i++) {
        if(cnt[i]<18) cmb += cnt[i]/3;
        else cmb += (30-cnt[i])/3;
    }
    return cmb;
}

Path Board::solve() const {
    Board b;
    Path path;
    memcpy(&b, this, sizeof(Board));
    //    memset(&path, 0, sizeof(Path));

    //    if(calcBoardValue() >= maxCombo()) return path;

    for(int bound=1; bound<=MAXSTEP; bound++) {
        for(int pos=0; pos<R*C; pos++) {
            int x=pos/C, y=pos%C;
            path = b.ida_star(x, y, Direction(null), 0, bound);
            //return value of idastar? path?
            //parameter? prev_step, cost, bound, currentpath
        }
    }
    return path;
}

Path Board::ida_star(int x, int y, Direction prevStep, int cost, int bound) {

}
