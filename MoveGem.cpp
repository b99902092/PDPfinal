#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <time.h>
#include "MoveGem.h"
#define OMP   //enable this if you want to use OpenMP
using namespace std;

// Global
Path pathArray[R][C];	// need mutex 
pthread_mutex_t mutex_ans;
int aInt;

void errorMsg(const char *msg) { fprintf(stderr, "Error!, %s\n", msg); }
// return 
//void* Board::ida_star_thread (void *argPtr) {	// it doesn't work
void* ida_star_thread (void *argPtr) {	 
	ThreadArg *args = (ThreadArg*) argPtr;
	// Test if threads are created succesfully
	fprintf(stderr,"Thread %d called with arguments: (x,y)=(%d,%d) ,bound=%d, targetCmb=%d\n===\n",pthread_self(),args->x,args->y,args->bound,args->target);
	//pathArray[x][y] = b.ida_star(x, y, Direction(null), 0, bound, targetCmb, stack);
	Path p = args->board->ida_star(args->x, args->y, Direction(null), 0, args->bound, args->target, *(args->stack));
	// lock
	pthread_mutex_lock(&mutex_ans);
	memcpy(&pathArray[args->x][args->y],&p,sizeof(Path));
	aInt ++;
	// unlock
	pthread_mutex_unlock(&mutex_ans);

	return NULL;
}

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
    pthread_t threads[R*C];
    ThreadArg args[R*C];
    Path answer;

    pthread_mutex_init(&mutex_ans,NULL);
    aInt = 0;
    
    int maxCmb = maxCombo();
    int flag = 0;

    for(int targetCmb=maxCmb; targetCmb>=1 && answer.dirLen==-1; targetCmb--) { // at least 1 combo
        for(int bound=1; bound<=MAXSTEP && answer.dirLen==-1; bound++) { // iterative deepening
            fprintf(stderr, "boundary = %d, target combo = %d\n", bound, targetCmb);
	    Stack stacks[R*C];
	    //Path pathArray[R][C];	 // make it global
	    //#pragma omp parallel for
	    for(int pos=0; pos<R*C; pos++) {
		    int x=pos/C, y=pos%C;
		    args[pos].x = x;
		    args[pos].y = y;
		    args[pos].bound = bound;
		    args[pos].target = targetCmb;
		    args[pos].board = &boards[pos];
		    args[pos].stack = &stacks[pos];
		    //                fprintf(stderr, "(%d, %d)\n", x, y);
		    fprintf(stderr,"Created thread[%d] (x,y)=(%d,%d) ,bound=%d, targetCmb=%d\n---\n",pos,args[pos].x,args[pos].y,args[pos].bound,args[pos].target);
		    pthread_create(&threads[pos], NULL, ida_star_thread, &args[pos]);
		    /*
		    //Original metod;
		    Board b;	// might be useless when parallized
		    memcpy(&b, this, sizeof(Board));
		    Stack stack;
		    pathArray[x][y] = b.ida_star(x, y, Direction(null), 0, bound, targetCmb, stack);
		     */
		    /*
		       if(pathArray[x][y].dirLen != -1) {
		       answer = pathArray[x][y];
		       answer.startX = x; 
		       answer.startY = y;
		       break;
		       }
		     */
	    }
	    // join all the threads
	    for(int pos=0;pos<R*C;pos++){
		    //pthread_join(threads[pos],NULL);
	    }
	    fprintf(stderr,"bound %d\ttarget %d\taInt %d\n",bound,targetCmb,aInt);
	    for(int i=0; i<R && answer.dirLen==-1; i++){
		    for(int j=0; j<C && answer.dirLen==-1; j++){
			    fprintf(stderr,"%d\t",pathArray[i][i].dirLen);
			    if(pathArray[i][j].dirLen!=-1) {
				    answer = pathArray[i][j];
				    answer.startX = i;
				    answer.startY = j;
			    }
		    }
		    fprintf(stderr,"\n");
	    }
	    char trash;
	    scanf("%c",&trash);
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
    pthread_mutex_destroy(&mutex_ans);
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

	for(int color=1; color<=6 && m; color++) {
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
