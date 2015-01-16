/*****************************************************************************\
*                                                                             *
* Author:               Jake Crane                                            *
* UserID:               JAC86520                                              *
* Date:                 10/14/2012                                            *
* Project ID:           Minesweeper                                           *
* CS class:             CS 2500                                               *
* Problem Description: This program allows the user to play Minesweeper.      *
*                                                                             *
*                                                                             *
\*****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_ROW 100
#define MAX_COL 100
#define GAME_EMPTY_CELL 0
#define GAME_MINE -1
#define GAME_BOUNDING_CELL -2
#define DISPLAY_MINE 15
#define DISPLAY_HIDDEN_CELL 0
#define DISPLAY_BLANK_CELL 11

//prototype for revealSurroundingCells
void revealSurroundingCells(int r, int c, int m, int n, unsigned char display[][MAX_COL],
		char game[][MAX_COL], int *num_of_hidden_cells, char checkedCells[][MAX_COL]);

//This function returns the number of mines to be placed given rows, columns and difficulty.
int getNumberOfMines(int row, int column, char difficulty) {
	int numberOfcells = row * column;
	return (int) ((numberOfcells * difficulty * 0.05) + .5);
}

//This function returns a sudo random number within the min and max input.
int getRandomNumber(int min, int max) {
	return (rand() % (max - min + 1) + min);
}

//This function sets the specified cell value of game equal to the number of surrounding mines.
//The function skips the cell if it is a bounding cell or if the cell contains a mine.
void addMineCount(char game[][MAX_COL], int row, int column) {
	if (game[row][column] != GAME_MINE && game[row][column] != GAME_BOUNDING_CELL) {
		game[row][column]++;
	}
}

//This function places the mines in game and calls addMineCount.
void placeMines(char game[][MAX_COL], int rows, int columns, int numberOfMines) {
	int numberOfMinesToPlace = numberOfMines;

	do {
		int randomRow = getRandomNumber(1, rows);
		int randomColumn = getRandomNumber(1, columns);
		if (game[randomRow][randomColumn] != GAME_MINE) {

			game[randomRow][randomColumn] = GAME_MINE;
			numberOfMinesToPlace--;

			addMineCount(game, randomRow - 1, randomColumn - 1); //upper-left - -

			addMineCount(game, randomRow - 1, randomColumn + 1); //upper-right - +

			addMineCount(game, randomRow + 1, randomColumn - 1); //lower left + -

			addMineCount(game, randomRow + 1, randomColumn + 1); //lower-right + +

			addMineCount(game, randomRow, randomColumn - 1); //left 0 -

			addMineCount(game, randomRow, randomColumn + 1); //right 0 +

			addMineCount(game, randomRow - 1, randomColumn); //above - 0

			addMineCount(game, randomRow + 1, randomColumn); //below + 0
		}
	} while(numberOfMinesToPlace > 0);

}

//This function prints dislpay.
void printBoard(unsigned char display[][MAX_COL], int rows, int columns) {
	printf("   ");
	for (int i = 1; i < (columns + 1); i++) {
			if (i < 10) {
				printf(" %d ", i);
			} else {
				printf("%d ", i);
			}
	}

	printf("\n");
	for (int i = 0; i < rows; i++) {
		if ((i + 1) < 10) {
			printf("%d   ", i + 1);
		} else {
			printf("%d  ", i + 1);
		}
		for (int j = 0; j < columns; j++) {
			if (display[i][j] == DISPLAY_HIDDEN_CELL) {
				printf("*  ");
			} else if (display[i][j] == DISPLAY_MINE) {
				printf("M  ");
			} else if (display[i][j] == DISPLAY_BLANK_CELL) {
				printf("   ");
			} else {
				printf("%hhu  ", display[i][j]);
			}
		}
		printf("\n");
	}
}

//This function returns 1 if the given row and column are within the defined number of rows and columns.
//Otherwise it returns 0;
int inputIsValid(int r, int c, int rows, int columns) {
	if (r > -1 && c > -1 && r <= rows && c <= columns) {
		return 1;
	}
	return 0;
}

//This function adds the mine display value to display.
void addMinesToDisplay(unsigned char display[][MAX_COL], char game[][MAX_COL], int rows, int columns) {
	for (int i = 1; i <= rows; i++) {
		for (int j = 1; j <= columns; j++) {
			if (game[i][j] == GAME_MINE) {
				display[i - 1][j - 1] = DISPLAY_MINE;
			}
		}
	}
}

//This function sets dislpay's corresponding cell to a space if the given game cell is GAME_EMPTY_CELL
//or to the number of surrounding mines if the given game cell contains the number of surrounding mines.
void checkAndReveal(int gameRow, int gameColumn, int m, int n, unsigned char display[][MAX_COL],
		char game[][MAX_COL], int *num_of_hidden_cells, char checkedCells[][MAX_COL]) {

	if (game[gameRow][gameColumn] == GAME_BOUNDING_CELL) {
		return;
	} else if (game[gameRow][gameColumn] == GAME_EMPTY_CELL) {
		display[gameRow - 1][gameColumn - 1] = DISPLAY_BLANK_CELL;
		(*num_of_hidden_cells)--;
		revealSurroundingCells(gameRow, gameColumn, m, n, display, game, num_of_hidden_cells, checkedCells);
	} else if (game[gameRow][gameColumn] != GAME_MINE) { //game is number representing how many mines are surrounding
		display[gameRow - 1][gameColumn - 1] = game[gameRow][gameColumn];
	}

}

//This function will reveal the given cell's surrounding empty cells and cells containing a surrounding mine count.
void revealSurroundingCells(int gameRow, int gameColumn, int m, int n, unsigned char display[][MAX_COL],
		char game[][MAX_COL], int *num_of_hidden_cells, char checkedCells[][MAX_COL]) {

	if (game[gameRow][gameColumn] == GAME_BOUNDING_CELL || checkedCells[gameRow][gameColumn] == 1) {
		return;
	}

	checkedCells[gameRow][gameColumn] = 1;

	checkAndReveal(gameRow - 1, gameColumn - 1, m, n, display, game, num_of_hidden_cells, checkedCells); //upper left - -

	checkAndReveal(gameRow - 1, gameColumn + 1, m, n, display, game, num_of_hidden_cells, checkedCells); //upper right - +

	checkAndReveal(gameRow + 1, gameColumn - 1, m, n, display, game, num_of_hidden_cells, checkedCells); //lower left + -

	checkAndReveal(gameRow + 1, gameColumn + 1, m, n, display, game, num_of_hidden_cells, checkedCells); //lower right + +

	checkAndReveal(gameRow, gameColumn - 1, m, n, display, game, num_of_hidden_cells, checkedCells); //left 0 -

	checkAndReveal(gameRow, gameColumn + 1, m, n, display, game, num_of_hidden_cells, checkedCells); //right 0 +

	checkAndReveal(gameRow - 1, gameColumn, m, n, display, game, num_of_hidden_cells, checkedCells); //above - 0

	checkAndReveal(gameRow + 1, gameColumn, m, n, display, game, num_of_hidden_cells, checkedCells); //below + 0

}

//This function will execute the user's chosen move.
void move(int gameRow, int gameColumn, int m, int n, unsigned char display[][MAX_COL],
		char game[MAX_ROW][MAX_COL], unsigned char *hit_a_mine, int *num_of_hidden_cells) {

	switch (game[gameRow][gameColumn]) {
	case GAME_MINE:
		*hit_a_mine = 1;
		break;
	case GAME_EMPTY_CELL:
		display[gameRow - 1][gameColumn - 1] = DISPLAY_BLANK_CELL;
		(*num_of_hidden_cells)--;
		char checkedCells[MAX_ROW][MAX_COL];
		memset(checkedCells, 0, sizeof(checkedCells));
		revealSurroundingCells(gameRow, gameColumn, m, n, display, game, num_of_hidden_cells, checkedCells);
		break;
	default: //cell contains number
		display[gameRow - 1][gameColumn - 1] = game[gameRow][gameColumn];
		(*num_of_hidden_cells)--;
		break;
	}

}

//This function sets the cells bounding game equal to GAME_BOUNDING_CELL.
void setBoundingCells(char game[MAX_ROW][MAX_COL], int boundingNumber, int rows, int columns) {

	memset(game, GAME_BOUNDING_CELL, (columns + 2)); //top line

	memset(game[rows + 1], GAME_BOUNDING_CELL, (columns + 2)); //bottom line

	//sides
	for (int i = 1; i < (rows + 1); i++) {
		for (int j = 0; j <= (columns+ 1); j += columns + 1) {
			game[i][j] = GAME_BOUNDING_CELL;
		}
	}

}

void main(int argc, char **argv) {
	char game[MAX_ROW][MAX_COL]; //The game board
	unsigned char display[MAX_ROW][MAX_COL]; //The display board
	int m, n; // actual number of rows and columns entered from the command line arguments
	unsigned char difficulty = 1; // difficulty level entered from the command line argument
	int r, c; //player Selected row and column indexes
	unsigned char hit_a_mine; // game not over
	int num_of_mines; // The number of mines placed in game
	int num_of_hidden_cells; // The number of hidden cells
	char playAgain[12]; // String used to store whether or not the user wants to play again.

	srand ((long) time(NULL) ); // Seeds the random number generator

	setbuf(stdin, NULL); // Disable stdin buffer

	setbuf(stdout, NULL); // Disable stdout buffer

	if (argc != 4) {
		printf("Usage: %s row col diff\n", argv[0]);
		return;
	}

	difficulty = atoi(argv[3]);

	if (difficulty < 1 || difficulty > 10) {
		printf("Invalid Difficulty. Difficulty must be > 0 and < 11.\n");
		return;
	}

	m = atoi(argv[1]);
	n = atoi(argv[2]);


	if (m < 3 || n < 3 || m > 100 || n > 100) {
		printf("Invalid board size. Board size must be > 2, 2 and < 100, 100.\n");
		return;
	}
	
	num_of_mines = getNumberOfMines(m, n, difficulty);

	do { //loop until player chooses to end game

		memset(game, GAME_EMPTY_CELL, sizeof(game)); //initialize game to GAME_EMPTY_CELL
		memset(display, DISPLAY_HIDDEN_CELL, sizeof(display)); //initialize display to DISPLAY_HIDDEN_CELL
		setBoundingCells(game, GAME_BOUNDING_CELL, m, n);
		placeMines(game, m, n, num_of_mines);
		hit_a_mine = 0;
		num_of_hidden_cells = m * n;
		printBoard(display, m, n);
		do { //loop until player hits a mine or clears the board

			printf("Choose a cell (r c): ");

			scanf("%d %d", &r, &c);
			printf("\n");
			
			if (inputIsValid(r, c, m, n)) {
				if (display[r - 1][c - 1] == DISPLAY_HIDDEN_CELL) {
					move(r, c, m, n, display, game, &hit_a_mine, &num_of_hidden_cells);
					system("tput clear");
					printBoard(display, m, n);
				} else {
					system("tput clear");
					printBoard(display, m, n);
					printf("This location has been cleared. Select a different location.\n");
				}
			} else {
				printf("Invalid input.\n");
			}
			

		} while((!hit_a_mine) && (num_of_hidden_cells != num_of_mines));

		system("tput clear");

		if (hit_a_mine) {
			addMinesToDisplay(display, game, m, n);
			printf("***GAME IS OVER***\n\n\n");
			printBoard(display, m, n);
		} else if (num_of_hidden_cells == num_of_mines) {
			addMinesToDisplay(display, game, m, n);
			printf("***YOU WIN!***\n\n\n");
			printBoard(display, m, n);
		}

		while (1) { //loop until user enters y or n
			printf("Would You like to play again? (y/n): ");

			scanf("%s", playAgain);

			if (playAgain[0] == 'y') {
				system("tput clear");
				break;
			} else if (playAgain[0] == 'n') {
				break;
			} else {
				printf("Invalid Input.\n");
			}

		}


	} while(playAgain[0] == 'y');

}
