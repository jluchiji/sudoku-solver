#include<stdio.h>
#include "state.h"
#include "trace.h"

int main(int argc, char** argv) {

    State* state = state_create();
    
    if (argc > 1) {
        char* input = argv[1];
        for (index_t i = 0; i < 81; i++) {
            DBG_VERBOSE("cell = %c, move = %d\n", input[i], input[i] - '0');
            move_t move = input[i] == '.' ? 0 : input[i] - '0';
            if (move < 0 || move > 9) {
                DBG_WARN("Skipping input cell: %d\n", move);
                continue;
            } else if (move != 0) {
                state_move(state, i, move);
            }
        }
    }
    
    State* solved = state_solve(state);
    state_print(solved);
}
