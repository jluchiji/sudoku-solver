#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include "sudoku.h"
#include "trace.h"

int _count_bits(uint32_t i) {
     i = i - ((i >> 1) & 0x55555555);
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
     return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

Sudoku* sudoku_create() {
    Sudoku* ptr = (Sudoku*)malloc(sizeof(Sudoku));
    if (ptr) {
        memset(ptr, 0, sizeof(Sudoku));
    }
    return ptr;
}

Sudoku* sudoku_clone(const Sudoku* src) {
    Sudoku* copy = sudoku_create();
    memcpy(copy, src, sizeof(Sudoku));
    return copy;
}

mask_t sudoku_candidate(const Sudoku* sudoku, index_t i) {
    if (sudoku->grid[i]) {
        return 0; 
    }
    return ~(sudoku->rows[ROW(i)] | sudoku->columns[COL(i)] | sudoku->blocks[BOX(i)]) & 0x1FF;
}

index_t sudoku_next(const Sudoku* sudoku) {
    int min = 9;
    index_t next = 0;
    for (index_t z = 0; z < 81; z++) {
        int len = _count_bits(sudoku_candidate(sudoku, z));
        DBG_VERBOSE("len = %d\n", len);
        if (len < min && !sudoku->grid[z]) {
            next = z;
            min = len;
        }
    }
    DBG_INFO("min = %d, next = %d\n", min, next);
    return next;
}

Sudoku* sudoku_move(Sudoku* sudoku, index_t i, move_t m) {
    if (!(sudoku_candidate(sudoku, i) & (1 << (m - 1)))) {
        printf("Invalid move");
        exit(1);
    }

    sudoku->solved += 1;
    sudoku->grid[i] = m;
    sudoku->rows[ROW(i)] |= MASK(m);
    sudoku->columns[COL(i)] |= MASK(m);
    sudoku->blocks[BOX(i)] |= MASK(m);

    return sudoku;
}

Sudoku* sudoku_solve(const Sudoku* sudoku) {
    if (sudoku->solved == 81) {
        return sudoku_clone(sudoku);
    }

    index_t next = sudoku_next(sudoku);
    if (!_count_bits(sudoku_candidate(sudoku, next))) {
        return NULL;
    }

    for (index_t j = 0; j < 9; j++) {
        if (!(sudoku_candidate(sudoku, next) & (1 << j))) {
            continue;
        }
        Sudoku* clone = sudoku_clone(sudoku);
        DBG_INFO("index = %d, move = %d, solved = %d\n", next, j + 1, sudoku->solved + 1);
        Sudoku* result = sudoku_solve(sudoku_move(clone, next, j + 1));
        if (result) {
            return result;
        }
        free(clone);
    }
    DBG_WARN("Dead end\n");
    return NULL;
}

void sudoku_print(const Sudoku* sudoku) {
    printf("\n");
    for (int i = 0; i < 27; i++) {
        move_t cell[3] = {
            sudoku->grid[i * 3],
            sudoku->grid[i * 3 + 1],
            sudoku->grid[i * 3 + 2]
        };
        if (i != 0 && !(i % 9)) {
            printf("-------+-------+-------\n");
        }
        printf(" %d %d %d %s", cell[0], cell[1], cell[2], (i + 1) % 3 ? "|" : "\n");
    }
}
