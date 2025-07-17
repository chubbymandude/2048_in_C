#include "2048.h"

/*
    This main method provides the game 
    loop in order to play a 
    terminal-based implementation of the
    2048-game.

    The game is implemented in methods written
    in the "2048_funcs.c" file, with the header
    file being located in "2048.h"
*/
int main(int argc, char **argv)
{
    // how-to-play
    printf("\nW --> Move Tiles Up\n");
    printf("A --> Move Tiles Left\n");
    printf("S --> Move Tiles Down\n");
    printf("D --> Move Tiles Right\n");
    printf("Q --> Quit\n");
    printf("R --> Restart\n");

    // set terminal & rand generator
    struct termios old, new;
    set_terminal(old, new);
    srand(time(NULL));

GAME_INIT: 
    game_t *game = game_init();
    place_random_tile(game); // place an initial random tile

    while(game_running(game)) // run until the game cannot continue
    {
        game->game_status = RUNNING;
        place_random_tile(game);
        game_print(game, NO_LOG); 
        printf("\n");

GET_INPUT: 
        int move = getchar(); // obtain move from player
        if(tolower(move) == 'w') // move up
        {
            move_all(game, NORTH);
        }
        else if(tolower(move) == 'a') // move left
        {
            move_all(game, WEST);
        }
        else if(tolower(move) == 's') // move down
        {
            move_all(game, SOUTH);
        }
        else if(tolower(move) == 'd') // move right
        {
            move_all(game, EAST);
        }
        else if(tolower(move) == 'q')
        {
            break;
        }
        else if(tolower(move) == 'r')
        {
            game_free(game);
            printf("\nGame restarting...\n");
            goto GAME_INIT; // explicit jump to reinitialize the game
        }
        else
        {
            printf("Sorry, your input was invalid, try again...\n");
            goto GET_INPUT; // explicit jump to get user to enter input again
        }

        // if the user reaches 2048 print a special message
        if(game->highest_tile == 2048)
        {
            printf("You've won!\n");
            printf("You've reached the tile 2048!\n");
            printf("Press Q to quit, any other character to continue: \n");
            int quit = getchar();
            if(tolower(quit) == 'q')
            {
                break;
            }
        }
    }
    // print final stats
    printf("The game is done!\n");
    printf("Your stats this round...\n");
    game_print(game, NO_LOG);
    game_free(game);
}