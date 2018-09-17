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
 * This struct represents the sudoku of a Sudoku puzzle, along
 * with some cached constraints data to speed up solution
 */
typedef struct _sudoku {
    int solved;
    move_t rows[9];
    mask_t blocks[9];
    mask_t columns[9];
    mask_t grid[81];
} Sudoku;


/*
 * Allocates a new instance of Sudoku*
 * New instances are always zero-filled
 */
Sudoku* sudoku_create(void);

/*
 * Creates a deep clone of an existing Sudoku
 */
Sudoku* sudoku_clone(const Sudoku*);

/*
 * Given a Sudoku and a cell index, returns a bit mask
 * where 1 represents a viable candidate move
 */
mask_t sudoku_candidate(const Sudoku*, index_t);

/*
 * Determines the best cell to fill next
 */
index_t sudoku_next(const Sudoku*);

/*
 * Updates an existing Sudoku to fill in a cell
 * Also updates all cached constrains data
 */
Sudoku* sudoku_move(Sudoku*, index_t, move_t);

/*
 * Recursively solves a specific puzzle
 * Always returns a new instance; the original instance
 * remains unchanged
 */
Sudoku* sudoku_solve(const Sudoku*);

/*
 * Pretty-prints a puzzle to stdout
 */
void sudoku_print(const Sudoku*);


#endif
