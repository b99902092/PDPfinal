#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
using namespace std;

void errorMsg(char *msg) { sprintf(stderr, "Error!, %s\n", msg); }

int readInput(char *path, Board *b) {
    FILE *fp = fopen(path, "r");
    char str[100];
    for(int i=0; i<5; i++) {
        if( scanf("%s", str) == -1 ) {
            errorMsg("file less than 5 rows");
            return -1;
        }
        if(strlen(str) != 6) {
            errorMsg("1 row must contain exactly 6 numbers");
            return -1;
        }
        for(int j=0; j<6; j++) {
            if(str[j]<'1' || str[j] >'6') {
                errorMsg("input contain invalid number");
                return -1;
            }
            b->board[i*6+j] = str[j] - '0';
        }
    }
    return 0;
}

void showBoard(board *b) {
    for(int i=0; i<5; i++) {
        for(int j=0; j<6; j++)
            sprintf(stderr, "%d", b->board[i*6+j]);
        sprintf(stderr, "\n");
    }
}
