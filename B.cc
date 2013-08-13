/**************************
 *** B.CC (Backtracing) ***
 **************************/

#include <iostream>
#include <vector>
#include "CSP.cc"					// CSP

using namespace std;

extern CSP sudoku;

unsigned int node = 0;
unsigned int steps = 0;

unsigned int selectVariable() {
	for ( unsigned int i = 0; i < 9; i += 1 ) {
		for ( unsigned int j = 0; j < 9; j += 1 ) {
			if ( sudoku[i][j] == 0 ) return i*9 + j;
		}
	}
	return 0;
}

bool backtracking() {
	unsigned int variable;		// The relative position of the variable
	if ( complete() ) {
		return true;			// DONE!!!
	} else {
		variable = selectVariable();	  		// Select an unassigned value
		int x = variable / 9; 					// actual row position of variable 
		int y = variable % 9;					// actual column position of variable
		for ( unsigned int i = 1; i < 10; i += 1 ) { 		   	// Go through all values 
			steps += 1;
			if ( checkConstraints( i, x, y ) ) { 	// Check constraints
				node += 1;			   	 			// Go down one node 
				sudoku[x][y] = i;
				if ( backtracking() ) { 						// Next iteration 
					return true;
				} else {
					sudoku[x][y] = 0;
				}
			}
		}
		return false;
	}
}
