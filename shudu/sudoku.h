#pragma once
#include <string.h>

enum DIFFICULTY{EASY, MIDDLE, HARD};

class Sudoku
{
private:
	int final_num = 0;
	char* game_path = nullptr;
	int game_num = 0;
	int difficulty = EASY;
	int blank_num = 20;
	bool need_unique = false;
public:
	Sudoku(int final_num, char *game_path, int game_num, int difficulty, int blank_num, bool need_unique) {
		this->final_num = final_num;
		if (game_path != nullptr) {
			long long len = strlen(game_path);
			this->game_path = new char[len + 1];
			strncpy_s(this->game_path, len + 1, game_path, len);
		}
		this->game_num = game_num;
		this->difficulty = difficulty;
		this->blank_num = blank_num;
		this->need_unique = need_unique;
	}
	~Sudoku() {
		if (game_path != nullptr) {
			delete[]game_path;
			game_path = nullptr;
		}
	}

	//Sudoku(char* game_path) {
	//	if (game_path != nullptr) {
	//		long long len = strlen(game_path);
	//		this->game_path = new char[len + 1];
	//		strncpy_s(this->game_path, len + 1, game_path, len);
	//	}
	//}

	void generate_finales() const{}

	void solve_games() const;

	void generate_games() const{}

};