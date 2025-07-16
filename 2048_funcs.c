#include "2048.h"

// array of states that can be obtained when printing game
char *status_arr[3] = {"INIT", "RUNNING", "DONE"};

// arrays that determine direction to move tile (NORTH, SOUTH, EAST, WEST)
int dir_x[4] = {0, 0, 1, -1};
int dir_y[4] = {-1, 1, 0, 0};

////////////////////////
// FUNCTIONS FOR LIST // 
////////////////////////

// allocates space for an empty_list with 0 nodes
empty_list_t *empty_list_init()
{
    empty_list_t *empty_list = malloc(sizeof(empty_list_t));
    empty_list->front = NULL;
    empty_list->length = 0;
    return empty_list;
}

// frees space allocated by empty_list and all nodes in it
void empty_list_free(empty_list_t *list)
{
    list_node_t *curr = list->front;
    while(curr != NULL)
    {
        list_node_t *deallocate = curr;
        curr = curr->next;
        free(deallocate);
    }
    free(list);
}

/*
    Adds given row-col elements on board to the empty_list
    Indicates that this spot has no tiles
    List add is done at the front
*/
void empty_list_add(empty_list_t *list, int row, int col)
{
    list_node_t *new_node = malloc(sizeof(list_node_t));
    new_node->row = row;
    new_node->col = col;
    if(list->length == 0) // special case, no elements in list
    {
        new_node->next = NULL;
    }
    else
    {
        new_node->next = list->front;
    }
    list->front = new_node;
    list->length++;
}

/*
    Removes element with given row-col elements from the empty list
    uses a list traversal in order to find the element

    If the row-col elements are found return 1. if not found return 0
*/
int empty_list_remove(empty_list_t *list, int row, int col)
{
    list_node_t *curr = list->front;
    list_node_t *prev = NULL;
    // loop until element with row-col coordinates is found or list traversal is complete
    while(curr != NULL)
    {
        if(curr->row == row && curr->col == col) // removal now occurs
        {
            list_node_t *deallocate = curr;
            if(curr == list->front) // special case, move front
            {
                list->front = list->front->next;
            }
            else
            {
                prev->next = curr->next;
            }
            free(deallocate);
            list->length--;
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0; // row-col elements were not found
}

/*
    Checks if a given row-col coordinates are in the empty_list

    Returns 1 if in the list, if not found returns 0
*/
int empty_list_contains(empty_list_t *list, int row, int col)
{
    list_node_t *curr = list->front;
    while(curr != NULL)
    {
        if(curr->row == row && curr->col == col)
        {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/*
    Gets a random set of row-col coordinates from empty_list and assigns them to parameter row/col 
    If list is empty this method cannot work and returns 0 for failure, otherwise return 1 for success
*/
int empty_list_get_random(empty_list_t *list, int *rowP, int *colP)
{
    if(list->length == 0) // cannot get random coordinates
    {
        return 0;
    }
    int rand_index = rand() % list->length; // generate random index
    // loop until element at index is found
    list_node_t *curr = list->front;
    int index = 0;
    while(index != rand_index)
    {
        curr = curr->next;
        index++;
    }
    *rowP = curr->row;
    *colP = curr->col;
    return 1; // random coordinates successfully found
}

// prints all contents of the empty_list for debugging
void empty_list_print(empty_list_t *list)
{
    printf("List Length: %d\n", list->length);
    printf("INDEX ROW COL\n");
    list_node_t *curr = list->front;
    int index = 0;
    while(curr != NULL)
    {
        printf("%3d    %d   %d\n", index, curr->row, curr->col);
        curr = curr->next;
        index++;
    }
}

////////////////////////////////////
// FUNCTIONS FOR GAME INIT + FREE //
////////////////////////////////////

/*
    Allocates space for game struct 
    Initializes fields of game struct
    Allocates space for board and initializes its values
    Allocates space for empty list and adds all the empty spaces to it
*/
game_t *game_init()
{
    game_t *game = malloc(sizeof(game_t));
    game->points = 0;
    game->highest_tile = 0;
    game->game_status = INIT;
    game->board = board_init(game);
    game->empty_list = empty_list_init();
    empty_list_add_all(game);
    return game;
}

// initializes the board associated with the game struct with EDGE & EMPTY spaces, returns the board at the end
int **board_init()
{
    int **board = malloc(sizeof(int) * BOARD_LENGTH * BOARD_LENGTH); // allocate space for rows
    for(int row = 0; row < BOARD_LENGTH; row++)
    {
        // allocate space for each set of columns
        board[row] = malloc(sizeof(int) * BOARD_LENGTH);
        for(int col = 0; col < BOARD_LENGTH; col++)
        {
            if(is_edge(row, col))
            {
                board[row][col] = EDGE;
            }
            else
            {
                board[row][col] = EMPTY;
            }
        }
    }
    return board;
}

// adds all empty coordinates in the game struct to the game struct's list (upon initialization)
void empty_list_add_all(game_t *game)
{
    for(int row = START; row <= END; row++)
    {
        for(int col = START; col <= END; col++)
        {
            empty_list_add(game->empty_list, row, col);
        }
    }
}

/*
    An edge is any location on the board
    that is on the sides of the 2D array board

    returns 1 if row-col is edge, otherwise -
*/
int is_edge(int row, int col)
{
    if(row == EDGE_START || col == EDGE_START || row == EDGE_END || col == EDGE_END)
    {
        return 1;
    }
    return 0;
}

// frees all content of game struct, including board and empty_list
void game_free(game_t *game)
{
    board_free(game);
    empty_list_free(game->empty_list);
    free(game);
}

// frees all content of the board associated with the games; first all column sets and then the rows
void board_free(game_t *game)
{
    for(int row = 0; row < BOARD_LENGTH; row++)
    {
        free(game->board[row]);
    }
    free(game->board);
}

// prints game content (some content is only printed depending on parameter "logging")
void game_print(game_t *game, int logging)
{
    // print game board, only print EDGES when logging
    printf("\n");
    for(int row = START; row <= END; row++)
    {
        for(int col = START; col <= END; col++)
        {
            printf("%6d", game->board[row][col]);
        }
        printf("\n");
    }
    printf("\n");
    printf("points: %d\n", game->points);
    printf("highest_tile: %d\n", game->highest_tile);
    if(logging >= LOGGING)
    {
        printf("game status: %s\n", status_arr[game->game_status]);
        empty_list_print(game->empty_list);
    }
}

////////////////////////////////
// FUNCTIONS FOR RANDOM TILE //
//////////////////////////////

// picks either a tile of value 2 or 4 to place on the board (90% for 2, 10% for 4)
int pick_random_tile()
{
    int rand_index = rand() % 10;
    if(rand_index == SPECIAL)
    {
        return SPECIAL;
    }
    return STANDARD;
}

void place_random_tile(game_t *game)
{
    //Get a pair of row-col elements from the empty_list, remove from empty list
    int row, col;
    empty_list_get_random(game->empty_list, &row, &col);
    empty_list_remove(game->empty_list, row, col);
    //Add to the board
    int rand_tile = pick_random_tile();
    game->board[row][col] = rand_tile;
    //Highest tile should be updated if the random tile is higher than the current highest
    if(rand_tile > game->highest_tile)
    {
        game->highest_tile = rand_tile;
    }
}  

////////////////////////////////
// FUNCTIONS FOR MOVING TILES //
////////////////////////////////

/*
    Moves the tile at game->board[row][col] in the direction specified.

    Moves the tile until it either reaches another tile or reaches the edge.
    If it reaches another tile, tries to combine with it 
*/
int move_tile(game_t *game, int row, int col, int dir)
{
    // if no tile at this location then do no run this method; exit code 0
    if(game->board[row][col] == EMPTY)
    {
        return 0;
    }
    // loop until reach edge or another tile
    while(game->board[row + dir_y[dir]][col + dir_x[dir]] == EMPTY)
    {
        swap_tiles(game, row, col, row + dir_y[dir], col + dir_x[dir]);
        row += dir_y[dir];
        col += dir_x[dir];
    }
    // try to combine with adjacent tile if possible
    combine_tiles(game, row, col, row + dir_y[dir], col + dir_x[dir], NOT_CHECKING);
    return 1;
}

// moves all tiles in one column in the north direction
void move_col_north(game_t *game, int col)
{
    // traverse from top to bottom
    for(int row = START; row <= END; row++)
    {
        move_tile(game, row, col, NORTH);
    }
}

// moves all tiles in one column in the south direction
void move_col_south(game_t *game, int col)
{
    // traverse from bottom to top
    for(int row = END; row >= START; row--)
    {
        move_tile(game, row, col, SOUTH);
    }
}

// moves all tiles in one row in the east direction
void move_row_east(game_t *game, int row)
{
    // traverse from right to left
    for(int col = END; col >= START; col--)
    {
        move_tile(game, row, col, EAST);
    }
}

// moves all tiles in one row in the west direction
void move_row_west(game_t *game, int row)
{
    // traverse from left to right
    for(int col = START; col <= END; col++)
    {
        move_tile(game, row, col, WEST);
    }
}

// moves all rows/columns depending on direction given
void move_all(game_t *game, int dir)
{
    for(int index = START; index <= END; index++)
    {
        // determine which method to call using a switch statement
        switch(dir)
        {
            case NORTH:
                move_col_north(game, index);
                break;
            case SOUTH:
                move_col_south(game, index);
                break;
            case EAST:
                move_row_east(game, index);
                break;
            case WEST:
                move_row_west(game, index);
                break;
        }
    }
}

////////////////////////////////////////
// FUNCTIONS FOR UPDATING BOARD TILES //
////////////////////////////////////////

// swaps tile at row1, col1 to tile at row2, col2
void swap_tiles(game_t *game, int row1, int col1, int row2, int col2)
{
    int temp = game->board[row1][col1];
    game->board[row1][col1] = game->board[row2][col2];
    game->board[row2][col2] = temp;
    empty_list_add(game->empty_list, row1, col1);
    empty_list_remove(game->empty_list, row2, col2);
}

/*
    Combine tile at row1/col1 with row2/col2, store in row2/col2
    If checking surrounding tiles, parameter checking is non-zero
    
    Returns 0 if tiles cannot be combined, returns 1 for successful combine
*/
int combine_tiles(game_t *game, int row1, int col1, int row2, int col2, int checking)
{
    if(game->board[row1][col1] != game->board[row2][col2]) // cannot combine
    {
        return 0;
    }
    if(!checking) // only run this when playing game, not when checking if game is over
    {
        // update tiles
        game->board[row1][col1] = 0;
        game->board[row2][col2] *= 2;
        // update empty_list
        empty_list_add(game->empty_list, row1, col1);
        // update game points & highest tile
        game->points += game->board[row2][col2];
        if(game->board[row2][col2] > game->highest_tile)
        {
            game->highest_tile = game->board[row2][col2];
        }
    }
    return 1;
}

// checks if the game is still running by seeing if any tiles on the board can be combined with any adjacent tiles 
int game_running(game_t *game)
{
    if(game->empty_list->length != 0) // if there are empty tiles game is still running
    {
        return 1;
    }
    // loop through each row-col coordinate
    for(int row = START; row <= END; row++)
    {
        for(int col = START; col <= END; col++)
        {
            // if non-zero that means combine_tiles succeeded for an adjacent tile
            if(check_surrounding_tiles(game, row, col)) 
            {
                return 1; // game is not done
            }
        }
    }
    game->game_status = DONE;
    return 0; // game is done
}

// checks all 4 surrounding tiles to this one and sees if any of them can be combined 
int check_surrounding_tiles(game_t *game, int row, int col)
{
    int total = 0; // if this remains 0 that means none of the surrounding tiles could be combined
    total += combine_tiles(game, row, col, row + dir_y[NORTH], col, CHECKING); // tile above
    total += combine_tiles(game, row, col, row + dir_y[SOUTH], col, CHECKING); // tile below
    total += combine_tiles(game, row, col, row, col + dir_x[EAST], CHECKING); // tile to the right
    total += combine_tiles(game, row, col, row, col + dir_x[WEST], CHECKING); // tile to the left
    return total;
}

/////////////////////////////////////
// FUNCTIONS FOR SETTING TERMINAL //
///////////////////////////////////

// sets the terminal so user can sent input without needing to press "enter"
void set_terminal(struct termios old, struct termios new)
{
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= (~ICANON & ~ECHO);
    new.c_cc[VMIN] = 1; // one character allowed per loop
    new.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &new);
}