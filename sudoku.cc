/*****************
 *** SUDOKU.CC ***
 *****************/

#include <iostream>
#include <fstream>
#include <cstdlib>				// exit
#include <vector>				// vector
#include "printer.cc"			// printer

// Sudoku solve type
#if defined ( BACK )
#include "B.cc"					// backtracking algorithm
#elif defined ( BF )
#include "BF.cc"				// backtracking + forwarding
#elif defined ( BFH )
#include "BFH.cc"				// backtracking + forwarding + 3 heuristics
#else
	#error unsupported solve type
#endif

using namespace std;			// direct access to std

CSP sudoku;						// Sudoku puzzle
extern unsigned int node;		// Number of nodes
extern unsigned int steps;		// Number of steps


void usage( char *argv[] ) {
    cerr << "Usage: " << argv[0] << "input-file" << endl;
    exit( EXIT_FAILURE );		// TERMINATE
} // usage

int main( int argc, char *argv[] ) {
    istream *infile;				// default value
	vector< vector< unsigned int > > s(9);		   	// sudoku puzzle 
	sudoku = &s[0];
	
    switch ( argc ) {
	case 2:
        infile = new ifstream( argv[1] );
        if ( infile->fail() ) {		// open failed ?
			cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
			usage( argv );
		} // if
		break;
	default:						// wrong number of options
		usage( argv );
    } // switch

	// Initialize sudoku puzzle
	for ( unsigned int i = 0; i < 9; i += 1 ) {
		vector<unsigned int> r(9);
		sudoku[i] = r;
	}

	// Read input
	char ch;
    for ( unsigned int i = 0; i < 9; i += 1 ) {				   
		for ( unsigned int j = 0; j < 9; j += 1 ) {
			*infile >> ch;					// read a character
			//cout << ch << endl;
			sudoku[i][j] = (int)ch - 48;	   	// write into puzzle
		}
	}	

	// Print initial puzzle
	cout << "Given sudoku puzzle" << endl;
	print( &sudoku[0] );
	cout << endl;

	// Solve it
	bool result = backtracking();

	// Print solved puzzle
	if ( result ) {
		cout << "Solved puzzle" << endl;
		print( &sudoku[0] );
		cout << endl;

		// Number of node
		cout << "Number of nodes: " << node << endl;
		
	} else {
		cout << "Error: puzzle not solved." << endl;
	}

    if ( infile != &cin ) delete infile;// close file, do not delete cin!
} // main
