/*
 * CSc103 Project 5: Syntax highlighting, part two.
 * See readme.html for details.
 * Please list all references you made use of in order to complete the
 * assignment: your classmates, websites, etc.  Aside from the lecture notes
 * and the book, please list everything.  And remember- citing a source does
 * NOT mean it is okay to COPY THAT SOURCE.  What you submit here **MUST BE
 * YOUR OWN WORK**.
 * References: Dude, trust me
 *
 *
 * Finally, please indicate approximately how many hours you spent on this:
 * #hours: 5
 */

#include "fsm.h"
using namespace cppfsm;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <set>
using std::set;
#include <map>
using std::map;
#include <initializer_list> // for setting up maps without constructors.

// enumeration for our highlighting tags:
enum {
	hlstatement,  // used for "if,else,for,while" etc...
	hlcomment,    // for comments
	hlstrlit,     // for string literals
	hlpreproc,    // for preprocessor directives (e.g., #include)
	hltype,       // for datatypes and similar (e.g. int, char, double)
	hlnumeric,    // for numeric literals (e.g. 1234)
	hlescseq,     // for escape sequences
	hlerror,      // for parse errors, like a bad numeric or invalid escape
	hlident       // for other identifiers.  Probably won't use this.
};

// usually global variables are a bad thing, but for simplicity,
// we'll make an exception here.
// initialize our map with the keywords from our list:
map<string, short> hlmap = {
#include "res/keywords.txt"
};
// note: the above is not a very standard use of #include...

// map of highlighting spans:
map<int, string> hlspans = {
	{hlstatement, "<span class='statement'>"},
	{hlcomment, "<span class='comment'>"},
	{hlstrlit, "<span class='strlit'>"},
	{hlpreproc, "<span class='preproc'>"},
	{hltype, "<span class='type'>"},
	{hlnumeric, "<span class='numeric'>"},
	{hlescseq, "<span class='escseq'>"},
	{hlerror, "<span class='error'>"}
};
// note: initializing maps as above requires the -std=c++0x compiler flag,
// as well as #include<initializer_list>.  Very convenient though.
// to save some typing, store a variable for the end of these tags:
string spanend = "</span>";

string translateHTMLReserved(char c) {
    switch (c) {
        case '"':
            return "&quot;";
        case '\'':
            return "&apos;";
        case '&':
            return "&amp;";
        case '<':
            return "&lt;";
        case '>':
            return "&gt;";
        case '\t': // make tabs 4 spaces instead.
            return "&nbsp;&nbsp;&nbsp;&nbsp;";
        default:
            char s[2] = {c,0};
            return s;
    }
}

void scanLine(string itemInLine, string lineInHTML){ // function to read scan line and turn into HTML
    int state = start;
    bool newLine = false;
    string end = "\\n";
    string storage = ""; // what is being scanned
    for (unsigned long i = 0; i < itemInLine.length(); i++){
        updateState(state,itemInLine[i]); // change state into numbers to detect
        switch(state){
            case start: // state = 0; if there is a space, then just add it to output
                lineInHTML += translateHTMLReserved(itemInLine[i]);
                break;
                
            case scanid: // state = 1; if number or leters, scan for them
                storage += itemInLine[i];
                while(state == scanid){ // make a temporary string to be used for matching with hlmap
                    i++;
                    updateState(state,itemInLine[i]);
                    storage += itemInLine[i];
                }
                i--;
                
                storage = storage.substr(0,storage.length()-1); // remove the space at the end
                switch(hlmap.find(storage) -> second){ // matching with hlmap
                    case hlstatement: // its a satement
                        if (storage == "using" || storage == "namespace" || storage == "if" || storage == "else" || storage == "while" || storage == "do" || storage == "for" || storage == "range for" || storage == "continue" || storage == "goto" || storage == "case" || storage == "break" || storage == "default" || storage == "return"){
                            lineInHTML += hlspans[hlstatement] + storage + spanend;
                        }else{
                            lineInHTML += storage;
                        }
                        break;
                    case hltype: // its a data type
                        lineInHTML += hlspans[hltype] + storage + spanend;
                        break;
                    case hlpreproc: // its a directive
                        lineInHTML += hlspans[hlpreproc] + storage + spanend;
                        break;
                    default:
                        lineInHTML += storage;
                        break;
                }
                storage.clear(); // delete the string being scanned, so it can be reused
                break;
            
            case comment: // state = 2; if scanning a comment
                while(i < itemInLine.size()){ // make a temporary string to be used for matching with hlmap
                    updateState(state,itemInLine[i]);
                    storage += itemInLine[i];
                    i++;
                }
            
                storage = storage.substr(0,storage.length()-1); // remove the space at the end
                lineInHTML += hlspans[hlcomment] + storage + spanend;
                storage.clear();
                break;
            
             case strlit: // state = 3; if scanning a string
                i++;
                storage += itemInLine[i-1];
                while (state == strlit){
                    if (i != itemInLine.length()){
                        updateState(state,itemInLine[i]);
                        storage += itemInLine[i];
                        i++;
                    }
                    if (itemInLine[i] == '\\' && itemInLine[i+1] == 'n'){
                        i = i+2;
                        newLine = true;
                    }
                    if (updateState(state,itemInLine[i]) == error){
                        storage = storage.substr(0,storage.length()-1); // remove the space at the end
                    }
                }
                
                if (newLine == true){
                    lineInHTML += hlspans[hlstrlit] + storage + spanend + hlspans[hlescseq] + end + spanend;
                    lineInHTML += hlspans[hlstrlit] + '"' + spanend;
                }else if (itemInLine[i] == '"'){
                    lineInHTML += hlspans[hlstrlit] + storage + '"' + spanend;
                }else{
                    lineInHTML += hlspans[hlstrlit] + storage + spanend;
                }
                storage.clear();
                break;
                
            case readfs: // state = 4; scanning a / (possible comment)
                if (itemInLine[i+1] != '/'){
                    lineInHTML += itemInLine[i];
                } else{
                    i--;
                }
                break;
                
            case scannum: // state = 6; scanning numbers (1,2,3,4,5...)
                while(state == scannum){ // store the whole comment
                    if (i != itemInLine.length()){
                        updateState(state,itemInLine[i]);
                        storage += itemInLine[i];
                        i++;
                    }
                }
                i--;
                
                storage = storage.substr(0,storage.length()-1); // remove the space at the end
                if (itemInLine[i] == ';'){
                    lineInHTML += hlspans[hlnumeric] + storage + spanend + itemInLine[i];
                } else{
                    lineInHTML += hlspans[hlnumeric] + storage + spanend;
                }
                storage.clear();
                break;
                
            case error: // state = 7; there was an error
                i--;
                while(updateState(state,itemInLine[i]) == error){ // store the whole comment
                    if (i != itemInLine.length()){
                        updateState(state,itemInLine[i]);
                        storage += itemInLine[i];
                        i++;
                    } else{
                        state = start;
                    }
                }
                storage = storage.substr(0,storage.length()-1); // remove the space at the end

                lineInHTML += hlspans[hlerror] + storage + spanend;
                storage.clear();
                break;
        }
    }
    cout << lineInHTML << endl;
}

int main() {
    // TODO: write the main program.
    // It may be helpful to break this down and write
    // a function that processes a single line, which
    // you repeatedly call from main().
    string lineBeingRead; // line being read
    string lineInHTML; // line in HTML
    while(getline(cin,lineBeingRead)){
        lineBeingRead += " "; // add space in the end so it doesn't break
        scanLine(lineBeingRead, lineInHTML);
        lineInHTML = ""; // reset output
    }
    return 0;
}
