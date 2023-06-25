#pragma once
#include <string.h>

enum DIFFICULTY{EASY, MIDDLE, HARD};

class Sudoku
{
private:
	int n_finale = 0;
	char* game_path = nullptr;
	int n_game = 0;
	int difficulty = EASY;
	int n_blank = 20;
	bool unique = false;
public:
	Sudoku(int n_final, char *game_path, int n_game, int difficulty, int n_blank, bool unique) {
		this->n_finale = n_final;
		if (game_path != nullptr) {
			int len = strlen(game_path);
			this->game_path = new char[len + 1];
			strncpy_s(this->game_path, len + 1, game_path, len);
		}
		this->n_game = n_game;
		this->difficulty = difficulty;
		this->n_blank = n_blank;
		this->unique = unique;
	}
	~Sudoku() {
		if (game_path != nullptr) {
			delete[]game_path;
			game_path = nullptr;
		}
	}

	void generate_finales() const{}

	void solve_games() const{}

	void generate_games() const{}

};