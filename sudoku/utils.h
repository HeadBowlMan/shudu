
/*
*Copyright [2023] <Copyright shn yzx>
*/
#ifndef E__SHUDU_SUDOKU_UTILS_H_
#define E__SHUDU_SUDOKU_UTILS_H_

#include <utility>
#endif  // E__SHUDU_SUDOKU_UTILS_H_
void swap(int* a, int* b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void reverse(int* a, int* b) {
    while (a < b) {
        swap(a++, b--);
    }
}

bool permutation(int* start, int* end) {
    int* left = end;
    int* right, * rightest_larger;
    while (left != start) {
        right = left;
        left--;

        if (*left < *right) {
            rightest_larger = end;
            while (*rightest_larger <= *left) {
                rightest_larger--;
            }

            swap(left, rightest_larger);
            reverse(right, end);

            return true;
        }
    }

    reverse(start, end);
    return false;
}
