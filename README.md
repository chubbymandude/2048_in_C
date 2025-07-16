# 2048_in_C
A terminal-based 2048 game built in C

Downloading: 
1. Download this repository into your computer.
2. Navigate to the associated folder.
3. Type "make all" in terminal to compile all files.
4. Type "./program" to run the game.

Playing: 
* Only compatible with WASD for now (may update in the future)
* The terminal will automatically accept each key input (no need to press enter).
* Each iteration of the game loop will print the game board, the user's score and the highest tile the user has obtained.
* The user can quit the game prematurely with Q and also restart the game with R.
* The ultimate goal is to reach 2048, but the game can continue until the user cannot make any more moves.

Implementation: 
* In order to randomly place tiles, a list implementation is used with each node holding a row-col element. RNG is used to randomly obtain a pair of row-col elements from the list to place a random tile on the board.
* A global array of directions is used in order to neatly move tiles all in one single method. Group moves of tiles for each direction are implemented in their own methods, and finally a method which updates all rows/cols in a specific direction is what's called in the game loop.
* In order to check if the game is done, the algorithm loops through each tile and checks if those tiles can be combined with any surrounding tiles. In order to prevent tiles from being combined because of this process, an extra parameter was added to the combine_tiles method to take this into consideration. Do note that this method won't run in its entirety unless the entire board is full.
