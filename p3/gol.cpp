/*
 * CSc103 Project 3: Game of Life
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
 * #hours: 12
 */

#include <cstdio>
#include <stdlib.h> // for exit();
#include <getopt.h> // to parse long arguments.
#include <unistd.h> // sleep
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using namespace std;

static const char* usage =
"Usage: %s [OPTIONS]...\n"
"Text-based version of Conway's game of life.\n\n"
"   --seed,-s     FILE     read start state from FILE.\n"
"   --world,-w    FILE     store current world in FILE.\n"
"   --fast-fw,-f  NUM      evolve system for NUM generations and quit.\n"
"   --help,-h              show this message and exit.\n";

size_t max_gen = 0; /* if > 0, fast forward to this generation. */
string wfilename =  "/tmp/gol-world-current"; /* write state here */
FILE* fworld = 0; /* handle to file wfilename. */
string initfilename = "/tmp/gol-world-current"; /* read initial state from here. */

/* NOTE: you don't have to write these functions -- this is just how
 * I chose to organize my code. */
size_t nbrCount(size_t i, size_t j, const vector<vector<bool> >& g);
void update(vector<vector<bool> >& world);
string initFromFile(const string& fname,const string& stdout);
void mainLoop(vector<vector<bool> >& world,size_t max_gen);
FILE* dumpState(FILE* f, FILE* stdout);

/* NOTE: you can use a *boolean* as an index into the following array
 * to translate from bool to the right characters: */
char text[3] = ".O";

int main(int argc, char *argv[]) {
	// define long options
	static struct option long_opts[] = {
		{"seed",    required_argument, 0, 's'},
		{"world",   required_argument, 0, 'w'},
		{"fast-fw", required_argument, 0, 'f'},
		{"help",    no_argument,       0, 'h'},
		{0,0,0,0}
	};
	// process options:
	char c;
	int opt_index = 0;
	while ((c = getopt_long(argc, argv, "hs:w:f:", long_opts, &opt_index)) != -1) {
		switch (c) {
			case 'h': // help
				printf(usage,argv[0]);
				return 0;
			case 's': // input file
				initfilename = optarg;
				break;
			case 'w': // output file
				wfilename = optarg;
				break;
			case 'f': // number of generations
				max_gen = atoi(optarg);
				break;
		//	case '?': // none existent
		//		printf(usage,argv[0]);
		//		return 1;
		}
	}
	/* NOTE: at this point wfilename initfilename and max_gen
	 * are all set according to the command line: */
	printf("input file:  %s\n",initfilename.c_str());
	printf("output file: %s\n",wfilename.c_str());
	printf("fast forward to generation: %lu\n",max_gen);
	/* TODO: comment out 3 lines above once you see what's in them. */
	/* NOTE: if wfilename == "-" you should write to stdout, not a file
	 * named "-"! */

	/* If you wrote the initFromFile function, call it here: */
    vector<vector<bool> > world;
    
    initFromFile(initfilename,wfilename);
    mainLoop(world,max_gen);
	return 0;
}

// translate the file to a boolean vector and make a copy of it
string initFromFile(const string& fname,const string& stdout){
    FILE* f = fopen(fname.c_str(),"rb"); // note conversion to char*
    FILE* newWorld = fopen(stdout.c_str(),"rb");
    char c;
    
    // read the file and turn it into a vector
    while (fread(&c,1,1,f)){
        fwrite(&c,1,1,newWorld);
    }
    return stdout;
}

// update every time
void update(vector<vector<bool> >& world, FILE* stdout){
    char c;
    int neighbors = 0;
    stdout = fopen(wfilename.c_str(),"wb");
    // checking for neighbors
    for(int i = 0; i < world.size(); i++){
        for(int j = 0; j < world[i].size(); j++){
            
            if(i != world.size() && world[i+1][j] == true){ // checking above
                neighbors++;
            }else if(i != 0 && world[i-1][j] == true){ // checking below
                neighbors++;
            }else if(j != 0 && world[i][j-1] == true){ // checking left
                neighbors++;
            }else if(j != world[i].size() && world[i][j+1] == true){ // checking right
                neighbors++;
            }else if(i != 0 && j != world[i].size() && world[i-1][j+1] == true){ // checking upper right
                neighbors++;
            }else if(i != world.size() && j != world[i].size() && world[i+1][j+1] == true){ // checking lower right
                neighbors++;
            }else if(i != world.size() && j != 0 && world[i-1][j-1] == true){ // checking upper left
                neighbors++;
            }else if(i != world.size() && j != 0 && world[i+1][j-1] == true){ // checking lower left
                neighbors++;
            }
            
            // say whether the cell is alive or dead based on the number of neighbors sorounding it
            if (neighbors > 3 || neighbors < 2){
                c = '.';
                world[i].push_back(false); // state it is dead
                fwrite(&c,1,1,stdout);
                fclose(stdout);
            } else if (neighbors == 3 || neighbors == 2){
                c = 'O';
                world[i].push_back(true); // state it is alive
                fwrite(&c,1,1,stdout);
                fclose(stdout);
            }
        }
    }
}


// display the answer in 2 different modes
void mainLoop(vector<vector<bool> >& world, size_t max_gen){
    /* TODO: write this */
	/* update, write, sleep */
    
    // mode 1, display new generation every second
	if (max_gen == 0) {
		/* make one generation update per second */
        while(true){
            update(world,stdout);// update
            sleep(1); // wait 1 second
                printf(" ",stdout); // write
        }
        
    // mode 2, skip to last generation
	} else {
		/* go through generations as fast as you can until
		 * max_gen is reached... */
        FILE* stdout = fopen(wfilename.c_str(),"wb");
        int generations = 0;
        while(generations < max_gen){
            update(world,stdout);
            generations++;
        }
        printf(" ",stdout);
    }
}
