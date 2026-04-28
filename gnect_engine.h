#ifndef KINDLE_GNECT_ENGINE_H
#define KINDLE_GNECT_ENGINE_H

#include <glib.h>

#define GNECT_COLS 7
#define GNECT_ROWS 6
#define GNECT_MAX_MOVES (GNECT_COLS * GNECT_ROWS)

typedef enum {
    GNECT_EMPTY = 0,
    GNECT_RED = 1,
    GNECT_YELLOW = 2
} GnectPiece;

typedef enum {
    GNECT_RUNNING = 0,
    GNECT_WIN,
    GNECT_DRAW,
    GNECT_INVALID
} GnectMoveState;

typedef struct {
    GnectPiece board[GNECT_ROWS][GNECT_COLS];
    GnectPiece turn;
    GnectPiece winner;
    int move_count;
    int moves[GNECT_MAX_MOVES];
    GnectPiece history[GNECT_MAX_MOVES + 1][GNECT_ROWS][GNECT_COLS];
    GnectPiece turn_history[GNECT_MAX_MOVES + 1];
    GnectPiece winner_history[GNECT_MAX_MOVES + 1];
} GnectGame;

void gnect_game_init(GnectGame *game);
GnectPiece gnect_other_player(GnectPiece player);
gboolean gnect_is_valid_column(const GnectGame *game, int col);
int gnect_collect_valid_moves(const GnectGame *game, int moves[GNECT_COLS]);
GnectMoveState gnect_apply_move(GnectGame *game, int col);
gboolean gnect_undo(GnectGame *game);
gboolean gnect_is_full(const GnectGame *game);
int gnect_count_piece(const GnectGame *game, GnectPiece piece);
void gnect_ai_pick_move(const GnectGame *game, int level, int *out_col);

#endif
