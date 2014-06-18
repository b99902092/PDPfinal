#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <time.h>
#include "MoveGem.h"
#define OMP   //enable this if you want to use OpenMP
using namespace std;

#ifdef OMP
#include <omp.h>
#endif

void errorMsg(const char *msg) { fprintf(stderr, "Error!, %s\n", msg); }

void Path::printReadablePath() {
    if(dirLen == -1) {
        fprintf(stderr, "no path found\n");
        return;
    }
    fprintf(stderr, "start point = (%d, %d)\n", startX, startY);
    fprintf(stderr, "path length = %d\n", dirLen);
    for(int i=0; i<dirLen; i++) {
        if(dir[i] == 8) fprintf(stderr, "up\n");
        if(dir[i] == 2) fprintf(stderr, "down\n");
        if(dir[i] == 4) fprintf(stderr, "left\n");
        if(dir[i] == 6) fprintf(stderr, "right\n");
    }
}

void Path::print() {
    if(dirLen==-1) {
        printf("-1 -1\n");
        return;
    }
    printf("%d %d ", startX, startY);
    for(int i=0; i<dirLen; i++)
        printf("%d", dir[i]);
    fflush(stdout);
}

int Board::readInput(const char *path) {
    FILE *fp = fopen(path, "r");
    if(fp == NULL) {
        errorMsg("no such input file");
        return -1;
    }
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

void Board::printBoard() const {
    fprintf(stderr, "====show board====\n");
    for(int i=0; i<R; i++) {
        for(int j=0; j<C; j++)
            fprintf(stderr, "%d", board[i][j]);
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "==================\n");
}

void Board::eliminateElement(int x, int y, int clr, int mark[R][C]) {
    mark[x][y] = 0;
    board[x][y] = 0;
    for(int i=0; i<SIZE(dx); i++) {
        int nx = x+dx[i], ny = y+dy[i];
        if(nx<0 || nx>=R || ny<0 || ny>=C) continue;
        if(mark[nx][ny] == clr)
            eliminateElement(nx, ny, clr, mark);
    }
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

Board Board::calcComboAndFallenBoard(int *rtv) const {
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

                for(l=0; i+l<R && b.board[i][j]==b.board[i+l][j]; l++);
                if(l>=3) for(int t=0; t<l; t++) mark[i+t][j] = b.board[i][j];

                for(l=0; j+l<C && b.board[i][j]==b.board[i][j+l]; l++);
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
                    while(t>=0 && b.board[t][j]) t--;
                    if(i>=t) continue;
                    b.board[t][j] = b.board[i][j];
                    b.board[i][j] = 0;
                }
            }
//        b.printBoard();
    }
    *rtv = cmb;
    return b;
}

Path Board::solve() const {
    srand(time(NULL));
    Board boards[R*C];
    for(int pos=0; pos<R*C; pos++)
        memcpy(&boards[pos], this, sizeof(Board));
    Path answer;
    
    int maxCmb = maxCombo();
    int flag = 0;

    for(int targetCmb=maxCmb; targetCmb>=1; targetCmb--) { // at least 1 combo
        for(int bound=1; bound<=MAXSTEP && answer.dirLen==-1; bound++) { // iterative deepening
            fprintf(stderr, "boundary = %d, target combo = %d\n", bound, targetCmb);
            Path pathArray[R*C];
            Stack stacks[R*C];
#ifdef OMP
            #pragma omp parallel for num_threads(30)
#endif
            for(int pos=0; pos<R*C; pos++) {
                int x=pos/C, y=pos%C;

                pathArray[pos] = boards[pos].ida_star(x, y, Direction(null), 0, bound, targetCmb, stacks[pos], flag);
            }

            for(int pos=0; pos<R*C && answer.dirLen==-1; pos++)
                if(pathArray[pos].dirLen!=-1) {
                    answer = pathArray[pos];
                    answer.startX = pos/C;
                    answer.startY = pos%C;
                }
        }
    }

    if(answer.dirLen==0) { // for special case, at least move 1 step.
        for(int i=0; i<R && answer.dirLen==0; i++)
            for(int j=0; j<C && answer.dirLen==0; j++)
                if(j+1<C && board[i][j] == board[i][j+1]) {
                    answer.dirLen = 1;
                    answer.startX = i; answer.startY = j;
                    answer.dir[0] = right;
                }
                else if(i+1<R && board[i][j] == board[i+1][j]) {
                    answer.dirLen = 1;
                    answer.startX = i; answer.startY = j;
                    answer.dir[0] = down;
                }
    }

    return answer;
}

int Board::calcDist(int a, int b, int c) const{
    // A--d1--B--d2---C
    int d = b-a-1 > 0 ? b-a-1 : 0;
    d += c-b-1 > 0 ? c-b-1 : 0;	// calc d2, c-b
    return d;
}

int Board::heuristic(int cmbNeeded) const{
    //return 0;
    int m = INF;

<<<<<<< HEAD
    for(int color=1; color<=6 && m!=0; color++) {
=======
    for(int color=1; color<=6 && m; color++) {
>>>>>>> f4add4a2463e951b17a1dc88abf9ff57b48b3595
        int vis[6]={}, arr[6]={}, ind=0;
        for(int i=0; i<R; i++)
            for(int j=0; j<C; j++)
                if(board[i][j] == color) vis[j] = 1;

        for(int i=0; i<6; i++)
            if(vis[i]) arr[ind++] = i;

        //if(ind < 3) continue;
        for(int i=0; i<ind-2; i++) {
            int d = calcDist(arr[i], arr[i+1], arr[i+2]);
            if(m > d) m = d;
        }
    }

    if(m == INF) return 0; // unknown situation
    return m + (cmbNeeded - 1); //magic, don't touch, actually it should overestimate....

    // actually, we still need at least one more move to eliminate more gems.
    // but it can be considered in the condition: (path size == 0).
    // which exchange the position of two adjacent gems.
}

Path Board::ida_star(int x, int y, Direction prevStep, int cost, int bound, int target, Stack &stack, int &flag) {
    int currentCombo;
    Path path;
    if(flag) return path; // global flag, break the recursion when solution found

    Board fallen = calcComboAndFallenBoard(&currentCombo);

    int h = fallen.heuristic(target - currentCombo);
    int f = cost + h;

    if(f > bound) return path; //fail to continue
    /*
       path.printReadablePath();
       printBoard();
       fprintf(stderr, "cost = %d, bound = %d, target = %d, board combo = %d, heuristic = %d\n", cost, bound, target, currentCombo,h); 
       */
    if(currentCombo >= target) { //find solution, return a solution path
        fprintf(stderr, "find solution!!, %d\n", currentCombo);
        printBoard();
        strncpy(path.dir, stack.s, sizeof(char)*stack.size());
        path.dir[stack.size()] = 0;
        path.dirLen = strlen(path.dir);
        flag = 1;
        return path;
    }


    //    int rnd = rand()%SIZE(order);
    for(int i=0; i<SIZE(dirList); i++) {
        //        int i = order[rnd][t];
        int nx = x+dx[i], ny = y+dy[i];

        if(dirList[i] == (10 - prevStep)) continue; //no going back
        if(nx<0 || nx>=R || ny<0 || ny>=C) continue;

        stack.push(dirList[i]);
        swap(board[x][y], board[nx][ny]);

        Path p = ida_star(nx, ny, dirList[i], cost+1, bound, target, stack, flag);

        if(p.dirLen != -1) {
            if(path.dirLen == -1 || p.dirLen < path.dirLen) {
                path = p;
                break; // can be deleted if we want to find multiple sols someday.
            }
        }

        swap(board[x][y], board[nx][ny]);
        stack.pop();
    }

    return path;
}
