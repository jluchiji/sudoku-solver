#ifndef STATE_H
#define STATE_H

#define ROW(i) (i / 9)
#define COL(i) (i % 9)
#define BOX(i) ((i / 27) * 3 + (i % 9) / 3)
#define MASK(move) (1 << (m - 1))


typedef unsigned char index_t;
typedef unsigned char move_t;
typedef unsigned short mask_t;

/*
 * This struct represents the state of a Sudoku puzzle, along
 * with some cached constraints data to speed up solution
 */
typedef struct _state {
    int solved;
    move_t rows[9];
    mask_t blocks[9];
    mask_t columns[9];
    mask_t grid[81];
} State;


/*
 * Allocates a new instance of State*
 * New instances are always zero-filled
 */
State* state_create(void);

/*
 * Creates a deep clone of an existing State
 */
State* state_clone(const State*);

/*
 * Given a State and a cell index, returns a bit mask
 * where 1 represents a viable candidate move
 */
mask_t state_candidate(const State*, index_t);

/*
 * Determines the best cell to fill next
 */
index_t state_next(const State*);

/*
 * Updates an existing State to fill in a cell
 * Also updates all cached constrains data
 */
State* state_move(State*, index_t, move_t);

/*
 * Recursively solves a specific puzzle
 * Always returns a new instance; the original instance
 * remains unchanged
 */
State* state_solve(const State*);

/*
 * Pretty-prints a puzzle to stdout
 */
void state_print(const State*);


#endif
