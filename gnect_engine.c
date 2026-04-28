#include "gnect_engine.h"

#include <string.h>

static const int dirs[4][2] = {
    {1, 0}, {0, 1}, {1, 1}, {1, -1}
};

static void save_history(GnectGame *game)
{
    if (game->move_count > GNECT_MAX_MOVES)
        return;

    memcpy(game->history[game->move_count], game->board, sizeof(game->board));
    game->turn_history[game->move_count] = game->turn;
    game->winner_history[game->move_count] = game->winner;
}

static gboolean in_bounds(int col, int row)
{
    return col >= 0 && col < GNECT_COLS && row >= 0 && row < GNECT_ROWS;
}

static int find_drop_row(const GnectGame *game, int col)
{
    int row;

    if (col < 0 || col >= GNECT_COLS)
        return -1;

    for (row = GNECT_ROWS - 1; row >= 0; row--) {
        if (game->board[row][col] == GNECT_EMPTY)
            return row;
    }

    return -1;
}

GnectPiece gnect_other_player(GnectPiece player)
{
    return player == GNECT_RED ? GNECT_YELLOW : GNECT_RED;
}

void gnect_game_init(GnectGame *game)
{
    memset(game, 0, sizeof(*game));
    game->turn = GNECT_RED;
    save_history(game);
}

gboolean gnect_is_valid_column(const GnectGame *game, int col)
{
    return game->winner == GNECT_EMPTY && find_drop_row(game, col) >= 0;
}

int gnect_collect_valid_moves(const GnectGame *game, int moves[GNECT_COLS])
{
    int count = 0;
    int col;

    for (col = 0; col < GNECT_COLS; col++) {
        if (gnect_is_valid_column(game, col))
            moves[count++] = col;
    }

    return count;
}

gboolean gnect_is_full(const GnectGame *game)
{
    int col;

    for (col = 0; col < GNECT_COLS; col++) {
        if (game->board[0][col] == GNECT_EMPTY)
            return FALSE;
    }

    return TRUE;
}

int gnect_count_piece(const GnectGame *game, GnectPiece piece)
{
    int count = 0;
    int row;
    int col;

    for (row = 0; row < GNECT_ROWS; row++) {
        for (col = 0; col < GNECT_COLS; col++) {
            if (game->board[row][col] == piece)
                count++;
        }
    }

    return count;
}

static gboolean has_four_from(const GnectGame *game, int col, int row, GnectPiece piece)
{
    int d;

    for (d = 0; d < 4; d++) {
        int dx = dirs[d][0];
        int dy = dirs[d][1];
        int count = 1;
        int step;

        for (step = 1; step < 4; step++) {
            int c = col + dx * step;
            int r = row + dy * step;
            if (!in_bounds(c, r) || game->board[r][c] != piece)
                break;
            count++;
        }

        for (step = 1; step < 4; step++) {
            int c = col - dx * step;
            int r = row - dy * step;
            if (!in_bounds(c, r) || game->board[r][c] != piece)
                break;
            count++;
        }

        if (count >= 4)
            return TRUE;
    }

    return FALSE;
}

GnectMoveState gnect_apply_move(GnectGame *game, int col)
{
    int row = find_drop_row(game, col);
    GnectPiece player = game->turn;

    if (row < 0 || game->winner != GNECT_EMPTY)
        return GNECT_INVALID;

    game->board[row][col] = player;
    if (game->move_count < GNECT_MAX_MOVES)
        game->moves[game->move_count] = col;
    game->move_count++;

    if (has_four_from(game, col, row, player)) {
        game->winner = player;
        save_history(game);
        return GNECT_WIN;
    }

    if (gnect_is_full(game)) {
        save_history(game);
        return GNECT_DRAW;
    }

    game->turn = gnect_other_player(player);
    save_history(game);
    return GNECT_RUNNING;
}

gboolean gnect_undo(GnectGame *game)
{
    if (game->move_count <= 0)
        return FALSE;

    game->move_count--;
    memcpy(game->board, game->history[game->move_count], sizeof(game->board));
    game->turn = game->turn_history[game->move_count];
    game->winner = game->winner_history[game->move_count];
    game->moves[game->move_count] = 0;
    return TRUE;
}

static int score_window(GnectPiece cells[4], GnectPiece player)
{
    GnectPiece other = gnect_other_player(player);
    int mine = 0;
    int theirs = 0;
    int empty = 0;
    int i;

    for (i = 0; i < 4; i++) {
        if (cells[i] == player)
            mine++;
        else if (cells[i] == other)
            theirs++;
        else
            empty++;
    }

    if (mine == 4)
        return 100000;
    if (theirs == 4)
        return -100000;
    if (mine == 3 && empty == 1)
        return 80;
    if (mine == 2 && empty == 2)
        return 12;
    if (theirs == 3 && empty == 1)
        return -100;
    if (theirs == 2 && empty == 2)
        return -10;
    return 0;
}

static int evaluate_position(const GnectGame *game, GnectPiece player)
{
    int score = 0;
    int row;
    int col;
    int d;

    for (row = 0; row < GNECT_ROWS; row++) {
        if (game->board[row][GNECT_COLS / 2] == player)
            score += 6;
    }

    for (row = 0; row < GNECT_ROWS; row++) {
        for (col = 0; col < GNECT_COLS; col++) {
            for (d = 0; d < 4; d++) {
                GnectPiece cells[4];
                int dx = dirs[d][0];
                int dy = dirs[d][1];
                int i;

                for (i = 0; i < 4; i++) {
                    int c = col + dx * i;
                    int r = row + dy * i;
                    if (!in_bounds(c, r))
                        break;
                    cells[i] = game->board[r][c];
                }
                if (i == 4)
                    score += score_window(cells, player);
            }
        }
    }

    return score;
}

static int score_move(const GnectGame *game, int col, int level)
{
    GnectGame copy = *game;
    GnectPiece player = game->turn;
    int score;
    int replies[GNECT_COLS];
    int reply_count;
    int i;

    if (gnect_apply_move(&copy, col) == GNECT_WIN)
        return 1000000;

    score = evaluate_position(&copy, player);
    if (level <= 1)
        score += g_random_int_range(0, 8);

    if (level >= 1) {
        copy.turn = gnect_other_player(player);
        reply_count = gnect_collect_valid_moves(&copy, replies);
        for (i = 0; i < reply_count; i++) {
            GnectGame reply = copy;
            if (gnect_apply_move(&reply, replies[i]) == GNECT_WIN)
                score -= 900000;
        }
    }

    if (level >= 2) {
        int best_reply = -G_MAXINT;
        copy.turn = gnect_other_player(player);
        reply_count = gnect_collect_valid_moves(&copy, replies);
        for (i = 0; i < reply_count; i++) {
            GnectGame reply = copy;
            gnect_apply_move(&reply, replies[i]);
            best_reply = MAX(best_reply, evaluate_position(&reply, gnect_other_player(player)));
        }
        if (best_reply != -G_MAXINT)
            score -= best_reply / 2;
    }

    return score;
}

void gnect_ai_pick_move(const GnectGame *game, int level, int *out_col)
{
    int moves[GNECT_COLS];
    int count = gnect_collect_valid_moves(game, moves);
    int best_score = -G_MAXINT;
    int best_index = 0;
    int i;

    *out_col = -1;
    if (count <= 0)
        return;

    if (level <= 0 && g_random_int_range(0, 3) == 0) {
        *out_col = moves[g_random_int_range(0, count)];
        return;
    }

    for (i = 0; i < count; i++) {
        int score = score_move(game, moves[i], level);
        score -= ABS(moves[i] - (GNECT_COLS / 2));
        if (score > best_score) {
            best_score = score;
            best_index = i;
        }
    }

    *out_col = moves[best_index];
}
