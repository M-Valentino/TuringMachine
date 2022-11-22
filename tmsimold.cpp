// Mark Valentino CSCE 355
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
using std::string;

const int MAXSIZE = 4096;
// Holds all transition functions with symbols compartmentalized.
string transitions[MAXSIZE][6];
char tape[MAXSIZE];
int headPosition;
int numNonHaltingStates = 0;
string alphabet = "";
string state = "0";

void readSymbol(string symbol) {
    for (unsigned int i = 0; i < MAXSIZE; i++) {
        if (transitions[i][0] == state && transitions[i][1] == symbol) {
            state = transitions[i][3];
            
        }
    }
}

void readString(string symbols) {
    string temp = "";
    for (unsigned int i = 0; i < symbols.length(); i++) {
        temp += symbols.at(i);
        readSymbol(temp);
        temp = "";
    }
}

void createTape(string input) {
    for (unsigned int i = 2048; i < input.length(); i++) {
        tape[i] = input.at(i);
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
    for (unsigned int i = 0; i < MAXSIZE; i++) {
        if (transitions[i][0] == "") {
            break;
        }
        cout << transitions[i][0] << " " << transitions[i][1] << " " << transitions[i][2] << " " << transitions[i][3] << " " << transitions[i][4] << " " << transitions[i][5] << endl;
    }
    while (1) {
        cin >> input;
        createTape(input);
    }
    return 0;      
}
