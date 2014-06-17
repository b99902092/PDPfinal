#define R 5
#define C 6
#define MAXSTEP 30
#define INF 10000000
#define SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#include<cstring>

typedef enum {null=0, up=8, down=2, left=4, right=6} Direction;

static const Direction dirList[] = { up, down, left, right };

static int dx[4]={-1, 1, 0, 0}, dy[4]={0, 0, -1, 1};

static int order[24][4]={
    {0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 1, 3}, {0, 2, 3, 1}, {0, 3, 1, 2}, {0, 3, 2, 1},
    {1, 0, 2, 3}, {1, 0, 3, 2}, {1, 2, 0, 3}, {1, 2, 3, 0}, {1, 3, 0, 2}, {1, 3, 2, 0},
    {2, 0, 1, 3}, {2, 0, 3, 1}, {2, 1, 0, 3}, {2, 1, 3, 0}, {2, 3, 0, 1}, {2, 3, 1, 0},
    {3, 0, 1, 2}, {3, 0, 2, 1}, {3, 1, 0, 2}, {3, 1, 2 ,0}, {3, 2, 0, 1}, {3, 2, 1, 0}
};

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
    void printBoard () const;  /* just show the board */
    Board calcComboAndFallenBoard (int *rtv) const;  /* return combo count of the board */
    //void show_combo_cost() const;
    int maxCombo () const;
    Path solve() const; /* solve the pazzle and return path */
    //void* ida_star_thread (void *argPtr);
    //Path turnOne(int x,int y, int step,Path path);
    //void showTurnDetail(Path path) const;
    
    Path ida_star(int x, int y, Direction prevStep, int cost, int bound, int target, Stack &stack);// Try making it public

    private:
    void eliminateElement (int i, int j, int clr, int mark[R][C]);
    int heuristic() const;
    int calcDist(int a, int b, int c) const;
};

typedef struct threadArg{
	int x;
	int y;
	//Direction prevStep;
	//int cost; -> 0
	int bound;
	int target;
	Stack *stack;
	Board *board;
}ThreadArg;

