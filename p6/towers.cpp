/*
 * CSc103 Project 6: Towers of Hanoi
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References:
 *
 *
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 3
 */

// TODO: write the program.
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <getopt.h> // to parse long arguments.
#include <cstdlib> // for atoi function

void solvingPuzzle(short n, short start, short end){ // function to solve puzzle
    if (n == 1) { // if there is only one disk, it is solved; Display as is
        cout << start << '\t' << end << '\n';
    }
    else { // mutliple disks
        solvingPuzzle(n - 1, start, 6-start-end); //  move disks to the next peg available
        cout << start << '\t' << end << '\n'; //  display actions done
        solvingPuzzle(n - 1, 6-start-end, end); // move disks to the last peg
    }
}

/* Here's a skeleton main function for processing the arguments. */
int main(int argc, char *argv[]) {
	// define long options
	static struct option long_opts[] = {
		{"start",        required_argument, 0, 's'},
		{"end",          required_argument, 0, 'e'},
		{"num-disks",    required_argument, 0, 'n'},
		{0,0,0,0} // this denotes the end of our options.
	};
	// now process the options:
	char c; // to hold the option
	int opt_index = 0;
	short n=0,start=0,end=0; /* to store inputs to the towers function. */
	while ((c = getopt_long(argc, argv, "s:e:n:", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 's': // process option s
				start = atoi(optarg);
				break;
			case 'e': // process option e
				end = atoi(optarg);
				break;
			case 'n': // process option n
				n = atoi(optarg);
				break;
			case '?': // this will catch unknown options.
				// here is where you would yell at the user.
				// although, getopt will already print an error message.
                cout << "This inputs do not work, try again with something that makes sense\n";
				return 1;
		}
	}

	/* TODO: now that you have the options and arguments,
	 * solve the puzzle. */

    if (start != end || n >= 1){ // There is more than one peg; solve puzzle
        solvingPuzzle(n, start, end);
    }
    else { // There is only one peg; it is already in order
        cout << "already solved\n";
    }
    
	return 0;
}
