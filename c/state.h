#ifndef STATE_H
#define STATE_H

#define ROW(i) (i / 9)
#define COL(i) (i % 9)
#define BOX(i) ((i / 27) * 3 + (i % 9) / 3)
#define MASK(move) (1 << (m - 1))


typedef unsigned char index_t;
typedef unsigned char move_t;
typedef unsigned short mask_t;

typedef struct _state {
    int solved;
    move_t rows[9];
    mask_t blocks[9];
    mask_t columns[9];
    mask_t grid[81];
} State;


State* state_create(void);

State* state_clone(const State*);

mask_t state_candidate(const State*, index_t);

State* state_move(State*, index_t, move_t);

State* state_solve(const State*);

void state_print(const State*);


#endif
