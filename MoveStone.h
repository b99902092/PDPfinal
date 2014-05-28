#define R 5
#define C 6

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
    private:
    void eliminateElement (int i, int j, int clr, int mark[R][C]);
};

