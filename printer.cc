/******************
 *** PRINTER.CC ***
 ******************/

#include <iostream>				// cout
#include <vector>				// vector
#include <iomanip>				// setfill, setw

using namespace std;

void print( vector< vector< unsigned int > >::pointer sudoku ) {
	unsigned int i, j;
	unsigned int c = 0;
	unsigned int BOARDER_WIDTH = 25;

	// Top boarder
	cout << setfill('-') << setw( BOARDER_WIDTH ) << "-" << endl;

	// Sudoku content
	cout << setw(0) << setfill(' ');
	for ( i = 0; i < 9; i += 1  ) {
		for ( j = 0; j < 9; j += 1 ) {
			if ( c % 3 == 0 ) cout << "| ";
			cout << sudoku[i][j] << " ";
			c += 1;
		}
		cout << "|" << endl;
		// boarder for every 3 rows printed
		if ( (i + 1) % 3 == 0 ) {
			cout << setfill('-') << setw( BOARDER_WIDTH ) << "-" << endl;
			cout << setfill(' ') << setw(0);
		}
	}
}
