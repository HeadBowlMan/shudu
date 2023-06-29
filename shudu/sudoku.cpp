#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include "sudoku.h"
#include "utils.h"

using namespace std;

void Sudoku::generate_finales() const
{
	vector<int> temp_line;
	for (int i = 1; i < 10; i++)
	{
		temp_line.push_back(i);
	}
	random_shuffle(temp_line.begin(), temp_line.end());
	int first_line[9];
	for (int i = 0; i < 9; i++)
	{
		first_line[i] = temp_line[i];
	}
	int row[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int line_mov[8] = { 3, 6, 1, 4, 7, 2, 5, 8 };
	int cur_num = 0;
	int** temp = new int* [9];
	for (int i = 0; i < 9; i++)
	{
		temp[i] = new int[9];
	}
	int* finales = new int[81 * final_num];
	do
	{
		for (int i = 0; i < 9; i++)
		{
			temp[0][i] = first_line[i];
		}
		for (int i = 1; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				temp[i][j] = first_line[(line_mov[i - 1] + j) % 9];
			}
		}
		do 
		{
			do 
			{
				do
				{
					for (int i = 0; i < 9; i++)
					{
						for (int j = 0; j < 9; j++)
						{
							finales[81 * cur_num + i * 9 + j] = temp[row[i]-1][j];
						}
					}

					cur_num++;

					if (cur_num == final_num)
					{
						char* boards = int_array2char_array(finales);
						ofstream outfile("finales.txt");
						if (!outfile)
						{
							cerr << "Failed to create finales file." << endl;
							return;
						}

						for (int i = 0; boards[i] != '\0'; i++)
						{
							outfile << boards[i];
						}

						delete[]boards;
						delete[]finales;
						for (int i = 0; i < 9; i++)
						{
							delete[]temp[i];
						}
						delete[]temp;
						return;
					}

				} while (permutation(row+6, row+8));
			} while (permutation(row+3, row+5));
		} while (permutation(row, row+2));
	} while (permutation(first_line, first_line+8));
}

char* Sudoku::int_array2char_array(int *array) const
{
	char* write_data = new char[81 * 3 * final_num];
	int idx = 0;
	for (int i = 0; i < final_num; i++)
	{
		for (int j = 0; j < 81; j++)
		{
			write_data[idx] = array[81 * i + j] + '0';
			idx++;

			if (j % 9 == 8)
			{
				write_data[idx] = '\n';
				idx++;
			}
	
		}

		if (i < final_num - 1)
		{
			write_data[idx] = '\n';
			idx++;
		}

	}
	write_data[idx] = '\0';
	return write_data;
}

// 查找未分配数字的单元格
bool find_unassigned(char **board, int& row, int& col) {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (board[row][col] == '$') {
                return true;   
            }
        }
    }
    return false;
}

int count_board_blanks(char** board)
{
	int num = 0;
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			if (board[row][col] == '$') {
				num++;
			}
		}
	}
	return num;
}

// 判断填入数字是否安全
bool is_legal(char **board, int row, int col, int num) {
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
bool solve(char **board) {
    int row, col;
    if (!find_unassigned(board, row, col)) {	// 查找未分配数字的单元格
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

// 判断数独解是否唯一
bool find_solutions(char **board, int &solution_num) {
	int row, col;
	if (!find_unassigned(board, row, col)) {	// 查找未分配数字的单元格
		solution_num++;
		return true;
	}
	for (int num = 1; num <= 9; num++) {        // 尝试在当前单元格中填入数字
		if (is_legal(board, row, col, num)) {
			board[row][col] = num + '0';
			if (find_solutions(board, solution_num)) {  // 递归解决剩余的数独
				if (solution_num >= 2)
				{
					return true;
				}// 如果解的数量大于等于2，则返回true
				board[row][col] = '$';
				break;
			}
			board[row][col] = '$';  // 如果解决失败，则回溯，并将当前单元格置为空格
		}
	}
	return false;
}

// 从文件中读取棋盘
bool read_board(std::ifstream& infile, char **board){
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
void write_board(std::ofstream& outfile, char **board) {
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
    char **board = new char*[9];
	for (int i = 0; i < 9; i++)
	{
		board[i] = new char[9];
	}
    while (read_board(infile, board)) {    // 从文件中读取数独棋盘
        solve(board);       // 解数独
        write_board(outfile, board);    // 将解答写入输出文件
    }
	for (int i = 0; i < 9; i++)
	{
		delete[]board[i];
	}
	delete[]board;
}

bool get_next_nonblank(char** board, int& row, int& col, int difficulty, bool** skip_place)
{
	random_device rd;
	mt19937 engine(rd());
	uniform_int_distribution<int> distribution(0, 8);
	bool all_true = true;
	switch (difficulty)
	{
	case EASY:
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (!skip_place[i][j])
				{
					all_true = false;
					break;
				}
			}
		}
		if (all_true)
		{
			break;
		}
		while (true)
		{
			row = distribution(engine);
			col = distribution(engine);
			if (!skip_place[row][col])
			{
				return true;
			}
		}
		break;
	case MIDDLE:
		for (int i = 0; i < 9; i++)
		{
			if (i % 2 == 0)
			{
				for (int j = 0; j < 9; j++)
				{
					if (j % 2 == 0 && !skip_place[i][j])
					{
						row = i;
						col = j;
						return true;
					}
				}
			}
			else
			{
				for (int j = 8; j >= 0; j--)
				{
					if (j % 2 != 0 && !skip_place[i][j])
					{
						row = i;
						col = j;
						return true;
					}
				}
			}
		}
		for (int i = 0; i < 9; i++)
		{
			if (i % 2 == 0)
			{
				for (int j = 0; j < 9; j++)
				{
					if (j % 2 != 0 && !skip_place[i][j])
					{
						row = i;
						col = j;
						return true;
					}
				}
			}
			else
			{
				for (int j = 8; j >= 0; j--)
				{
					if (j % 2 == 0 && !skip_place[i][j])
					{
						row = i;
						col = j;
						return true;
					}
				}
			}
		}
		break;
	case HARD:
		for (int i = 0; i < 9; i++)
		{
			if (i % 2 == 0)
			{
				for (int j = 0; j < 9; j++)
				{
					if (!skip_place[i][j])
					{
						row = i;
						col = j;
						return true;
					}
				}
			}
			else
			{
				for (int j = 8; j >= 0; j--)
				{
					if (!skip_place[i][j])
					{
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

void Sudoku::generate_games() const
{
	generate_finales();
	char*** finales = new char** [final_num];
	ifstream infile("finales.txt");
	if (!infile) {
		std::cerr << "Failed to open finales file" << endl;
		return;
	}
	for (int i = 0; i < final_num; i++)
	{
		finales[i] = new char*[9];
		for (int j = 0; j < 9; j++)
		{
			finales[i][j] = new char[9];
		}
		read_board(infile, finales[i]);
	}
	infile.close();
	bool** skip_place = new bool* [9];
	for (int i = 0; i < 9; i++)
	{
		skip_place[i] = new bool[9];
	}
	for (int i = 0; i < game_num; i++)
	{
		int cur_blanks = 0;
		for (int r = 0; r < 9; r++)
		{
			for (int c = 0; c < 9; c++)
			{
				skip_place[r][c] = false;
			}
		}
		while (cur_blanks < blank_num)
		{
			int row, col;
			if (!get_next_nonblank(finales[i], row, col, difficulty, skip_place))
			{
				// cerr << "Get next nonblank failed." << endl;

				generate_finales();
				ifstream infile("finales.txt");
				if (!infile) {
					std::cerr << "Failed to open finales file" << endl;
					return;
				}
				for (int t = i; t < final_num; t++)
				{
					read_board(infile, finales[t]);
				}
				infile.close();
				for (int r = 0; r < 9; r++)
				{
					for (int c = 0; c < 9; c++)
					{
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
			if (need_unique)
			{
				int solution_num = 0;
				char** board = new char* [9];
				for (int k = 0; k < 9; k++)
				{
					board[k] = new char[9];
				}
				for (int r = 0; r < 9; r++)
				{
					for (int c = 0; c < 9; c++)
					{
						board[r][c] = finales[i][r][c];
					}
				}
				find_solutions(board, solution_num);
				if (solution_num >= 2)
				{
					// cout << "not unique" << endl;
					finales[i][row][col] = no_str;
					cur_blanks--;
				}
			}
		}
		// cout << i << endl;
	}

	ofstream outfile(game_path);
	if (!outfile)
	{
		cerr << "Failed to create games file." << endl;
		return;
	}

	for (int i = 0; i<game_num; i++)
	{
		for (int row = 0; row < 9; row++)
		{
			for (int col = 0; col < 9; col++)
			{
				outfile << finales[i][row][col];
			}
			outfile << "\n";
		}
		outfile << "\n";
	}

	cout << "Games generation completed." << endl;
}
