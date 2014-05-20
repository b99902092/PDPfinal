struct Board {
    int board[30];
};

int readInput(char* path, Board *b);
int eval(Board *b);
void showBoard(Board *b);
