#include <fstream>
#include "pch.h"
#include "../sudoku/sudoku.h"
TEST(test_int_array_to_char_array, test_convert)
{
	int int_array[162] = {
		1,2,3,4,5,6,7,8,9,
		2,3,4,5,6,7,8,9,1,
		3,4,5,6,7,8,9,1,2,
		4,5,6,7,8,9,1,2,3,
		5,6,7,8,9,1,2,3,4,
		6,7,8,9,1,2,3,4,5,
		7,8,9,1,2,3,4,5,6,
		8,9,1,2,3,4,5,6,7,
		9,1,2,3,4,5,6,7,8,
		1,2,3,4,5,6,7,8,9,
		2,3,4,5,6,7,8,9,1,
		3,4,5,6,7,8,9,1,2,
		4,5,6,7,8,9,1,2,3,
		5,6,7,8,9,1,2,3,4,
		6,7,8,9,1,2,3,4,5,
		7,8,9,1,2,3,4,5,6,
		8,9,1,2,3,4,5,6,7,
		9,1,2,3,4,5,6,7,8
	};
	char expect_char_array[182] = {
		"123456789\n"
		"234567891\n"
		"345678912\n"
		"456789123\n"
		"567891234\n"
		"678912345\n"
		"789123456\n"
		"891234567\n"
		"912345678\n"
		"\n"
		"123456789\n"
		"234567891\n"
		"345678912\n"
		"456789123\n"
		"567891234\n"
		"678912345\n"
		"789123456\n"
		"891234567\n"
		"912345678\n"
	};
	Sudoku s;
	char* char_array = s.int_array2char_array(int_array);
	char* expect_char_array_ptr = expect_char_array;
	for (int i = 0; i < 181; i++)
	{
		EXPECT_EQ(*char_array++, *expect_char_array_ptr++);
	}
}

TEST(test_is_legal, check)
{
	char board[9][9] = {
		{'$','1','$','$','$','$','$','$','$'},
		{'6','$','7','$','5','4','$','2','$'},
		{'4','$','5','$','$','$','$','$','$'},
		{'$','4','$','$','$','$','$','$','$'},
		{'$','5','$','$','$','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
		{'$','9','$','$','$','$','$','$','$'},
		{'$','8','$','$','$','$','$','$','$'},
	};
	char** example = new char*[9];
	for (int i = 0; i < 9; i++)
	{
		example[i] = new char[9];
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			example[i][j] = board[i][j];
		}
	}
	Sudoku s;
	EXPECT_EQ(s.is_legal(example, 1, 1, 4), false);
	EXPECT_EQ(s.is_legal(example, 1, 1, 7), false);
	EXPECT_EQ(s.is_legal(example, 0, 0, 5), false);
	EXPECT_EQ(s.is_legal(example, 1, 1, 3), true);
}

TEST(test_find_unassigned, get_blank)
{
	char board1[9][9] = {
		{'3','1','8','9','6','2','4','5','7'},
		{'6','$','7','$','5','4','$','2','$'},
		{'4','$','5','$','$','$','$','$','$'},
		{'$','4','$','$','$','$','$','$','$'},
		{'$','5','$','$','$','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
		{'$','9','$','$','$','$','$','$','$'},
		{'$','8','$','$','$','$','$','$','$'},
	};
	char** example = new char* [9];
	for (int i = 0; i < 9; i++)
	{
		example[i] = new char[9];
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			example[i][j] = board1[i][j];
		}
	}
	int row, col;
	Sudoku s;
	
	EXPECT_EQ(s.find_unassigned(example, row, col), true);
	EXPECT_EQ(row, 1);
	EXPECT_EQ(col, 1);

	char board2[9][9] = {
		{'3','1','8','9','6','2','4','5','7'},
		{'3','1','8','9','6','2','4','5','7'},
		{'3','1','8','9','6','2','4','5','7'},
		{'3','1','8','9','6','2','4','5','7'},
		{'3','1','8','9','6','2','4','5','7'},
		{'3','1','8','9','6','2','4','5','7'},
		{'3','1','8','9','6','2','4','5','7'},
		{'3','1','8','9','6','2','4','5','7'},
		{'3','1','8','9','6','2','4','5','7'},
	};
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			example[i][j] = board2[i][j];
		}
	}
	EXPECT_EQ(s.find_unassigned(example, row, col), false);
	EXPECT_EQ(row, 9);
	EXPECT_EQ(col, 9);
}

TEST(test_get_next_nonblank, get_nonblank)
{
	Sudoku s1(1, nullptr, 1, EASY, 20, false);
	bool** skip = new bool* [9];
	for (int i = 0; i < 9; i++)
	{
		skip[i] = new bool[9];
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			skip[i][j] = false;
		}
	}
	int row, col;

	EXPECT_EQ(s1.get_next_nonblank(row, col, skip), true);

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			skip[i][j] = true;
		}
	}
	EXPECT_EQ(s1.get_next_nonblank(row, col, skip), false);

	Sudoku s2(1, nullptr, 1, MIDDLE, 20, false);
	EXPECT_EQ(s2.get_next_nonblank(row, col, skip), false);

	skip[0][0] = false;
	EXPECT_EQ(s2.get_next_nonblank(row, col, skip), true);
	EXPECT_EQ(row, 0);
	EXPECT_EQ(col, 0);

	skip[0][0] = true;
	skip[1][7] = false;
	EXPECT_EQ(s2.get_next_nonblank(row, col, skip), true);
	EXPECT_EQ(row, 1);
	EXPECT_EQ(col, 7);

	skip[1][7] = true;
	skip[0][1] = false;
	EXPECT_EQ(s2.get_next_nonblank(row, col, skip), true);
	EXPECT_EQ(row, 0);
	EXPECT_EQ(col, 1);

	skip[0][1] = true;
	skip[1][8] = false;
	EXPECT_EQ(s2.get_next_nonblank(row, col, skip), true);
	EXPECT_EQ(row, 1);
	EXPECT_EQ(col, 8);

	Sudoku s3(1, nullptr, 1, HARD, 20, false);
	EXPECT_EQ(s3.get_next_nonblank(row, col, skip), true);
	EXPECT_EQ(row, 1);
	EXPECT_EQ(col, 8);

	skip[1][8] = true;
	skip[2][0] = false;
	EXPECT_EQ(s3.get_next_nonblank(row, col, skip), true);
	EXPECT_EQ(row, 2);
	EXPECT_EQ(col, 0);
}

TEST(test_generate_finales, generate_max_finales)
{
	Sudoku s(1000000, nullptr, 1, MIDDLE, 20, false);
	s.generate_finales();
	char*** finales = new char** [1000000];
	std::ifstream infile("finales.txt");
	if (!infile) {
		std::cerr << "Failed to open finales file" << endl;
		return;
	}
	int i;
	for (i = 0; i < 1000000; i++) {
		finales[i] = new char* [9];
		for (int j = 0; j < 9; j++) {
			finales[i][j] = new char[9];
		}
		s.read_board(infile, finales[i]);
	}
	infile.close();
	EXPECT_EQ(i, 1000000);
}

TEST(test_solve, solve_game)
{
	char board1[9][9] = {
		{'$','$','7','$','1','$','8','$','5'},
		{'3','$','$','8','$','$','$','2','7'},
		{'$','$','$','$','2','$','$','$','$'},
		{'2','$','3','$','6','$','$','5','$'},
		{'$','6','$','$','$','$','$','$','$'},
		{'4','$','$','$','7','3','1','$','8'},
		{'$','3','1','$','$','$','5','$','2'},
		{'6','$','4','$','9','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
	};
	char** example = new char* [9];
	for (int i = 0; i < 9; i++)
	{
		example[i] = new char[9];
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			example[i][j] = board1[i][j];
		}
	}
	Sudoku s;
	EXPECT_EQ(s.solve(example), true);
}

TEST(test_find_solutions, check_if_unique)
{
	char board1[9][9] = {
		{'$','$','7','$','1','$','8','$','5'},
		{'3','$','$','8','$','$','$','2','7'},
		{'$','$','$','$','2','$','$','$','$'},
		{'2','$','3','$','6','$','$','5','$'},
		{'$','6','$','$','$','$','$','$','$'},
		{'4','$','$','$','7','3','1','$','8'},
		{'$','3','1','$','$','$','5','$','2'},
		{'6','$','4','$','9','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
	};
	char** example = new char* [9];
	for (int i = 0; i < 9; i++)
	{
		example[i] = new char[9];
	}
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			example[i][j] = board1[i][j];
		}
	}
	Sudoku s;
	int num = 0;
	s.find_solutions(example, num);
	EXPECT_EQ(num, 1);

	char board2[9][9] = {
		{'$','$','7','$','1','$','8','$','5'},
		{'3','$','$','8','$','$','$','2','7'},
		{'$','$','$','$','2','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
		{'$','3','1','$','$','$','5','$','2'},
		{'6','$','4','$','9','$','$','$','$'},
		{'$','$','$','$','$','$','$','$','$'},
	};
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			example[i][j] = board2[i][j];
		}
	}
	num = 0;
	s.find_solutions(example, num);
	EXPECT_EQ(num, 2);
}

TEST(test_generate_games, check_blank_num)
{
	Sudoku s1(1, "game.txt", 1, EASY, 20, false);
	s1.generate_games();
	std::ifstream infile1("game.txt");
	char** board = new char* [9];
	for (int i = 0; i < 9; i++) {
		board[i] = new char[9];
	}
	s1.read_board(infile1, board);
	infile1.close();
	int num = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] == '$')
			{
				num++;
			}
		}
	}
	EXPECT_EQ(num, 20);

	Sudoku s2(1, "game.txt", 1, EASY, 55, true);
	s2.generate_games();
	std::ifstream infile2("game.txt");
	s1.read_board(infile2, board);
	infile2.close();
	num = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] == '$')
			{
				num++;
			}
		}
	}
	EXPECT_EQ(num, 55);
}

TEST(test_generate_games, check_unique)
{
	Sudoku s(1, "game.txt", 1, EASY, 55, true);
	s.generate_games();
	std::ifstream infile1("game.txt");
	char** board = new char* [9];
	for (int i = 0; i < 9; i++) {
		board[i] = new char[9];
	}
	s.read_board(infile1, board);
	infile1.close();
	int num = 0;
	s.find_solutions(board, num);
	EXPECT_EQ(num, 1);
}

TEST(test_generate_games, check_max_num)
{
	Sudoku s(10000, "game.txt", 10000, MIDDLE, 40, true);
	s.generate_games();
	std::ifstream infile("game.txt");
	char*** games = new char** [10000];
	int i;
	for (i = 0; i < 10000; i++) {
		games[i] = new char* [9];
		for (int j = 0; j < 9; j++) {
			games[i][j] = new char[9];
		}
		s.read_board(infile, games[i]);
	}
	infile.close();
	EXPECT_EQ(i, 10000);
}