初始想法使用IDA\*來做，平行的部分之後處理(在main裡)

一開始先把轉珠解法寫出來


file dependency:

main.cpp
|
-- MoveStone.h/.cc


input definition:

盤面是5\*6:
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

int readInput(char\* path, Board \*b); // 0 表示input OK

int eval(const Board \*b);  //回傳值是combo數？或是更能顯示盤面情況的數？


--- Heuristic ---
1. 消除會被消掉的珠子(包含掉落)，只留餘珠
2. 找出同顏色的珠子，並用行數排序
3. 找出相鄰三珠(更遠的應該 cost 會更高,忽略)，並且根據以下方式計算：
	1. 計算使左二珠要處於相鄰兩行的步數(110000 -> 0)
	2. 計算使最左右的兩珠相鄰三行的步數(101000 -> 0)
	3. 兩者相加(用妲己的想法)為總步數
	4. 假如為零，加一(因為真的零步就搞定的話，剛剛就消掉了)

Design Issue
計算 heuristic 跟 calcCombo 會做一樣的計算，可以抽出來並且記錄盤面(Heuristic 只需要剩餘盤面即可)，並把盤面存在 Board 裡？
我自己嘗試了一次，將 calcBoardCombo() 改為不 const, 並且存取 int[][] board\_fallen, 結果就失敗了，不知為何 =(
