/***********************************************
 *** BF.CC (Backtracking + Forward Checking) ***
 ***********************************************/

#include <iostream>
#include <vector>				// vector
#include <list>					// list
#include <map>					// map
#include "CSP.cc"			   	// CSP

using namespace std;

extern CSP sudoku;

unsigned int node = 0;
unsigned int steps = 0;
map<unsigned int, list<unsigned int> > legalMap;

// Naive class for enum
class lm {
public:
	enum UPDATE { ADD, REMOVE };
};

unsigned int selectVariable() {
	for ( unsigned int i = 0; i < 9; i += 1 ) {
		for ( unsigned int j = 0; j < 9; j += 1 ) {
			if ( sudoku[i][j] == 0 ) return i*9 + j;
		}
	}
	return 0;
}

list<unsigned int> legalMoves( unsigned int x, unsigned int y ) {
	unsigned int i, j;
	list<unsigned int> legal;

	// No legal moves for assigned variable
	if ( sudoku[x][y] != 0 ) {
		legal.resize(0);
		return legal;
	}

	// Initialize legal list
	for ( i = 0; i < 9; i += 1 ) 
		legal.push_back( i + 1 );

	// Check row
	for ( i = 0; i < 9; i += 1 ) 
		if ( sudoku[x][i] != 0 ) legal.remove( sudoku[x][i] );

	// Check column
	for ( i = 0; i < 9; i += 1 )
		if ( sudoku[i][y] != 0 ) legal.remove( sudoku[i][y] );

	// Check block
	for ( i = 3 * ( x / 3 ); i < 3 * ( x / 3 ) + 3; i += 1 ) {
		for ( j = 3 * ( y / 3 ); j < 3 * ( y / 3 ) + 3; j += 1 ) {
			if ( sudoku[i][j] != 0 ) legal.remove( sudoku[i][j] );
		}
	}

	return legal;
}

void initialize() {
	// Calculate all legal moves for variable not are not filled
	for ( unsigned int i = 0; i < 9; i += 1 ) {
		for ( unsigned int j = 0; j < 9; j += 1 ) {
			list<unsigned int> l = legalMoves( i, j );
			legalMap.insert( pair<unsigned int, list<unsigned int> >( i*9 + j, l ) );
		}
	}
}

void updateLegalMoves( unsigned int value, unsigned int x, unsigned int y, lm::UPDATE update ) {
	unsigned int i, j;

	legalMap[x*9+y].clear();	// Drop all legal moves
	// If we dropped the value, we need its legal moves again
	if ( update == lm::ADD ) legalMap[x*9+y] = legalMoves( x, y ); 

	// Always remove before insert to prevent duplicates

	// Check row
	for ( i = 0; i < 9; i += 1 ) {
		if ( sudoku[x][i] == 0 ) {
			legalMap[x*9+i].remove( value );
			if ( update == lm::ADD ) legalMap[x*9+i] = legalMoves( x, i );
		}
	}

	// Check column
	for ( i = 0; i < 9; i += 1 ) {
		if ( sudoku[i][y] == 0 ) {
			legalMap[i*9+y].remove( value );
			if ( update == lm::ADD ) legalMap[i*9+y] = legalMoves( i, y );
		}
	}

	// Check block
	for ( i = 3 * ( x / 3 ); i < 3 * ( x / 3 ) + 3; i += 1 ) {
		for ( j = 3 * ( y / 3 ); j < 3 * ( y / 3 ) + 3; j += 1 ) {
			if ( sudoku[i][j] == 0 ) {
				legalMap[i*9+j].remove( value );
				if ( update == lm::ADD ) legalMap[i*9+j] = legalMoves( i, j );
			}
		}
	}
	
}

// For debugging
void printMap() {
	for ( unsigned int i = 0; i < 9; i += 1 ) {
		for ( unsigned int j = 0; j < 9; j += 1 ) {
			list<unsigned int> l = legalMap[i*9+j];
			cout << "Position " << (i * 9 + j) << ":\t";
			for ( list<unsigned int>::iterator k = l.begin(); k != l.end(); k ++ )
				cout << *k << " ";
			cout << endl;
		}
	}
}


bool backtracking() {
	unsigned int variable;		// The relative position of the variable
	list<unsigned int> legal;

	// Remember to initialize
	if ( node == 0 ) initialize();

	if ( complete() ) {
		return true;			// DONE!!!
	} else {
		variable = selectVariable();				   	// Select an unassigned value
		unsigned int x = variable / 9; 				   	// actual row position of variable 
		unsigned int y = variable % 9;				    // actual column position of variable
		legal = legalMap[x*9 + y];		   				// Forward checking
		unsigned int size = legal.size();
		// Now we only check the legal list instead of all values
		for ( unsigned int i = 0; i < size; i += 1 ) {
			steps += 1;
			unsigned int value = legal.front();
			node += 1;			   	 			   	// Go down one node 
			sudoku[x][y] = value;
			// Update the legal moves of the selected variable's neighbour 
			updateLegalMoves( value, x, y, lm::REMOVE ); 	
			if ( backtracking() ) {				    // Next iteration 
				return true;
			} else {
				sudoku[x][y] = 0;
				// Add the value back to neighbour's legal moves 
				updateLegalMoves( value, x, y, lm::ADD );
			}
			legal.pop_front();
		}
		return false;
	}
}

