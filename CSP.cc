/**************
 *** CSP.CC ***
 **************/

#include <vector>

using namespace std;

typedef vector< vector < unsigned int > >::pointer CSP;

extern CSP sudoku;

/*
 * Relative Position of puzzle
 *---------------------------------
 *| 0  1  2  | 3  4  5 |  6  7  8 | 
 *| 9  10 11 | ...................|
 *|...............................|
 *|..................... 78 79 80 |
 *---------------------------------
 */

bool complete() {
	for ( unsigned int i = 0; i < 9; i += 1 ) {
		for ( unsigned int j = 0; j < 9; j += 1 ) {
			if ( sudoku[i][j] == 0 ) return false; 	// Empty slot, not complete
		}
	}
	return true;
}

bool checkConstraints( unsigned int value, unsigned int x, unsigned int y ) {
	unsigned int i, j;

	// Check row constraints
	for ( i = 0; i < 9; i += 1 ) 
		if ( sudoku[x][i] == value ) return false;

	// Check colomn constraints
	for ( i = 0; i < 9; i += 1 ) 
		if ( sudoku[i][y] == value ) return false;

	// Check block constraints
	for ( i = 3 * ( x / 3 ); i < 3 * ( x / 3 ) + 3; i += 1 ) {
		for ( j = 3 * ( y / 3 ); j < 3 * ( y / 3 ) + 3; j += 1 ) {
			if ( sudoku[i][j] == value ) return false;
		}
	}

	// costly check
	//	for ( i = 0; i < 100000; i += 1 );

	return true;
}
