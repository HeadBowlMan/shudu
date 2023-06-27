#include <fstream>
#include <iostream>
#include "sudoku.h"
#include "utils.h"

using namespace std;

void Sudoku::generate_finales() const
{
	int first_line[9] = { 6, 1, 2, 3, 4, 5, 7, 8, 9 };
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

						cout << "Finales generation completed." << endl;

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