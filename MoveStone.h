#define R 5
#define C 6
#define MAXSTEP 40
#define INF 1000000
#define SIZE(arr) (sizeof(arr)/sizeof(arr[0])

typedef enum {null=0, up=8, down=2, left=4, right=6} Direction;

static const Direction dir_list[] = { up, down, left, right };

struct Path {
    Direction dir[MAXSTEP];
    int dir_len;
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
    int calcBoardValue () const;  /* return combo count of the board */
    int maxCombo () const;
    Path solve() const; /* solve the pazzle and return path */

    private:
    void eliminateElement (int i, int j, int clr, int mark[R][C]);
    Path ida_star(Direction prevStep, int cost, int bound);
};


