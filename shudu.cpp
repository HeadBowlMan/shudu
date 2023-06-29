
/*
*Copyright [2023] <Copyright shn yzx>
*/
#include <iostream>

#include "sudoku/sudoku.h"

// using namespace std;

int main(int argc, char* argv[]) {
  int final_num = 0;
  char* game_path = nullptr;
  int game_num = 0;
  int difficulty = 0;
  int blank_num = 0;
  bool need_unique = false;
  if (argc < 2) {
    cerr << "Input format error." << endl;
    return 1;
  }
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-c") == 0) {
      i++;
      if (i >= argc) {
        cerr << "The number of finales to be generated needs to be provided "
                "after '-c'."
             << endl;
        return 1;
      }
      final_num = atoi(argv[i]);
      if (final_num < 1 || final_num > 1000000) {
        cerr << "The number of finales to be generated is invalid." << endl;
        return 1;
      }
    } else if (strcmp(argv[i], "-s") == 0) {
      i++;
      if (i >= argc) {
        cerr << "The path of games file needs to be provided after '-s'."
             << endl;
        return 1;
      }
      game_path = argv[i];
    } else if (strcmp(argv[i], "-n") == 0) {
      i++;
      if (i >= argc) {
        cerr << "The number of games to be generated needs to be provided "
                "after '-n'."
             << endl;
        return 1;
      }
      game_num = atoi(argv[i]);
      final_num = game_num;
      if (game_num < 1 || game_num > 10000) {
        cerr << "The number of games to be generated is invalid." << endl;
        return 1;
      }
    } else if (strcmp(argv[i], "-m") == 0) {
      i++;
      if (i >= argc) {
        cerr << "The difficulty of games to be generated needs to be provided "
                "after '-m'."
             << endl;
        return 1;
      }
      difficulty = atoi(argv[i]);
      if (difficulty < EASY || difficulty > HARD) {
        cerr << "The difficulty of games to be generated is invalid." << endl;
        return 1;
      }
    } else if (strcmp(argv[i], "-r") == 0) {
      i++;
      if (i >= argc) {
        cerr << "The number of blanks in each game to be generated needs to be "
                "provided after '-r'."
             << endl;
        return 1;
      }
      blank_num = atoi(argv[i]);
      if (blank_num < 20 || blank_num > 55) {
        cerr << "The number of blanks in each game to be generated is invalid."
             << endl;
        return 1;
      }
    } else if (strcmp(argv[i], "-u") == 0) {
      need_unique = true;
    } else {
      cerr << "Invalid command." << endl;
      return 1;
    }
  }
  if (game_num != 0) {
    if (difficulty == 0) {
      difficulty = 1;
    }
    if (blank_num == 0) {
      blank_num = 20;
    }
    char path[] = {"game.txt"};
    game_path = path;
  }
  if (difficulty != 0 && game_num == 0) {
    cerr << "'-m' needs to be used with '-n'." << endl;
    return 1;
  }
  if (blank_num != 0 && game_num == 0) {
    cerr << "'-r' needs to be used with '-n'." << endl;
    return 1;
  }
  if (need_unique != 0 && game_num == 0) {
    cerr << "'-u' needs to be used with '-n'." << endl;
    return 1;
  }
  Sudoku sudoku(final_num, game_path, game_num, difficulty, blank_num,
                need_unique);
  if (final_num != 0 && game_num == 0) {
    sudoku.generate_finales();
    cout << "Finales generation completed." << endl;
  } else if (game_num != 0) {
    sudoku.generate_games();
  } else if (game_path != nullptr) {
    sudoku.solve_games();
  }

  return 0;
}
