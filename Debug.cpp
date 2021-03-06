#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <vector>
#include "MoveStone.h"
using namespace std;

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
    for(int i=0; i<SIZE(dx); i++) {
        if(x+dx[i]<0 || x+dx[i]>=R || y+dy[i]<0 || y+dy[i]>=C) continue;
        if(mark[x+dx[i]][y+dy[i]] == clr)
            eliminateElement(x+dx[i], y+dy[i], clr, mark);
    }
}

//=======================================

int Board::calcBoardCombo() const {
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
    memcpy(&b, this, sizeof(Board));

    Path answer;
    
    int maxCmb = maxCombo();


    for(int targetCmb=maxCmb; targetCmb>=1; targetCmb--) { // at least 1 combo
        for(int bound=1; bound<=MAXSTEP && answer.dirLen==-1; bound++) { // iterative deepening
            fprintf(stderr, "boundary = %d, target combo = %d\n", bound, targetCmb);
            for(int pos=0; pos<R*C; pos++) {
                int x=pos/C, y=pos%C;
                Stack stack;
//                fprintf(stderr, "(%d, %d)\n", x, y);
                Path p = b.ida_star(x, y, Direction(null), 0, bound, targetCmb, stack);

                if(p.dirLen != -1) {
                    answer = p;
                    answer.startX = x; 
                    answer.startY = y;
                    break;
                }
            }
        }
    }
    return answer;
}

int Board::calcDist3(int a, int b, int c) const{
// A--d1--B--d2---C
	int d = b-a-1 > 0 ? b-a-1 : 0;
	d += c-b-1 > 0 ? c-b-1 : 0;	// calc d2, c-b
	return d;
}

int Board::heuristic(int do_fall) const{
    //return 0;
    Board b;
    memcpy(&b, this, sizeof(Board));

    if(do_fall != 0){
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
	    }
    }
    // After all gems fallen / or do_fall == 0
    //b.showBoard();
    int min=-1;
    vector<int> buffer;
    for(int color=1;color<7;color++){
    	buffer.clear();
	for(int j=0; j<C; j++){
		for(int i=0; i<R; i++) {
			if(b.board[i][j]==color){
				buffer.push_back(j);
			}
		}
	}
	// For each color, find the minimum step to gather all gems together.
	/*
	// XXX Debug 
	for(int i=0;i<buffer.size();i++){
		fprintf(stderr,"%d->",buffer.at(i));
	}
	if(buffer.size()>0) 
		fprintf(stderr,"\n");
		*/

	if(buffer.size()<3){
		continue;
	}else{
		for(int i=0;i<buffer.size()-2;i++){
			int d = calcDist3(buffer.at(i),buffer.at(i+1),buffer.at(i+2));
			if(d<min || min==-1)
				min = d;
		}
		if(min == 0)
			return 1;	// actually, we still need at least one more move to eliminate more gems.
	}
    }
    if(min==-1){
    	return 0;
    }else{
	    return min;
    }
}

Path Board::ida_star(int x, int y, Direction prevStep, int cost, int bound, int target, Stack &stack) {
	Path path;
	int h = heuristic(cost);	// that is, when cost = 0 -> count before falling
	int f = cost + h;

	if(f > bound) { //fail to continue
		//fprintf(stderr, "\tDIE! -> cost = %d, bound = %d, heuristic = %d\n", cost, bound, h); 
		//stack.print();
		//showBoard();
		//getc(stdin);
		return path;
	}
	// XXX
	/*
	path.printReadablePath();
	showBoard();
	fprintf(stderr, "cost = %d, bound = %d, target = %d, board combo = %d, heuristic = %d\n", cost, bound, target, calcBoardCombo(),h); 
	*/
	if(calcBoardCombo() >= target) { //find solution, return a solution path
		fprintf(stderr, "find sol!, %d\n", calcBoardCombo());
		showBoard();
		strncpy(path.dir, stack.s, sizeof(char)*stack.size());
		path.dir[stack.size()] = 0;
		path.dirLen = strlen(path.dir);
		return path;
	}
	int dx[] = {-1, 1, 0, 0}, dy[] = {0, 0, -1, 1};
	for(int i=0; i<SIZE(dirList); i++) {
		if(dirList[i] == (10 - prevStep)) continue; //no going back
		if(x+dx[i]<0 || x+dx[i]>=R || y+dy[i]<0 || y+dy[i]>=C) continue;
		stack.push(dirList[i]);
		swap(board[x][y], board[x+dx[i]][y+dy[i]]);
		// XXX
		/*
		fprintf(stderr,"(%d, %d) -> ",x,y);
		stack.print();*/

		Path p = ida_star(x+dx[i], y+dy[i], dirList[i], cost+1, bound, target, stack);

		if(p.dirLen != -1) {
			if(path.dirLen == -1 || p.dirLen < path.dirLen) {
				path = p;
				break; // can be deleted if we want to find multiple sols someday.
			}
		}

		swap(board[x][y], board[x+dx[i]][y+dy[i]]);
		stack.pop();
	}

	return path;
}

void Board::show_combo_cost() const{
	int h = heuristic(1);	// that is, when cost = 0 -> count before falling
	fprintf(stderr, "board combo = %d, heuristic = %d\n",calcBoardCombo(),h); 
}
void Board::showTurnDetail(Path path) const{
	Board b;
	int x = path.startX;
	int y = path.startY;
	memcpy(&b, this, sizeof(Board));
	fprintf(stderr, "show initial board:\n");
	showBoard();
	for(int i=0;i<path.dirLen;i++){
		// Turn the gem
		int j=-1;
		int dx[] = {-1, 1, 0, 0}, dy[] = {0, 0, -1, 1};
		switch(path.dir[i]){
			case 8:	// up
				j = 0;
				break;
			case 2:	// down
				j = 1;
				break;
			case 4:	// left
				j = 2;
				break;
			case 6:	// right
				j = 3;
				break;
			default:
				fprintf(stderr,"Error path dir-%c\n",path.dir[i]);
				break;
		}
		swap(b.board[x][y], b.board[x+dx[j]][y+dy[j]]);
		x = x+dx[j];
		y = y+dy[j];
		// print out the status
		b.showBoard();
		b.show_combo_cost();
	}
}
/*
Path Board::turnOne(int x,int y, int step,Path path){
	Path p=path;
	//memcpy(&p, path, sizeof(Path));
	int dx[] = {-1, 1, 0, 0}, dy[] = {0, 0, -1, 1};
	int i;
	if(step == 4){
		i = 1;
	}else if(step == 6){
		i = 2;
	}else if(step == 2){
		i = 3;
	}else if(step == 8){
		i = 4;
	}else{
		fprintf(stderr,"Error: Invalid direction!(l/r/u/d)\n");
		return p;
	}
	if(x+dx[i]<0 || x+dx[i]>=R || y+dy[i]<0 || y+dy[i]>=C){
		fprintf(stderr,"Error: Invalid step!\n");
	}else{
		p.dir[p.dirLen++] = step;
		swap(board[x][y], board[x+dx[i]][y+dy[i]]);
		showBoard();
		show_combo_cost();
	}
	return p;
}*/
