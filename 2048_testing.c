#include "2048.h"

void test_list();
void test_game_init();
void test_place_tile();
void test_move_one_tile();
void test_combine_tile();
void test_move_call_north();
void test_move_call_south();

/*
    This file is meant for testing
    specific functionality of the 
    2048 game implementation 
*/
int main(int argc, char **argv)
{
    test_move_call_south();
    return 0;
} 

// tests basic functionality of empty_list which is a core requirement for making 2048 work
void test_list()
{
    empty_list_t *list = empty_list_init();
    for(int i = 1; i <= 4; i++)
    {
        for(int j = 1; j <= 4; j++)
        {
            empty_list_add(list, i, j);
        }
    }
    int row;
    int col;
    for(int i = 0; i < 10; i++)
    {
        empty_list_get_random(list, &row, &col);
        printf("%d, %d\n", row, col);
    }
    empty_list_print(list);
    for(int i = 1; i <= 4; i++)
    {
        for(int j = 1; j <= 4; j++)
        {
            empty_list_remove(list, i, j);
        }
    }
    empty_list_print(list);
    empty_list_free(list);
}

// tests game initialization and free
void test_game_init()
{
    game_t *game = game_init();
    game_print(game, LOGGING);
    game_free(game);
}

void test_place_tile()
{
    srand(time(NULL)); // ensure that the random index is a random one from the list
    game_t *game = game_init();
    for(int i = 0; i < 16; i++)
    {
        place_random_tile(game);
    }
    game_print(game, NO_LOG);
    game_free(game);
}

void test_move_one_tile()
{
    game_t *game = game_init();
    game_print(game, LOGGING);
    game->board[1][1] = 2;
    game_print(game, LOGGING);
    move_tile(game, 1, 1, EAST);
    game_print(game, LOGGING);
    move_tile(game, 1, 4, SOUTH);
    game_print(game, LOGGING);
    move_tile(game, 4, 4, WEST);
    game_print(game, LOGGING);
    move_tile(game, 4, 1, NORTH);
    game_print(game, LOGGING);
    game_free(game);
}

void test_combine_tile()
{
    game_t *game = game_init();
    game_print(game, LOGGING);
    game->board[1][1] = 2;
    game->board[1][4] = 2;
    game_print(game, LOGGING);
    move_tile(game, 1, 1, EAST);
    game_print(game, LOGGING);
    game_free(game);
}

void test_move_call_north()
{
    game_t *game = game_init();
    game_print(game, LOGGING);
    game->board[4][1] = 2;
    game->board[2][2] = 2;
    game->board[4][2] = 2;
    game->board[1][3] = 2;
    game->board[2][3] = 2;
    game->board[3][3] = 2;
    game->board[1][4] = 2;
    game->board[2][4] = 2;
    game->board[3][4] = 2;
    game->board[4][4] = 2;
    game_print(game, LOGGING);
    move_all(game, NORTH);
    game_print(game, LOGGING);
    game_free(game);
}

void test_move_call_south()
{
    game_t *game = game_init();
    game_print(game, LOGGING);
    game->board[1][1] = 2;
    game->board[1][2] = 2;
    game->board[3][2] = 2;
    game->board[1][3] = 2;
    game->board[2][3] = 2;
    game->board[3][3] = 2;
    game->board[1][4] = 2;
    game->board[2][4] = 2;
    game->board[3][4] = 2;
    game->board[4][4] = 2;
    game_print(game, LOGGING);
    move_all(game, SOUTH);
    game_print(game, LOGGING);
    game_free(game);
}