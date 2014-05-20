struct Board {
    int board[30];
};

void readInput(char* path, Board *b);
int eval(Board *b);
void showBoard(Board *b);
