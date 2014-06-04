#define R 5
#define C 6
#define MAXSTEP 40
#define INF 1000000
#define SIZE(arr) (sizeof(arr)/sizeof(arr[0]))
#include<cstring>

typedef enum {null=0, up=8, down=2, left=4, right=6} Direction;

static const Direction dirList[] = { up, down, left, right };

struct Path {
    char dir[MAXSTEP+1];
    int dirLen;
    int startX, startY;
    Path() {
        memset(dir, 0, sizeof(dir));
        dirLen = startX = startY = -1;
    }
    void printReadablePath();
    /* max step 40, vertical and horizontal moves only for now */
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
    int maxCombo () const;
    Path solve() const; /* solve the pazzle and return path */

    private:
    void eliminateElement (int i, int j, int clr, int mark[R][C]);
    Path ida_star(int x, int y, Direction prevStep, int cost, int bound);
    int heuristic() const;
};

struct Stack {
    char s[MAXSTEP+1];
    int top;
    Stack() { top = 0; }
    void push(int x) { s[top++] = x; }
    char pop() { return s[--top]; }

};
static Stack stack;
