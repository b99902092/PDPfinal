初始想法使用IDA*來做，平行的部分之後處理(在main裡)

一開始先把轉珠解法寫出來


file dependency:

main.cpp
|
-- MoveStone.h/.cc


input definition:

盤面是5*6:
oooooo
oooooo
oooooo
oooooo
oooooo

從file讀入，每個o可以是1~6的一個數字代表珠子顏色，例如:

123456
654321
123123
456456
162534

這樣的一個盤面。


function: (待補)

struct Board{}

int readInput(char* path, Board *b); // 0 表示input OK

int eval(const Board *b);  //回傳值是combo數？或是更能顯示盤面情況的數？



