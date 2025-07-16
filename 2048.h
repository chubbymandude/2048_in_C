#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>

// board constants (both below 2, the lowest # tile)
#define EMPTY 0 
#define EDGE  1 

// board location constants
#define BOARD_LENGTH 6 // length of each side of the board
#define EDGE_START   0 // value where an edge can occur
#define EDGE_END     5 // another value where an edge can occur
#define START        1 // start edge for empty tiles
#define END          4 // end edge for empty tiles

// direction constants when used as parameters
#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

// constants defining game status
#define INIT    0
#define RUNNING 1
#define DONE    2

// levels for logging
#define NO_LOG  0
#define LOGGING 1

// constants for picking tiles
#define STANDARD 2
#define SPECIAL  4

// constants for checking if the game is over
#define NOT_CHECKING 0
#define CHECKING     1

// node struct used to create list
typedef struct list_node
{
    int row, col; // coordinates that the node holds
    struct list_node *next; // pointer to another node in the list
}
list_node_t;

// struct that mimics a list; primarily used to create a list of "empty" tiles
typedef struct
{
    list_node_t *front; // pointer to front of list for traversal
    int length; // amount of tiles in the list
}
empty_list_t;

// struct that holds the status of the game and other important features
typedef struct game
{
    int **board; // 4x4 board with surrounding edges
    int points; // points earned during the game
    int highest_tile; // value of the biggest # tile the user has obtained
    int game_status; // status of the game (constants defined above)
    empty_list_t *empty_list; // locations on the board that are not taken up by a tile 
}
game_t;

// FUNCTIONS FOR LIST (not involving game)
empty_list_t *empty_list_init(); // allocates a list to be used in game_t struct
void empty_list_free(empty_list_t *list); // frees list struct after allocation
void empty_list_add(empty_list_t *list, int row, int col); // adds set of row-col coordinates to the list
int empty_list_remove(empty_list_t *list, int row, int col); // removes set of row-col coordinates to the list
int empty_list_contains(empty_list_t *list, int row, int col); // checks if parameter row-col coordinates are in list
int empty_list_get_random(empty_list_t *list, int *rowP, int *colP); // gets random set of coordinates from the list
void empty_list_print(empty_list_t *list); // prints contents of list, for debugging

// FUNCTIONS FOR GAME INIT AND FREE
game_t *game_init(); // initializes game struct and fields, initializes empty list
int **board_init(); // initializes a board with either EMPTY or EDGE tiles
int is_edge(int row, int col); // checks if a location on the board is supposed to be an edge
void empty_list_add_all(game_t *game); // adds all spaces in the game to the game struct's empty list
void game_free(game_t *game); // frees game struct and all fields + frees list
void board_free(game_t *game); // frees board associated with game
void game_print(game_t *game, int logging); // prints contents of game in its current state

// FUNCTIONS FOR ADDING RANDOM TILES
int pick_random_tile(); // randomly picks between either 2 or 4 to add to the board
void place_random_tile(game_t *game); // places tile 2 or 4 at random place on the board that is empty

// FUNCTIONS FOR MOVING TILES 
int move_tile(game_t *game, int row, int col, int dir);
void move_col_north(game_t *game, int col); 
void move_col_south(game_t *game, int col);
void move_row_east(game_t *game, int row);
void move_row_west(game_t *game, int row);
void move_all(game_t *game, int dir); // moves all rows/cols in direction given in parameter

// FUNCTIONS FOR UPDATING BOARD
void swap_tiles(game_t *game, int row1, int col1, int row2, int col2); // swaps the board location of two tiles
int combine_tiles(game_t *game, int row1, int col1, int row2, int col2, int checking); // combines tiles, place in loc 2

// FUNCTIONS FOR CHECKING IF THE GAME HAS ENDED
int game_running(game_t *game); // checks if game is done by checking empty list status and surrounding tiles
int check_surrounding_tiles(game_t *game, int row, int col); // checks if surrounding tiles of this tile allow combine

// FUNCTIONS FOR SETTING CONDITIONS FOR PLAY WITHOUT NEEDING TO PRESS ENTER
void set_terminal(struct termios old, struct termios new);