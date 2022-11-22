// Mark Valentino CSCE 355
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using std::string;

const int MAXSIZE = 4096;
// Holds all transition functions with symbols compartmentalized.
string transitions[MAXSIZE][6];
string tape[MAXSIZE];
// Position of the TM's head.
int headPosition;
int numNonHaltingStates = 0;
string alphabet = "";
// State of the TM
string state = "0";
// Holds the last uniquely generated tape output.
string tempTapeOutput = "";

// Generates what the Turing machine is currently doing in string format.
string GenerateTapeOutput() {
    string tempString = state + ":";
    for (unsigned int i = 0; i < MAXSIZE; i++) {
        if (i == headPosition) {
            tempString += '(';
            if (tape[i] == "_") {
               tempString += '_'; 
            }
        }
        if (tape[i] != "_") {
            tempString += tape[i];
        }
        if (i == headPosition) {
            tempString += ')';
        }
    }
    return tempString;
}

// Prints the tape if it has changed.
void printTape() {
    if (GenerateTapeOutput() != tempTapeOutput) {
        tempTapeOutput = GenerateTapeOutput();
        std::cout << tempTapeOutput << endl;
    }
}

// Where the main action of the Turing machine is. 
void readTape() {
    int missCount = 0;
    while (1){
        // For iterating through transitions list
        for (unsigned int i = 0; i < MAXSIZE; i++) {
            printTape();
            if (transitions[i][0] == state && transitions[i][1] == tape[headPosition]) {
                missCount = 0;
                state = transitions[i][3];
                tape[headPosition] = transitions[i][4] + "";
                //cout << transitions[i][3].at(0) << endl;
                if (transitions[i][5] == "<") {
                    headPosition--;
                } else if (transitions[i][5] == ">") {
                    headPosition++;
                }
                i = 0;
            }
            printTape();
        }
        // If the accepting state has been reached.
        if (stoi(state) == numNonHaltingStates) {
            for (unsigned int i = 0; i < MAXSIZE; i++) {
                if (state == transitions[i][3]) {
                    cout << "accept" << endl;
                    break;
                }
            }
           break;
        }
        missCount++;
        if (missCount == 3) {
            cout << "reject" << endl;
                    break;
        }
    }
}

// Creates the tape from user Input.
void createTape(string input) {
    // Adds blanks to the tape.
    for (unsigned int i = 0; i < MAXSIZE; i++) {
        tape[i] = "_";
    }
    // Puts the string inputed by the user approximately in the middle of the tape.
    for (unsigned int i = 0; i < input.length(); i++) {
        tape[2048 + i] = input.at(i);
    }
    headPosition = 2048;
}

// Function to compartmentalize the symbols on every transition line and fill out transitions array.
void processTransition(string line, int index) {
    string temp = "";
    int compartment = 0;
    for (unsigned int i = 0; i < line.length(); i++) {
        if (line.at(i) == ' ' || i == line.length() - 1) {
            transitions[index][compartment] = temp;
            temp = "";
            compartment++;
        } else {
            temp += line.at(i);
        }  
    }
}

// Function to remove unneeded text on Turring Machine text files.
string processLine(string line) {
    string filteredString = "";
    
    for (unsigned int i = 0; i < line.length(); i++) {
        if (line.at(i) == '>') {
            filteredString += '>';
            break;
        } else if (line.at(i) == '<') {
            filteredString += '<';
            break;
        } else {
            filteredString += line.at(i);
        }
    }
    return filteredString + '\n';
}

// Function to parse given text file and fill out the global variables.
void parseFile(string argv) {
    std::ifstream file;
    file.open(argv);
    string templine;
    int lineCount = 0;
    if (file.is_open() ) {
        while (file) {
            std::getline (file, templine);
            lineCount++;
            // Get number of non-halting states
            if (lineCount == 1) {
                // The two right-most chars have the number needed.
                numNonHaltingStates = stoi(templine.substr(templine.length() - 2, templine.length()));
            // Get alphabet of Turing Machine
            } else if (lineCount == 2) {
                // Index 10 is where the alphabet begins on the line.
                alphabet = templine.substr(10, templine.length());
            }
            else {
                // Proccesses the current read line and adds data to transitions array.
                processTransition(processLine(templine), lineCount - 3);
            }
        }
    }   else {
        std::cout << "File couldn't be opened." << endl;
    }
}

int main(int argc, char *argv[]) {
    // Read given text file from command line.
    parseFile(argv[1]);
    string input;
    while (1) {
        // Reset for next calculation
        state = "0";
        tempTapeOutput = "";
        
        cin >> input;
        createTape(input);
        readTape();
    }
    return 0;      
}
