
/*
*Copyright [2023] <Copyright shn yzx>
*/

#include "sudoku/sudoku.h"

#include <fstream>
#include <iostream>
#include <random>
#include <vector>

#include "sudoku/utils.h"

using std::cout;

void Sudoku::generate_finales() const {
    vector<int> temp_line;
    for (int i = 1; i < 10; i++) {
        temp_line.push_back(i);
    }
    random_shuffle(temp_line.begin(), temp_line.end());
    int first_line[9];
    for (int i = 0; i < 9; i++) {
        first_line[i] = temp_line[i];
    }
    int row[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int line_mov[8] = { 3, 6, 1, 4, 7, 2, 5, 8 };
    int cur_num = 0;
    int** temp = new int* [9];
    for (int i = 0; i < 9; i++) {
        temp[i] = new int[9];
    }
    int* finales = new int[81 * (long long)final_num];
    do {
        for (int i = 0; i < 9; i++) {
            temp[0][i] = first_line[i];
        }
        for (int i = 1; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                temp[i][j] = first_line[(line_mov[i - 1] + j) % 9];
            }
        }
        do {
            do {
                do {
                    for (int i = 0; i < 9; i++) {
                        for (int j = 0; j < 9; j++) {
                            finales[81 * cur_num + i * 9 + j]
                                = temp[row[i] - 1][j];
                        }
                    }

                    cur_num++;

                    if (cur_num == final_num) {
                        char* boards = int_array2char_array(finales);
                        ofstream outfile("finales.txt");
                        if (!outfile) {
                            cerr << "Failed to create finales file." << endl;
                            return;
                        }

                        for (int i = 0; boards[i] != '\0'; i++) {
                            outfile << boards[i];
                        }

                        delete[] boards;
                        delete[] finales;
                        for (int i = 0; i < 9; i++) {
                            delete[] temp[i];
                        }
                        delete[] temp;
                        return;
                    }
                } while (permutation(row + 6, row + 8));
            } while (permutation(row + 3, row + 5));
        } while (permutation(row, row + 2));
    } while (permutation(first_line, first_line + 8));
}

char* Sudoku::int_array2char_array(int* array) const {
    char* write_data = new char[81 * 3 * (long long)final_num];
    int idx = 0;
    for (int i = 0; i < final_num; i++) {
        for (int j = 0; j < 81; j++) {
            write_data[idx] = array[81 * i + j] + '0';
            idx++;

            if (j % 9 == 8) {
                write_data[idx] = '\n';
                idx++;
            }
        }

        if (i < final_num - 1) {
            write_data[idx] = '\n';
            idx++;
        }
    }
    write_data[idx] = '\0';
    return write_data;
}

// Find cells that don't have a number assigned
bool Sudoku::find_unassigned(char** board, int& row, int& col) const{
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == '$') {
                return true;
            }
        }
    }
    return false;
}

bool Sudoku::solve(char** board) const{
    int row, col;
    if (!find_unassigned(board, row, col)) {
        return true;
    }
    for (int num = 1; num <= 9; num++) {
        if (is_legal(board, row, col, num)) {
            board[row][col] = num + '0';
            if (solve(board)) {
                return true;
            }
            board[row][col] = '$';
        }
    }
    return false;
}

// Determine whether the Sudoku solution is unique
bool Sudoku::find_solutions(char** board, int& solution_num) const{
    int row, col;
    if (!find_unassigned(board, row, col)) {
        solution_num++;
        return true;
    }
    for (int num = 1; num <= 9; num++) {
        if (is_legal(board, row, col, num)) {
            board[row][col] = num + '0';
            if (find_solutions(board, solution_num)) {
                if (solution_num >= 2) {
                    return true;
                }
                board[row][col] = '$';
                break;
            }
            board[row][col] = '$';
        }
    }
    return false;
}

// Read the checkerboard from a file
bool Sudoku::read_board(std::ifstream& infile, char** board) const{
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            char ch;
            infile.get(ch);
            if (!infile) {
                return false;
            }
            if (ch == '\n') {
                col--;
            } else {
                board[row][col] = ch;
            }
        }
    }
    return true;
}

// Write the Sudoku board to a file
void write_board(std::ofstream& outfile, char** board) {
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

void Sudoku::solve_games() const {
    if (game_path == nullptr) {
        std::cerr << "No game file." << std::endl;
        return;
    }
    std::ifstream infile(game_path);
    if (!infile) {
        std::cerr << "Failed to open game file: " << game_path << std::endl;
        return;
    }
    std::ofstream outfile("sudoku.txt");
    if (!outfile) {
        std::cerr << "Failed to create output file." << std::endl;
        return;
    }
    char** board = new char* [9];
    for (int i = 0; i < 9; i++) {
        board[i] = new char[9];
    }
    while (read_board(infile, board)) {
        solve(board);
        write_board(outfile, board);
    }
    for (int i = 0; i < 9; i++) {
        delete[] board[i];
    }
    delete[] board;
}

bool Sudoku::get_next_nonblank(int& row, int& col, 
    bool** skip_place)  const{
    random_device rd;
    mt19937 engine(rd());
    uniform_int_distribution<int> distribution(0, 8);
    bool all_true = true;
    switch (difficulty) {
    case EASY:
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (!skip_place[i][j]) {
                    all_true = false;
                    break;
                }
            }
        }
        if (all_true) {
            break;
        }
        while (true) {
            row = distribution(engine);
            col = distribution(engine);
            if (!skip_place[row][col]) {
                return true;
            }
        }
        break;
    case MIDDLE:
        for (int i = 0; i < 9; i++) {
            if (i % 2 == 0) {
                for (int j = 0; j < 9; j++) {
                    if (j % 2 == 0 && !skip_place[i][j]) {
                        row = i;
                        col = j;
                        return true;
                    }
                }
            } else {
                for (int j = 8; j >= 0; j--) {
                    if (j % 2 != 0 && !skip_place[i][j]) {
                        row = i;
                        col = j;
                        return true;
                    }
                }
            }
        }
        for (int i = 0; i < 9; i++) {
            if (i % 2 == 0) {
                for (int j = 0; j < 9; j++) {
                    if (j % 2 != 0 && !skip_place[i][j]) {
                        row = i;
                        col = j;
                        return true;
                    }
                }
            } else {
                for (int j = 8; j >= 0; j--) {
                    if (j % 2 == 0 && !skip_place[i][j]) {
                        row = i;
                        col = j;
                        return true;
                    }
                }
            }
        }
        break;
    case HARD:
        for (int i = 0; i < 9; i++) {
            if (i % 2 == 0) {
                for (int j = 0; j < 9; j++) {
                    if (!skip_place[i][j]) {
                        row = i;
                        col = j;
                        return true;
                    }
                }
            } else {
                for (int j = 8; j >= 0; j--) {
                    if (!skip_place[i][j]) {
                        row = i;
                        col = j;
                        return true;
                    }
                }
            }
        }
        break;
    default:
        break;
    }
    return false;
}

void Sudoku::generate_games() const {
    generate_finales();
    char*** finales = new char** [final_num];
    ifstream infile("finales.txt");
    if (!infile) {
        std::cerr << "Failed to open finales file" << endl;
        return;
    }
    for (int i = 0; i < final_num; i++) {
        finales[i] = new char* [9];
        for (int j = 0; j < 9; j++) {
            finales[i][j] = new char[9];
        }
        read_board(infile, finales[i]);
    }
    infile.close();
    bool** skip_place = new bool* [9];
    for (int i = 0; i < 9; i++) {
        skip_place[i] = new bool[9];
    }
    for (int i = 0; i < final_num; i++) {
        int cur_blanks = 0;
        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                skip_place[r][c] = false;
            }
        }
        while (cur_blanks < blank_num) {
            int row, col;
            if (!get_next_nonblank(row, col,
                 skip_place)) {
                // cerr << "Get next nonblank failed." << endl;

                generate_finales();
                ifstream infile("finales.txt");
                if (!infile) {
                    std::cerr << "Failed to open finales file" << endl;
                    return;
                }
                for (int t = i; t < final_num; t++) {
                    read_board(infile, finales[t]);
                }
                infile.close();
                for (int r = 0; r < 9; r++) {
                    for (int c = 0; c < 9; c++) {
                        skip_place[r][c] = false;
                    }
                }
                cur_blanks = 0;
                continue;
            }
            char no_str = finales[i][row][col];
            finales[i][row][col] = '$';
            skip_place[row][col] = true;
            cur_blanks++;
            if (need_unique) {
                int solution_num = 0;
                char** board = new char* [9];
                for (int k = 0; k < 9; k++) {
                    board[k] = new char[9];
                }
                for (int r = 0; r < 9; r++) {
                    for (int c = 0; c < 9; c++) {
                        board[r][c] = finales[i][r][c];
                    }
                }
                find_solutions(board, solution_num);
                if (solution_num >= 2) {
                    // cout << "not unique" << endl;
                    finales[i][row][col] = no_str;
                    cur_blanks--;
                }
            }
        }
        // cout << i << endl;
    }

    ofstream outfile(game_path);
    if (!outfile) {
        cerr << "Failed to create games file." << endl;
        return;
    }

    for (int i = 0; i < game_num; i++) {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                outfile << finales[i][row][col];
            }
            outfile << "\n";
        }
        outfile << "\n";
    }
    cout << "Games generation completed." << endl;
}

// Determine whether the number entered is legitimate
bool Sudoku::is_legal(char** board, int row, int col, int num) const
{
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
