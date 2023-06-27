// shudu.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include "sudoku.h"

int main()
{
    char test[] = { "game.txt" };
    Sudoku sudo(test);
    sudo.solve_games();
    std::cout << "Hello World!\n";
}

// 查找未分配数字的单元格
bool find_unassigned(const char board[][9], int& row, int& col) {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == '$') {
                return true;   
            }
        }
    }
    return false;
}

// 判断填入数字是否安全
bool is_legal(const char board[][9], int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num + '0') {
            return false;
        }
        if (board[i][col] == num + '0') {
            return false;
        }
        if (board[3 * (row / 3) + i / 3][3 * (col / 3) + i % 3] == num + '0') {
            return false;
        }
    }
    return true;  
}

// 求解数独
bool solve(char board[][9]) {
    int row, col;
    if (!find_unassigned(board, row, col)) {    // 查找未分配数字的单元格
        return true;   
    }
    for (int num = 1; num <= 9; num++) {        // 尝试在当前单元格中填入数字
        if (is_legal(board, row, col, num)) {    
            board[row][col] = num + '0';       
            if (solve(board)) {     // 递归解决剩余的数独
                return true;        // 如果解决成功，则返回true
            }
            board[row][col] = '$';  // 如果解决失败，则回溯，并将当前单元格置为空格
        }
    }
    return false;  
}

// 从文件中读取棋盘
bool read_board(std::ifstream& infile, char board[][9]){
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            char ch;
            infile.get(ch);
            if (!infile) {
                return false;   // 读取失败，返回false
            }
            if (ch == '\n') {
                col--;  // 如果读取到换行符，则回退一格 
            }
            else {
                board[row][col] = ch;
            }
        }
    }
    return true;    //读取成功，返回true
}

// 将数独棋盘写入文件
void write_board(std::ofstream& outfile, const char board[][9]) {
    outfile << "-------------------" << std::endl;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            outfile << board[row][col] << " ";
        }
        outfile << std::endl;
        if (row % 9 == 8) {
            outfile << "-------------------" << std::endl;
        }
    }
    outfile << std::endl;
}

// 解数独
void Sudoku::solve_games() const {
    if (game_path == nullptr) {
        std::cerr << "No game file." << std::endl;
        return;
    }
    std::ifstream infile(game_path);   // 打开数独棋盘文件
    if (!infile) {
        std::cerr << "Failed to open game file: " << game_path << std::endl;
        return;
    }
    std::ofstream outfile("sudoku.txt");    // 创建输出文件
    if (!outfile) {
        std::cerr << "Failed to create output file." << std::endl;
        return;
    }
    char board[9][9];
    while (read_board(infile, board)) {    // 从文件中读取数独棋盘
        solve(board);       // 解数独
        write_board(outfile, board);    // 将解答写入输出文件
    }
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
