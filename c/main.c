#include<stdio.h>
#include "sudoku.h"
#include "trace.h"

int main(int argc, char** argv) {

    Sudoku* sudoku = sudoku_create();
    
    if (argc > 1) {
        char* input = argv[1];
        for (index_t i = 0; i < 81; i++) {
            DBG_VERBOSE("cell = %c, move = %d\n", input[i], input[i] - '0');
            move_t move = input[i] == '.' ? 0 : input[i] - '0';
            if (move < 0 || move > 9) {
                DBG_WARN("Skipping input cell: %d\n", move);
                continue;
            } else if (move != 0) {
                sudoku_move(sudoku, i, move);
            }
        }
    }
    
    Sudoku* solved = sudoku_solve(sudoku);
    sudoku_print(solved);
}
