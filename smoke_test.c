#include "gnect_engine.h"

#include <stdio.h>

static int fail(const char *message)
{
    fprintf(stderr, "smoke-test: %s\n", message);
    return 1;
}

int main(void)
{
    GnectGame game;
    int moves[GNECT_COLS];
    int col = -1;

    gnect_game_init(&game);

    if (game.turn != GNECT_RED || game.move_count != 0)
        return fail("initial state is wrong");

    if (gnect_collect_valid_moves(&game, moves) != GNECT_COLS)
        return fail("all columns should be valid initially");

    if (gnect_apply_move(&game, 3) != GNECT_RUNNING)
        return fail("failed to apply opening move");

    if (game.board[GNECT_ROWS - 1][3] != GNECT_RED || game.turn != GNECT_YELLOW)
        return fail("opening move landed incorrectly");

    gnect_ai_pick_move(&game, 2, &col);
    if (col < 0 || col >= GNECT_COLS || !gnect_is_valid_column(&game, col))
        return fail("AI did not choose a valid column");

    gnect_game_init(&game);
    gnect_apply_move(&game, 0);
    gnect_apply_move(&game, 1);
    gnect_apply_move(&game, 0);
    gnect_apply_move(&game, 1);
    gnect_apply_move(&game, 0);
    gnect_apply_move(&game, 1);
    if (gnect_apply_move(&game, 0) != GNECT_WIN || game.winner != GNECT_RED)
        return fail("vertical win was not detected");

    if (!gnect_undo(&game) || game.winner != GNECT_EMPTY || game.turn != GNECT_RED)
        return fail("undo did not restore pre-win state");

    puts("smoke-test: ok");
    return 0;
}
