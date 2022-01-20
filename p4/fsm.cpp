/*
 * CSc103 Project 4: Syntax highlighting, part one.
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

#include "fsm.h"
using namespace cppfsm;
#include <vector>
using std::vector;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

// make sure this function returns the old state.  See the header
// file for details.
int cppfsm::updateState(int& state, char c) {
    int output = state; // value before updating
    
    // check what was inputed
    switch (state) {
            // state = 0; scanning at the start of the line
        case start:
            if (c == '/'){ // check if its a possible comment
                state = readfs;
            }else if(c == '"'){ // check if its a string
                state = strlit;
            }else if(ident_st.find(c) != ident_st.end()){ // check if its at the end of the line
                state = scanid;
            }else if(num.find(c) != num.end()){ // check if its a num
                state = scannum;
            }
            break;
            
            // state = 1; scanning at the middle of the line
        case scanid:
            if (c == '/'){ // check if it is a possible comment
                state = readfs;
            }else if(c == '\"'){ // say string hasn't ended
                state = strlit;
            }else if(iddelim.find(c) != iddelim.end()){ // check if its a space
                state = start;
            }else if(num.find(c) != num.end()){ // check if its a num
                state = scannum;
            }
            break;
            
            // state = 2; scanning for comments
        case comment:
            break; // it is a comment, everything else is a state of 2 (does not count)
            
        // state = 3
        case strlit: // scanning string
            if (c == '"'){ // check if string
                state = start;
            }else if(c == '\\'){ // check if at the end, start end sequence
                state = readesc;
            }
            break;

            // state 4; scanning for possible comments
        case readfs:
            if (c == '/'){
                state = comment;
            }else if(c == '"'){ // check if its a string
                state = strlit;
            }else if(ident_st.find(c) != ident_st.end()){ // check if at the end of line
                state = scanid;
            }else if(num.find(c) != num.end()){ // check if a num
                state = scannum;
            }
            break;
            
            // state 5; read end of line and escape
        case readesc:
            if (escseq.find(c) != escseq.end()){ // not the end, keep reading a string
                state = strlit;
            }else{
                state = error; // there is an error
            }
            break;
        
            // state 6; scanning numbers
        case scannum:
            if (num.find(c) != num.end()){ // check if it is a number
                state = scannum;
            }else if(iddelim.find(c) != iddelim.end()){ // check for space
                state = start;
            }else{ // there is an error
                state = error;
            }
            break;
            
            // state 7; state there is an error
        case error:
            break;

    }
    return output;
    return 0;
}

