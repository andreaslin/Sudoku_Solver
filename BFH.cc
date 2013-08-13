/**************************************************************
 *** BFH.CC (Backtracking + Forward Checking + 3 Heuristics ***
 **************************************************************/

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

// Naive class for update
class lm {
public:
	enum UPDATE { ADD, REMOVE };
};

// Calculate the number of not filled variable related to (x,y)
unsigned int numberOfConstraints( unsigned x, unsigned y ) {
	unsigned int i, j, cons = 0;

	// Remember not to count itself as a constraint
	// Check row
	for ( i = 0; i < 9; i += 1 ) 
		if ( sudoku[x][i] == 0 && i != y ) cons += 1;

	// Check column
	for ( i = 0; i < 9; i += 1 )
		if ( sudoku[i][y] == 0 && i != x ) cons += 1;

	// Check block
	// Remember not to check the position where we've already checked in row and column
	for ( i = 3 * ( x / 3 ); i < 3 * ( x / 3 ) + 3; i += 1 ) {
		for ( j = 3 * ( y / 3 ); j < 3 * ( y / 3 ) + 3; j += 1 ) {
			if ( sudoku[i][j] == 0 && i != x && j != y ) cons += 1;
		}
	}	

	return cons;
}


// Most constrained and most constraining heuristic
unsigned int selectVariable() {
	list<unsigned int> min_positions;

	// Most constrained heuristic
	for ( unsigned int i = 0; i < 9; i += 1 ) {
		for ( unsigned int j = 0; j < 9; j += 1 ) {
			if ( sudoku[i][j] == 0 ) {
				if ( min_positions.size() == 0 ) { 			// Set a minimum if list is empty
					min_positions.push_back( i*9 + j );
				} else {									
					unsigned int min_size = legalMap[min_positions.front()].size();
					unsigned int size = legalMap[i*9+j].size();
					if ( size < min_size ) {				// If we have a smaller position, update
						min_positions.clear();
						min_positions.push_back( i*9 + j );
					} else if ( size == min_size ) { 		// If we have a tie, push back to compare later 
						min_positions.push_back( i*9 + j );
					}
				}
			}
		}
	}

	if ( min_positions.size() == 1 ) return min_positions.front();
		
	// Most constraining heuristic
	unsigned int size = min_positions.size();
	unsigned int min_pos = min_positions.front();
	min_positions.pop_front();
	unsigned int pos;
	for ( unsigned int i = 0; i < size - 1; i += 1 ) {
		pos = min_positions.front();
		if ( numberOfConstraints( pos / 9, pos % 9 ) < numberOfConstraints( min_pos / 9, min_pos % 9 ) )
			min_pos = pos;
			}

	return min_pos;
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


/***********************
 *** DEBUGGING TOOLS ***
 ***********************/
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

void printList( list<unsigned int> l ) {
	for ( list<unsigned int>::iterator i = l.begin(); i != l.end(); i++ ) {
		cout << *i << " ";
	}
	cout << endl;
}

/*************************************************************************************/

unsigned int numberNotFilled( unsigned int x, unsigned int y, unsigned int value ) {
	unsigned int num = 0;
	unsigned int size;
	unsigned int i, j;
	list<unsigned int> l;

	// Check row
	for ( i = 0; i < 9; i += 1 ) {
		if ( sudoku[x][i] == 0 && i != y ) {
			l = legalMap[x*9 + i];
			size = l.size(); 				// Record original size of legal moves 
			l.remove( value );				// Try removing the value
			if ( size != l.size() ) {		// If the legal moves contains value
				num += ( size - 1 );		// Remaining legal moves - 1
			} else {						// Does not cotain value
				num += size;				// Same number of legal moves 
			}
		}
	}
			
		
	// Check column
	for ( i = 0; i < 9; i += 1 ) {
		if ( sudoku[i][y] != 0 && i != x ) {
			l = legalMap[i*9 + y];
			size = l.size();				// Record original size of legal moves 
			l.remove( value );				// Try removing the value
			if ( size != l.size() ) {		// If the legal moves contains value
				num += ( size - 1 );		// Remaining legal moves - 1
			} else {						// Does not cotain value
				num += size;				// Same number of legal moves 
			}			
		}
	}
	
	// Check block
	for ( i = 3 * ( x / 3 ); i < 3 * ( x / 3 ) + 3; i += 1 ) {
		for ( j = 3 * ( y / 3 ); j < 3 * ( y / 3 ) + 3; j += 1 ) {
			if ( sudoku[i][j] != 0 && i != x && j != y ) {
				l = legalMap[i*9 + j];
				size = l.size();				// Record original size of legal moves 
				l.remove( value );				// Try removing the value
				if ( size != l.size() ) {		// If the legal moves contains value
					num += ( size - 1 );		// Remaining legal moves - 1
				} else {						// Does not cotain value
					num += size;				// Same number of legal moves 
				}			
			}
		}
	}

	return num;
}

bool notFilled( pair<unsigned int, unsigned int> a, pair<unsigned int, unsigned int> b ) {
	return a.second <= b.second;
}

list<unsigned int> leastConstraining( unsigned int x, unsigned int y, list<unsigned int> legal ) {
	list< pair<unsigned int, unsigned int> > l;
	list<unsigned int> r;
	unsigned int size = legal.size();

	// Organize the legal moves into another list for sorting
	for ( unsigned int i = 0; i < size; i += 1 ) {
		unsigned int value = legal.front();
		l.push_back( pair<unsigned int, unsigned int>(value,numberNotFilled( x, y, value ) ) );
		legal.pop_front();
	}

	// Sort the value accroding to the legal moves
	l.sort( notFilled );

	// Reorganized the legal moves list
	size = l.size();
	for ( unsigned int i = 0; i < size; i += 1 ) {
		r.push_back( l.front().first );
		l.pop_front();
	}

	return r;
}


bool backtracking() {
	unsigned int variable;		// The relative position of the variable
	list<unsigned int> legal;

	if ( node == 0 ) initialize(); // First initialize the map

	if ( complete() ) {
		return true;			// DONE!!!
	} else {
		variable = selectVariable();				   	// Select an unassigned value
		unsigned int x = variable / 9; 				   	// actual row position of variable 
		unsigned int y = variable % 9;				    // actual column position of variable
		legal = legalMap[x*9 + y];						// Forward checking
		legal = leastConstraining( x, y, legal );	   	// Least-constraining heuristic
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

// Least-constraining: plug in the value, update the variables' neighbours legal move, 
//					sum the updated legal move up. Do this for all legal value in that variable, then sort.

