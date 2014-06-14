#define R 5
#define C 6
#define MAXSTEP 15
#define INF 1000000
#define SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#include<cstring>

typedef enum {null=0, up=8, down=2, left=4, right=6} Direction;

static const Direction dirList[] = { up, down, left, right };

struct Stack {
    char s[MAXSTEP+1];
    Stack() { top = 0; }
    void push(int x) { s[top++] = x; }
    char pop() { return s[--top]; }
    int size() { return top; }
    void print(){
    	for(int i=0;i<top;i++){
		if(s[i] == 8) fprintf(stderr, "up");
		else if(s[i] == 2) fprintf(stderr, "down");
		else if(s[i] == 4) fprintf(stderr, "left");
		else if(s[i] == 6) fprintf(stderr, "right");
		fprintf(stderr," -> ");
	}
	fprintf(stderr,"\n");
    }
    private:
    int top;
};

struct Path {
    char dir[MAXSTEP+1];
    int dirLen;
    int startX, startY;
    Path() {
        memset(dir, 0, sizeof(dir));
        dirLen = startX = startY = -1;
    }
    void printReadablePath();
    /* max step 30, vertical and horizontal moves only for now */
    /* direction represent as:
     * 7 8 9
     * 4 x 6
     * 1 2 3
    */
};

struct Board {
    int board[R][C];
    /* 1  2  3  4  5  6
     * 7  8  9  10 11 12
     * 13 14 15 16 17 18
     * 19 20 21 22 23 24
     * 25 26 27 28 29 30 */
    int readInput (const char* path);  /* return value: 0 is OK */
    void showBoard () const;  /* just show the board */
    int calcBoardCombo () const;  /* return combo count of the board */
    void show_combo_cost() const;
    int maxCombo () const;
    Path solve() const; /* solve the pazzle and return path */
    //Path turnOne(int x,int y, int step,Path path);
    void showTurnDetail(Path path) const;

    private:
    void eliminateElement (int i, int j, int clr, int mark[R][C]);
    Path ida_star(int x, int y, Direction prevStep, int cost, int bound, int target, Stack &stack);
    int heuristic(int do_fall) const;
    int calcDist3(int a, int b, int c) const;
};

