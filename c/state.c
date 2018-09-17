#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "state.h"
#include "trace.h"

State* state_create() {
    State* ptr = (State*)malloc(sizeof(State));
    if (ptr) {
        memset(ptr, 0, sizeof(State));
    }
    return ptr;
}

State* state_clone(const State* src) {
    State* copy = state_create();
    memcpy(copy, src, sizeof(State));
    return copy;
}

mask_t state_candidate(const State* state, index_t i) {
    if (state->grid[i]) {
        return 0; 
    }
    return ~(state->rows[ROW(i)] | state->columns[COL(i)] | state->blocks[BOX(i)]) & 0x1FF;
}

State* state_move(State* state, index_t i, move_t m) {
    if (!(state_candidate(state, i) & (1 << (m - 1)))) {
        printf("Invalid move");
        exit(1);
    }

    state->solved += 1;
    state->grid[i] = m;
    state->rows[ROW(i)] |= MASK(m);
    state->columns[COL(i)] |= MASK(m);
    state->blocks[BOX(i)] |= MASK(m);

    return state;
}

int numberOfSetBits(uint32_t i) {
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

State* state_solve(const State* state) {
    if (state->solved == 81) {
        return state_clone(state);
    }

    int min = 9;
    index_t next = 0;
    for (index_t z = 0; z < 81; z++) {
        int len = numberOfSetBits(state_candidate(state, z));
        DBG_VERBOSE("len = %d\n", len);
        if (len < min && !state->grid[z]) {
            next = z;
            min = len;
        }
    }
    if (!min) {
        return NULL;
    }
    DBG_INFO("min = %d, next = %d\n", min, next);

    for (index_t j = 0; j < 9; j++) {
        if (!(state_candidate(state, next) & (1 << j))) {
            continue;
        }
        State* clone = state_clone(state);
        DBG_INFO("index = %d, move = %d, solved = %d\n", next, j + 1, state->solved + 1);
        State* result = state_solve(state_move(clone, next, j + 1));
        if (result) {
            return result;
        }
        free(clone);
    }
    DBG_WARN("Dead end\n");
    return NULL;
}

void state_print(const State* state) {
    printf("\n");
    for (int i = 0; i < 27; i++) {
        move_t cell[3] = {
            state->grid[i * 3],
            state->grid[i * 3 + 1],
            state->grid[i * 3 + 2]
        };
        if (i != 0 && !(i % 9)) {
            printf("-------+-------+-------\n");
        }
        printf(" %d %d %d ", cell[0], cell[1], cell[2]);
        if ((i + 1) % 3) {
            printf("|");
        } else {
            printf("\n");
        }
    }
}
